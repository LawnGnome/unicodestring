dnl $Id$
dnl config.m4 for extension unicodestring

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(unicodestring, for unicodestring support,
dnl Make sure that the comment is aligned:
dnl [  --with-unicodestring             Include unicodestring support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(unicodestring, whether to enable unicodestring support,
Make sure that the comment is aligned:
[  --enable-unicodestring           Enable unicodestring support])

if test "$PHP_UNICODESTRING" != "no"; then
  PHP_SETUP_ICU(UNICODESTRING_SHARED_LIBADD)
  PHP_SUBST(UNICODESTRING_SHARED_LIBADD)
  PHP_REQUIRE_CXX()
  PHP_NEW_EXTENSION(unicodestring, unicodestring.cpp ustring.cpp, $ext_shared)
fi
