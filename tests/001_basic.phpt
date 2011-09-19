--TEST--
Basic UnicodeString loading
--FILE--
<?php
var_dump(extension_loaded('unicodestring'));

$us = new unicodestring\UString;
var_dump($us, count($us));

$us = new unicodestring\UString('foo');
var_dump($us, count($us), (string) $us);
--EXPECTF--
bool(true)
object(unicodestring\UString)#%d (0) {
}
int(0)
object(unicodestring\UString)#%d (0) {
}
int(3)
string(3) "foo"
