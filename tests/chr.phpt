--TEST--
UnicodeString::chr()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$chr = unicodestring\UString::chr(32);
var_dump($chr);

$chr = unicodestring\UString::chr(0x414);
var_dump($chr);

$chr = unicodestring\UString::chr(0x1d120);
var_dump($chr);
--EXPECTF--
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(1) " "
  ["length"]=>
  int(1)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(2) "Д"
  ["length"]=>
  int(1)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(4) "𝄠"
  ["length"]=>
  int(1)
}
