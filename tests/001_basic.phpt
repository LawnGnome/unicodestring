--TEST--
Basic UnicodeString loading
--FILE--
<?php
var_dump(extension_loaded('unicodestring'));

$us = new unicodestring\UString;
var_dump($us, count($us), (int) $us, (boolean) $us);

$us = new unicodestring\UString('foo');
var_dump($us, count($us), (string) $us);

$us = new unicodestring\UString('1.1');
var_dump((integer) $us, (float) $us);
--EXPECTF--
bool(true)
object(unicodestring\UString)#%d (0) {
}
int(0)
int(0)
bool(false)
object(unicodestring\UString)#%d (0) {
}
int(3)
string(3) "foo"
int(1)
float(1.1)
