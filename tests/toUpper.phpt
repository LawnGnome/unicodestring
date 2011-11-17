--TEST--
UnicodeString::toUpper()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("F\xf5o", 'ISO-8859-1');
$upper = $us->toUpper();

var_dump((string) $us, (string) $upper);
--EXPECT--
string(4) "Fõo"
string(4) "FÕO"
