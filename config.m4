PHP_ARG_ENABLE(unicodestring, whether to enable unicodestring support,
Make sure that the comment is aligned:
[  --enable-unicodestring           Enable unicodestring support])

if test "$PHP_UNICODESTRING" != "no"; then
  PHP_SETUP_ICU(UNICODESTRING_SHARED_LIBADD)
  PHP_SUBST(UNICODESTRING_SHARED_LIBADD)
  PHP_REQUIRE_CXX()
  PHP_NEW_EXTENSION(unicodestring, exceptions.cpp unicodestring.cpp ustring.cpp, $ext_shared)
fi
