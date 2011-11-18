--TEST--
UnicodeString::htmlspecialchars()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("Foo");
$spec = $us->htmlspecialchars();
var_dump($spec, (string) $spec);

$us = new unicodestring\UString("Foo&\"'é");
$spec = $us->htmlspecialchars();
var_dump($spec, (string) $spec);

$us = new unicodestring\UString("Foo&\"'é");
$spec = $us->htmlspecialchars(ENT_QUOTES);
var_dump($spec, (string) $spec);
--EXPECTF--
object(unicodestring\UString)#%d (0) {
}
string(3) "Foo"
object(unicodestring\UString)#%d (0) {
}
string(17) "Foo&amp;&quot;'é"
object(unicodestring\UString)#%d (0) {
}
string(22) "Foo&amp;&quot;&#039;é"
