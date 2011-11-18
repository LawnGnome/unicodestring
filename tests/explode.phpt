--TEST--
UnicodeString::explode()
--SKIPIF--
<?php if (!extension_loaded('unicodestring')) die('skip unicodestring must be loaded');
--FILE--
<?php
$us = new unicodestring\UString("foo a quux a djf", 'ISO-8859-1');
var_dump($us->explode(' a '));
var_dump($us->explode(' a ', 1));
var_dump($us->explode(' a ', 2));
var_dump($us->explode(' a ', 3));
var_dump($us->explode(' a ', 4));
var_dump($us->explode(' a ', 0));
var_dump($us->explode(' a ', -1));
var_dump($us->explode(' a ', -2));
var_dump($us->explode(''));
var_dump($us->explode(new unicodestring\UString(' a ')));
var_dump($us->explode('not in this string'));
--EXPECTF--
array(3) {
  [0]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(3) "foo"
    ["length"]=>
    int(3)
  }
  [1]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(4) "quux"
    ["length"]=>
    int(4)
  }
  [2]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(3) "djf"
    ["length"]=>
    int(3)
  }
}
array(1) {
  [0]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(16) "foo a quux a djf"
    ["length"]=>
    int(16)
  }
}
array(2) {
  [0]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(3) "foo"
    ["length"]=>
    int(3)
  }
  [1]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(10) "quux a djf"
    ["length"]=>
    int(10)
  }
}
array(3) {
  [0]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(3) "foo"
    ["length"]=>
    int(3)
  }
  [1]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(4) "quux"
    ["length"]=>
    int(4)
  }
  [2]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(3) "djf"
    ["length"]=>
    int(3)
  }
}
array(3) {
  [0]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(3) "foo"
    ["length"]=>
    int(3)
  }
  [1]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(4) "quux"
    ["length"]=>
    int(4)
  }
  [2]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(3) "djf"
    ["length"]=>
    int(3)
  }
}
array(1) {
  [0]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(16) "foo a quux a djf"
    ["length"]=>
    int(16)
  }
}
array(2) {
  [0]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(3) "foo"
    ["length"]=>
    int(3)
  }
  [1]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(4) "quux"
    ["length"]=>
    int(4)
  }
}
array(1) {
  [0]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(3) "foo"
    ["length"]=>
    int(3)
  }
}

Warning: Empty delimiter in /Users/adam/Trees/unicodestring/tests/explode.php on line 11
bool(false)
array(3) {
  [0]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(3) "foo"
    ["length"]=>
    int(3)
  }
  [1]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(4) "quux"
    ["length"]=>
    int(4)
  }
  [2]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(3) "djf"
    ["length"]=>
    int(3)
  }
}
array(1) {
  [0]=>
  object(unicodestring\UString)#%d (2) {
    ["string"]=>
    string(16) "foo a quux a djf"
    ["length"]=>
    int(16)
  }
}
