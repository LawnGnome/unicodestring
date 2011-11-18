--TEST--
UnicodeString::htmlspecialchars_decode()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("Foo");
$spec = $us->htmlspecialchars_decode();
var_dump($spec);

$us = new unicodestring\UString("Foo&amp;&quot;&#039;&eacute;");
$spec = $us->htmlspecialchars_decode();
var_dump($spec);

$us = new unicodestring\UString("Foo&amp;&quot;&#039;&eacute;");
$spec = $us->htmlspecialchars_decode(ENT_QUOTES);
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
  string(19) "Foo&"&#039;&eacute;"
  ["length"]=>
  int(19)
}
object(unicodestring\UString)#%d (2) {
  ["string"]=>
  string(14) "Foo&"'&eacute;"
  ["length"]=>
  int(14)
}
