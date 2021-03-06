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
	extern zend_class_entry *unicodestring_ConversionException;
	extern zend_class_entry *unicodestring_InternalException;
	extern zend_class_entry *unicodestring_InvalidInputException;
	extern zend_class_entry *unicodestring_OutOfRangeException;

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
	PHP_METHOD(UString, chr);
	PHP_METHOD(UString, encode);
	PHP_METHOD(UString, explode);
	PHP_METHOD(UString, htmlentities);
	PHP_METHOD(UString, htmlspecialchars);
	PHP_METHOD(UString, html_entity_decode);
	PHP_METHOD(UString, htmlspecialchars_decode);
	PHP_METHOD(UString, length);
	PHP_METHOD(UString, ltrim);
	PHP_METHOD(UString, offsetExists);
	PHP_METHOD(UString, offsetGet);
	PHP_METHOD(UString, offsetSet);
	PHP_METHOD(UString, offsetUnset);
	PHP_METHOD(UString, replace);
	PHP_METHOD(UString, rtrim);
	PHP_METHOD(UString, toLower);
	PHP_METHOD(UString, toUpper);
	PHP_METHOD(UString, trim);

	void register_unicodestring_ustring(TSRMLS_D);
}

#endif

// vim: set ai cin noet ts=8 sw=8:
