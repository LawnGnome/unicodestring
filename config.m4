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
  dnl Write more examples of tests here...

  dnl # --with-unicodestring -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/unicodestring.h"  # you most likely want to change this
  dnl if test -r $PHP_UNICODESTRING/$SEARCH_FOR; then # path given as parameter
  dnl   UNICODESTRING_DIR=$PHP_UNICODESTRING
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for unicodestring files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       UNICODESTRING_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$UNICODESTRING_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the unicodestring distribution])
  dnl fi

  dnl # --with-unicodestring -> add include path
  dnl PHP_ADD_INCLUDE($UNICODESTRING_DIR/include)

  dnl # --with-unicodestring -> check for lib and symbol presence
  dnl LIBNAME=unicodestring # you may want to change this
  dnl LIBSYMBOL=unicodestring # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $UNICODESTRING_DIR/lib, UNICODESTRING_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_UNICODESTRINGLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong unicodestring lib version or lib not found])
  dnl ],[
  dnl   -L$UNICODESTRING_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(UNICODESTRING_SHARED_LIBADD)

  PHP_SETUP_ICU(UNICODESTRING_SHARED_LIBADD)
  PHP_SUBST(UNICODESTRING_SHARED_LIBADD)
  PHP_REQUIRE_CXX()
  PHP_NEW_EXTENSION(unicodestring, php_unicodestring.cpp, $ext_shared)
fi
