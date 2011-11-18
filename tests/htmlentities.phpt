--TEST--
UnicodeString::htmlentities()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("Foo");
$spec = $us->htmlentities();
var_dump($spec, (string) $spec);

$us = new unicodestring\UString("Foo&\"'é");
$spec = $us->htmlentities();
var_dump($spec, (string) $spec);

$us = new unicodestring\UString("Foo&\"'é");
$spec = $us->htmlentities(ENT_QUOTES);
var_dump($spec, (string) $spec);
--EXPECTF--
object(unicodestring\UString)#%d (0) {
}
string(3) "Foo"
object(unicodestring\UString)#%d (0) {
}
string(23) "Foo&amp;&quot;'&eacute;"
object(unicodestring\UString)#%d (0) {
}
string(28) "Foo&amp;&quot;&#039;&eacute;"
