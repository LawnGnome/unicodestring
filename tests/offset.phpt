--TEST--
UnicodeString offset support
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("F\xf5o", 'ISO-8859-1');
var_dump(isset($us[1]));
var_dump((string) $us[1]);

$us[1] = 'o';
var_dump((string) $us);

unset($us[1]);
var_dump((string) $us);

// Expected failures.
var_dump(isset($us[2]));

try {
	var_dump((string) $us[2]);
} catch (unicodestring\OutOfRangeException $e) {
	echo "Caught exception on offsetGet\n";
}

try {
	$us[2] = 'o';
} catch (unicodestring\OutOfRangeException $e) {
	echo "Caught exception on offsetSet\n";
}

try {
	unset($us[2]);
} catch (unicodestring\OutOfRangeException $e) {
	echo "Caught exception on offsetUnset\n";
}
--EXPECT--
bool(true)
string(2) "õ"
string(3) "Foo"
string(2) "Fo"
bool(false)
Caught exception on offsetGet
Caught exception on offsetSet
Caught exception on offsetUnset
