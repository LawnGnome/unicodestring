--TEST--
UnicodeString::htmlentities()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("Foo");
$spec = $us->htmlentities();
var_dump($spec);

$us = new unicodestring\UString("Foo&\"'é");
$spec = $us->htmlentities();
var_dump($spec);

$us = new unicodestring\UString("Foo&\"'é");
$spec = $us->htmlentities(ENT_QUOTES);
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
  string(23) "Foo&amp;&quot;'&eacute;"
  ["length"]=>
  int(23)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(28) "Foo&amp;&quot;&#039;&eacute;"
  ["length"]=>
  int(28)
}
