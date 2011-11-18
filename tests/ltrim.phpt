--TEST--
UnicodeString::ltrim()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString(" Foo", 'ISO-8859-1');
var_dump($us->ltrim());
var_dump($us->ltrim(' '));
var_dump($us->ltrim(new unicodestring\UString(' F')));
var_dump($us->ltrim('g'));

$us = new unicodestring\UString();
var_dump($us->ltrim());
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
  string(2) "oo"
  ["length"]=>
  int(2)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(4) " Foo"
  ["length"]=>
  int(4)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(0) ""
  ["length"]=>
  int(0)
}
