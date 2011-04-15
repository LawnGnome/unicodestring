#include "php_unicodestring.h"

zend_module_entry unicodestring_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_UNICODESTRING_EXTNAME,
	NULL, // functions
	PHP_MINIT(unicodestring), // MINIT
	NULL, // MSHUTDOWN
	NULL, // RINIT
	NULL, // RSHUTDOWN
	NULL, // MINFO
	PHP_UNICODESTRING_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_UNICODESTRING
ZEND_GET_MODULE(unicodestring)
#endif

PHP_MINIT_FUNCTION(unicodestring) {
	register_ustring_class_entry();

	return SUCCESS;
}

// vim: set ai cin noet ts=8 sw=8:
