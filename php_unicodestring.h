#ifndef PHP_UNICODESTRING_H
#define PHP_UNICODESTRING_H

#define PHP_UNICODESTRING_EXTNAME "unicodestring"
#define PHP_UNICODESTRING_VERSION "0.0.0"

extern "C" {
#	ifdef HAVE_CONFIG_H
#		include "config.h"
#	endif

#	include "php.h"

	// exceptions.cpp
	extern zend_class_entry *unicodestring_InvalidInputException;

	PHP_METHOD(InvalidInputException, __construct);

	void register_unicodestring_exceptions(TSRMLS_D);

	// unicodestring.cpp
	extern zend_module_entry unicodestring_module_entry;
#	define phpext_unicodestring_ptr &unicodestring_module_entry

	PHP_MINIT_FUNCTION(unicodestring);

	// ustring.cpp
	extern zend_class_entry *unicodestring_UString;

	PHP_METHOD(UString, __construct);
	PHP_METHOD(UString, __toString);

	void register_unicodestring_ustring(TSRMLS_D);
}

#endif

// vim: set ai cin noet ts=8 sw=8:
