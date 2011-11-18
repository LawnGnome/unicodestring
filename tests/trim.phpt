--TEST--
UnicodeString::trim()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString(" Foo ", 'ISO-8859-1');
var_dump($us->trim());
var_dump($us->trim(' '));
var_dump($us->trim(new unicodestring\UString(' o')));
var_dump($us->trim('g'));

$us = new unicodestring\UString();
var_dump($us->trim());
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
  string(5) " Foo "
  ["length"]=>
  int(5)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(0) ""
  ["length"]=>
  int(0)
}
