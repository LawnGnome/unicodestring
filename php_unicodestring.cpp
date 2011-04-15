#include "php_unicodestring.h"

extern "C" {
#include "ext/spl/spl_exceptions.h"
#include "ext/standard/php_smart_str.h"
#include "Zend/zend_exceptions.h"
}

#include <cassert>
#include <cstring>
#include <string>
#include <unicode/normlzr.h>
#include <unicode/unistr.h>

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

static zend_class_entry *ustring_entry;
static zend_object_handlers ustring_object_handlers;

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_construct_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, string)
	ZEND_ARG_INFO(0, charset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_toString_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

static function_entry ustring_functions[] = {
	PHP_ME(UString, __construct, php_unicodestring_ustring_construct_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, __toString, php_unicodestring_ustring_toString_arginfo, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

struct ustring_obj {
	zend_object zo;
	UnicodeString *ustr;
};

inline static ustring_obj *getIntern(zval *obj TSRMLS_DC) {
	assert(obj);
	return (ustring_obj *) zend_object_store_get_object(obj TSRMLS_CC);
}

static void ustring_set(ustring_obj *intern, const char *str, int len, const char *charset TSRMLS_DC) {
	UnicodeString tmp(str, len, charset);

	if (tmp.isBogus()) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0 TSRMLS_CC, "Invalid input string for charset %s", charset);
	}

	UErrorCode err = U_ZERO_ERROR;
	Normalizer::normalize(tmp, UNORM_NFC, 0, *intern->ustr, err);

	if (U_FAILURE(err)) {
		php_error(E_ERROR, "Error normalising string: %s", u_errorName(err));
	}
}

extern "C" void php_unicodestring_ustring_object_free_storage(void *object TSRMLS_DC) {
	ustring_obj *intern = static_cast<ustring_obj *>(object);

	delete intern->ustr;
	zend_object_std_dtor(&intern->zo TSRMLS_CC);

	efree(intern);
}

extern "C" zend_object_value php_unicodestring_ustring_object_new(zend_class_entry *type TSRMLS_DC) {
	ustring_obj *intern;
	zend_object_value retval;
	zval *tmp;

	intern = (ustring_obj *) emalloc(sizeof(ustring_obj));
	intern->ustr = new UnicodeString;

	zend_object_std_init(&intern->zo, type TSRMLS_CC);
	zend_hash_copy(intern->zo.properties, &type->default_properties, (copy_ctor_func_t) zval_add_ref, &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(intern, NULL, php_unicodestring_ustring_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = &ustring_object_handlers;

	return retval;
}

extern "C" int php_unicodestring_ustring_cast_object(zval *src, zval *dst, int type TSRMLS_DC) {
	ustring_obj *intern = getIntern(src TSRMLS_CC);
	std::string dest;
	StringByteSink<std::string> sink(&dest);

	intern->ustr->toUTF8(sink);
	INIT_PZVAL(dst);
	ZVAL_STRINGL(dst, dest.data(), dest.length(), 1);

	convert_to_explicit_type(dst, type);
	
	return SUCCESS;
}

extern "C" int php_unicodestring_ustring_compare_objects(zval *a, zval *b TSRMLS_DC) {
	bool aIsObj = (Z_TYPE_P(a) == IS_OBJECT && instanceof_function(Z_OBJCE_P(a), ustring_entry TSRMLS_CC));
	bool bIsObj = (Z_TYPE_P(b) == IS_OBJECT && instanceof_function(Z_OBJCE_P(b), ustring_entry TSRMLS_CC));

	if (aIsObj && bIsObj) {
		ustring_obj *usa = getIntern(a);
		ustring_obj *usb = getIntern(b);

		return usa->ustr->compareCodePointOrder(*usb->ustr);
	}

	return 1;
}

extern "C" int php_unicodestring_ustring_count_elements(zval *obj, long *count TSRMLS_DC) {
	ustring_obj *intern = getIntern(obj TSRMLS_CC);
	*count = intern->ustr->countChar32();

	return SUCCESS;
}

PHP_MINIT_FUNCTION(unicodestring) {
	zend_class_entry ce;

	std::memcpy(&ustring_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	ustring_object_handlers.cast_object = php_unicodestring_ustring_cast_object;
	ustring_object_handlers.compare_objects = php_unicodestring_ustring_compare_objects;
	ustring_object_handlers.count_elements = php_unicodestring_ustring_count_elements;

	INIT_CLASS_ENTRY(ce, "unicodestring\\UString", ustring_functions);
	ce.create_object = php_unicodestring_ustring_object_new;
	ustring_entry = zend_register_internal_class(&ce TSRMLS_CC);

	return SUCCESS;
}

PHP_METHOD(UString, __construct) {
	char *string, *charset = NULL;
	int stringLen, charsetLen;
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &string, &stringLen, &charset, &charsetLen) == FAILURE) {
		RETURN_FALSE;
	}

	if (charset) {
		ustring_set(intern, string, stringLen, charset TSRMLS_CC);
	}
	else {
		ustring_set(intern, string, stringLen, "UTF-8" TSRMLS_CC);
	}
}

PHP_METHOD(UString, __toString) {
	php_unicodestring_ustring_cast_object(getThis(), return_value, IS_STRING TSRMLS_CC);
}

// vim: set ai cin noet ts=8 sw=8:
