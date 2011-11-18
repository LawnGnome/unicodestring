--TEST--
UnicodeString::html_entity_decode()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("Foo");
$spec = $us->html_entity_decode();
var_dump($spec, (string) $spec);

$us = new unicodestring\UString("Foo&amp;&quot;&#039;&eacute;");
$spec = $us->html_entity_decode();
var_dump($spec, (string) $spec);

$us = new unicodestring\UString("Foo&amp;&quot;&#039;&eacute;");
$spec = $us->html_entity_decode(ENT_QUOTES);
var_dump($spec, (string) $spec);
--EXPECTF--
object(unicodestring\UString)#%d (0) {
}
string(3) "Foo"
object(unicodestring\UString)#%d (0) {
}
string(13) "Foo&"&#039;é"
object(unicodestring\UString)#%d (0) {
}
string(8) "Foo&"'é"
