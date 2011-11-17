--TEST--
UnicodeString::encode()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("F\xf5o", 'ISO-8859-1');
var_dump($us->encode('ISO-8859-1'));
var_dump($us->encode('UTF-8'));

try {
	$us->encode('Bad charset');
} catch (unicodestring\ConversionException $e) {
	echo "Caught conversion exception\n";
}
--EXPECT--
string(3) "Fõo"
string(4) "FÃµo"
Caught conversion exception
