--TEST--
UnicodeString::encode()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("F\xf5o", 'ISO-8859-1');
var_dump($us->encode('ISO-8859-1'));
var_dump($us->encode('UTF-8'));
--EXPECT--
string(3) "F�o"
string(4) "Fõo"
