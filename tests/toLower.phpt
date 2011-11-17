--TEST--
UnicodeString::toLower()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("F\xf5o", 'ISO-8859-1');
$lower = $us->toLower();

var_dump((string) $us, (string) $lower);
--EXPECT--
string(4) "Fõo"
string(4) "fõo"
