#ifndef PHP_UNICODESTRING_H
#define PHP_UNICODESTRING_H

#define PHP_UNICODESTRING_EXTNAME "unicodestring"
#define PHP_UNICODESTRING_VERSION "0.0.0"

extern "C" {
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

	extern zend_module_entry unicodestring_module_entry;
#define phpext_unicodestring_ptr &unicodestring_module_entry

	PHP_MINIT_FUNCTION(unicodestring);

	PHP_METHOD(UString, __construct);
	PHP_METHOD(UString, __toString);
}

struct ustring_obj;

#endif

// vim: set ai cin noet ts=8 sw=8:
