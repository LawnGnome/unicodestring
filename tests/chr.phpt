--TEST--
UnicodeString::chr()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$chr = unicodestring\UString::chr(32);
var_dump($chr, (string) $chr);

$chr = unicodestring\UString::chr(0x414);
var_dump($chr, (string) $chr);

$chr = unicodestring\UString::chr(0x1d120);
var_dump($chr, (string) $chr);
--EXPECTF--
object(unicodestring\UString)#%d (0) {
}
string(1) " "
object(unicodestring\UString)#%d (0) {
}
string(2) "Д"
object(unicodestring\UString)#%d (0) {
}
string(4) "𝄠"
