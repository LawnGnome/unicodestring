--TEST--
UnicodeString::html_entity_decode()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("Foo");
$spec = $us->html_entity_decode();
var_dump($spec);

$us = new unicodestring\UString("Foo&amp;&quot;&#039;&eacute;");
$spec = $us->html_entity_decode();
var_dump($spec);

$us = new unicodestring\UString("Foo&amp;&quot;&#039;&eacute;");
$spec = $us->html_entity_decode(ENT_QUOTES);
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
  string(13) "Foo&"&#039;é"
  ["length"]=>
  int(12)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(8) "Foo&"'é"
  ["length"]=>
  int(7)
}
