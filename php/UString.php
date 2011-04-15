<?php
namespace unicodestring;

use \ArrayAccess, \BadMethodCallException, \Countable,
    \InvalidArgumentException, \Normalizer, \OutOfBoundsException,
    \OutOfRangeException;

if (!extension_loaded('intl')) {
	die('unicodestring requires intl');
}

if (!extension_loaded('mbstring')) {
	die('unicodestring requires mbstring');
}

class UString implements ArrayAccess, Countable {
	private $data;

	public function __construct($str = '', $charset = 'UTF-8') {
		$this->set($str, $charset);
	}

	public function __toString() {
		return $this->data;
	}

	public function count() {
		return grapheme_strlen($this->data);
	}

	public function offsetExists($offset) {
		try {
			$this->checkOffset($offset);
			return true;
		}
		catch (OutOfRangeException $e) {
			return false;
		}
	}

	public function offsetGet($offset) {
		return grapheme_substr($this->data, $this->checkOffset($offset), 1);
	}

	public function offsetSet($offset, $value) {
		throw new BadMethodCallException('Grapheme setting is not implemented');
	}

	public function offsetUnset($offset) {
		throw new BadMethodCallException('Grapheme unsetting is not implemented');
	}

	public function chunkSplit($length = 76, $end = "\r\n") {
		if (!is_numeric($length) || $length < 1) {
			throw new InvalidArgumentException('Length must be a positive integer');
		}

		$length = (int) $length;
		$split = new self;
		$end = new self($end);
		$count = $this->count();

		for ($i = 0; $i < $count; $i += $length) {
			if ($i > 0) {
				$split->concat($end);
			}

			$split->concat($this->substr($i, $i + length));
		}

		return $split;
	}

	public function chunk_split($length = 76, $end = "\r\n") {
		return $this->chunkSplit($length, $end);
	}

	public function explode($delimiter, $limit = null) {
		$delimiter = new self($delimiter);

		if (is_null($limit)) {
			return self::objectifyArray(explode($delimiter->data, $this->data));
		}

		return self::objectifyArray(explode($delimiter->data, $this->data, $limit));
	}

	public function replace($search, $replace, $count = null) {
	}

	public function stripos($needle, $offset = 0) {
		$no = new self($needle);
		return grapheme_stripos($this->data, $no->data, $offset);
	}

	public function stristr($needle, $offset = 0) {
		$no = new self($needle);
		return grapheme_stristr($this->data, $no->data, $offset);
	}

	public function strlen() {
		return $this->count();
	}

	public function strpos($needle, $offset = 0) {
		$no = new self($needle);
		return grapheme_strpos($this->data, $no->data, $offset);
	}

	public function strrchr($needle) {
		$no = new self($needle);
		$pos = grapheme_strrpos($this->data, $no->data);

		if ($pos !== false) {
			return $this->substr($pos);
		}

		return false;
	}

	public function strrichr($needle) {
		$no = new self($needle);
		$pos = grapheme_strripos($this->data, $no->data);

		if ($pos !== false) {
			return $this->substr($pos);
		}

		return false;
	}

	public function strripos($needle, $offset = 0) {
		$no = new self($needle);
		return grapheme_strripos($this->data, $no->data, $offset);
	}

	public function strrpos($needle, $offset = 0) {
		$no = new self($needle);
		return grapheme_strrpos($this->data, $no->data, $offset);
	}

	public function strstr($needle, $offset = 0) {
		$no = new self($needle);
		return grapheme_strstr($this->data, $no->data, $offset);
	}

	public function strtolower() {
		return new self(mb_strtolower($this->data, 'UTF-8'));
	}

	public function strtoupper() {
		return new self(mb_strtoupper($this->data, 'UTF-8'));
	}

	public function substr($start, $length = null) {
		if (is_null($length)) {
			return new self(grapheme_substr($this->data, $start));
		}

		return new self(grapheme_substr($this->data, $start, $length));
	}

	public function set($str, $charset = 'UTF-8') {
		if ($str instanceof self) {
			$this->data = $str;
			return;
		}

		$str = (string) $str;

		if (!is_string($charset)) {
			throw new InvalidArgumentException('Character set must be a string');
		}

		$this->data = mb_convert_encoding($str, 'UTF-8', $charset);
		$this->normalise();
	}

	static public function chr($codepoint) {
		if (!is_numeric($codepoint)) {
			throw new InvalidArgumentException('Invalid codepoint');
		}

		$codepoint = (int) $codepoint;
		
		if ($codepoint < 0 || $codepoint > 0x10ffff) {
			throw new OutOfBoundsException('Codepoint out of the valid range');
		}

		return new self(mb_decode_numericentity("&#$codepoint;", array(0x0, 0x10ffff, 0x0, 0x10ffff), 'UTF-8'));
	}

	static public function implode($glue, array $pieces) {
		$glue = new self($glue);
		return new self(implode($glue->data, $pieces));
	}

	static public function join($glue, array $pieces) {
		return self::implode($glue, $pieces);
	}

	static public function GET($key) {
		return new self($_GET[$key]);
	}

	static public function POST($key) {
		return new self($_POST[$key], self::postCharset());
	}

	static public function RAW_POST_DATA() {
		return new self(file_get_contents('php://input'), self::postCharset());
	}

	private function checkOffset($offset) {
		if (!is_numeric($offset) || $offset < 0) {
			throw new InvalidArgumentException('Offset must be a non-negative integer');
		}

		if ($offset >= $this->count()) {
			throw new OutOfRangeException('Offset is beyond the length of the string');
		}

		return (int) $offset;

	}

	private function normalise() {
		$this->data = Normalizer::normalize($this->data);
	}

	private static function objectifyArray(array $a) {
		return array_map(function ($s) { return new UString($s); }, $a);
	}

	private static function postCharset() {
		$charset = 'UTF-8';

		// Check for the POST Content-Type, if set.
		if (isset($_SERVER['HTTP_CONTENT_TYPE'])) {
			$matches = array();
			if (preg_match('/;\s*charset\s*=\s*["]?(.*)["]?\s*(;|$)/U', $_SERVER['HTTP_CONTENT_TYPE'], $matches)) {
				$charset = trim($matches[1]);
			}
		}

		return $charset;
	}
}

// vim: set cin ai ts=8 sw=8 noet:
