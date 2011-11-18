--TEST--
UnicodeString::rtrim()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("Foo ", 'ISO-8859-1');
var_dump($us->rtrim());
var_dump($us->rtrim(' '));
var_dump($us->rtrim(new unicodestring\UString(' o')));
var_dump($us->rtrim('g'));

$us = new unicodestring\UString();
var_dump($us->rtrim());
--EXPECTF--
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(3) "Foo"
  ["length"]=>
  int(3)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(3) "Foo"
  ["length"]=>
  int(3)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(1) "F"
  ["length"]=>
  int(1)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(4) "Foo "
  ["length"]=>
  int(4)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(0) ""
  ["length"]=>
  int(0)
}
