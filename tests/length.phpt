--TEST--
UnicodeString::length()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("F\xf5o", 'ISO-8859-1');
var_dump($us->length());
var_dump($us->strlen());
--EXPECT--
int(3)
int(3)
