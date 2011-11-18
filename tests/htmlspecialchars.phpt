--TEST--
UnicodeString::htmlspecialchars()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("Foo");
$spec = $us->htmlspecialchars();
var_dump($spec);

$us = new unicodestring\UString("Foo&\"'é");
$spec = $us->htmlspecialchars();
var_dump($spec);

$us = new unicodestring\UString("Foo&\"'é");
$spec = $us->htmlspecialchars(ENT_QUOTES);
var_dump($spec);
--EXPECTF--
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(3) "Foo"
  ["length"]=>
  int(3)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(17) "Foo&amp;&quot;'é"
  ["length"]=>
  int(16)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(22) "Foo&amp;&quot;&#039;é"
  ["length"]=>
  int(21)
}
