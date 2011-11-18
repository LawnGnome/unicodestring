--TEST--
Basic UnicodeString casting behaviour
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString;
var_dump($us, count($us), (int) $us, (boolean) $us);

$us = new unicodestring\UString('foo');
var_dump($us, count($us), (string) $us);

$us = new unicodestring\UString('1.1');
var_dump((integer) $us, (float) $us);
--EXPECTF--
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(0) ""
  ["length"]=>
  int(0)
}
int(0)
int(0)
bool(false)
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(3) "foo"
  ["length"]=>
  int(3)
}
int(3)
string(3) "foo"
int(1)
float(1.1)
