--TEST--
UnicodeString::htmlspecialchars_decode()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("Foo");
$spec = $us->htmlspecialchars_decode();
var_dump($spec, (string) $spec);

$us = new unicodestring\UString("Foo&amp;&quot;&#039;&eacute;");
$spec = $us->htmlspecialchars_decode();
var_dump($spec, (string) $spec);

$us = new unicodestring\UString("Foo&amp;&quot;&#039;&eacute;");
$spec = $us->htmlspecialchars_decode(ENT_QUOTES);
var_dump($spec, (string) $spec);
--EXPECTF--
object(unicodestring\UString)#%d (0) {
}
string(3) "Foo"
object(unicodestring\UString)#%d (0) {
}
string(19) "Foo&"&#039;&eacute;"
object(unicodestring\UString)#%d (0) {
}
string(14) "Foo&"'&eacute;"
