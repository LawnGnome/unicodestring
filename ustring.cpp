#include "php_unicodestring.h"

#include <cassert>
#include <cstring>
#include <string>

#include "lib/ustring.h"

extern "C" {
#	include "Zend/zend_exceptions.h"
#	include "Zend/zend_interfaces.h"

	// Callback function prototypes.
	int php_unicodestring_ustring_cast_object(zval *src, zval *dst, int type TSRMLS_DC);
	zend_object_value php_unicodestring_ustring_clone_object(zval *obj TSRMLS_DC);
	int php_unicodestring_ustring_compare_objects(zval *a, zval *b TSRMLS_DC);
	int php_unicodestring_ustring_count_elements(zval *obj, long *count TSRMLS_DC);
	void php_unicodestring_ustring_object_free_storage(void *object TSRMLS_DC);
	zend_object_value php_unicodestring_ustring_object_new(zend_class_entry *type TSRMLS_DC);
}

// The internal object we're using that encapsulates the UnicodeString.
struct ustring_obj {
	zend_object zo;
	UString *ustr;
};

// Helper function to get the internal object from a zval.
inline static ustring_obj *getIntern(zval *obj TSRMLS_DC) {
	assert(obj);
	assert(Z_TYPE_P(obj) == IS_OBJECT);

	return (ustring_obj *) zend_object_store_get_object(obj TSRMLS_CC);
}

// Helper function to set the UnicodeString in a ustring_obj.
static void ustring_set(ustring_obj *intern, const char *str, int len, const char *charset TSRMLS_DC) {
	try {
		intern->ustr = new UString(str, len, charset);
	} catch (MalformedInput e) {
		zval *exception, zvalInput, zvalCharset;

		MAKE_STD_ZVAL(exception);
		object_init_ex(exception, unicodestring_InvalidInputException);

		INIT_ZVAL(zvalInput);
		ZVAL_STRINGL(&zvalInput, str, len, 0);

		INIT_ZVAL(zvalCharset);
		ZVAL_STRING(&zvalCharset, charset, 0);

		zend_call_method_with_2_params(&exception, unicodestring_InvalidInputException, &unicodestring_InvalidInputException->constructor, "__construct", NULL, &zvalInput, &zvalCharset);

		zend_throw_exception_object(exception TSRMLS_CC);

		zend_throw_exception_ex(unicodestring_InvalidInputException, 0 TSRMLS_CC, "%s", e.what());
	} catch (NormalisationError e) {
		php_error(E_ERROR, e.what());
	}
}

// Class entry and object handlers.
zend_class_entry *unicodestring_UString;
static zend_object_handlers ustring_object_handlers;

// Arginfo and method list definitions.
ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_construct_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, string)
	ZEND_ARG_INFO(0, charset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_toString_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_offsetExists_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_offsetGet_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_offsetSet_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_offsetUnset_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_toLower_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_toUpper_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry ustring_functions[] = {
	PHP_ME(UString, __construct, php_unicodestring_ustring_construct_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, __toString, php_unicodestring_ustring_toString_arginfo, ZEND_ACC_PUBLIC)

	PHP_ME(UString, offsetExists, php_unicodestring_ustring_offsetExists_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, offsetGet, php_unicodestring_ustring_offsetGet_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, offsetSet, php_unicodestring_ustring_offsetSet_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, offsetUnset, php_unicodestring_ustring_offsetUnset_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, toLower, php_unicodestring_ustring_toLower_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, toUpper, php_unicodestring_ustring_toUpper_arginfo, ZEND_ACC_PUBLIC)

	PHP_MALIAS(UString, set, __construct, php_unicodestring_ustring_construct_arginfo, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

// Callbacks and handlers.
void php_unicodestring_ustring_object_free_storage(void *object TSRMLS_DC) {
	ustring_obj *intern = static_cast<ustring_obj *>(object);

	delete intern->ustr;
	zend_object_std_dtor(&intern->zo TSRMLS_CC);

	efree(intern);
}

zend_object_value php_unicodestring_ustring_object_new(zend_class_entry *type TSRMLS_DC) {
	ustring_obj *intern;
	zend_object_value retval;
	zval *tmp;

	intern = (ustring_obj *) emalloc(sizeof(ustring_obj));
	intern->ustr = new UString;

	zend_object_std_init(&intern->zo, type TSRMLS_CC);
#if PHP_API_VERSION > 20090626
	object_properties_init(&intern->zo, type);
#else
	zend_hash_copy(intern->zo.properties, &type->default_properties, (copy_ctor_func_t) zval_add_ref, &tmp, sizeof(zval *));
#endif

	retval.handle = zend_objects_store_put(intern, NULL, php_unicodestring_ustring_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = &ustring_object_handlers;

	return retval;
}

int php_unicodestring_ustring_cast_object(zval *src, zval *dst, int type TSRMLS_DC) {
	ustring_obj *intern = getIntern(src TSRMLS_CC);
	std::string utf8;

	try {
		utf8 = intern->ustr->toUTF8();
	} catch (ConversionError e) {
		zend_throw_exception_ex(unicodestring_ConversionException, 0 TSRMLS_CC, "%s", e.what());
	}

	INIT_PZVAL(dst);
	ZVAL_STRINGL(dst, utf8.c_str(), utf8.size(), 1);

	convert_to_explicit_type(dst, type);
	
	return SUCCESS;
}

zend_object_value php_unicodestring_ustring_clone_object(zval *obj TSRMLS_DC) {
	assert(Z_TYPE_P(obj) == IS_OBJECT);
	assert(instanceof_function(Z_OBJCE_P(obj), unicodestring_UString TSRMLS_CC));

	ustring_obj *intern = getIntern(obj);
	zend_object_value clone = php_unicodestring_ustring_object_new(unicodestring_UString TSRMLS_CC);
	ustring_obj *cloned = (ustring_obj *) zend_object_store_get_object_by_handle(clone.handle);

	cloned->ustr = new UString(*intern->ustr);

	return clone;
}

int php_unicodestring_ustring_compare_objects(zval *a, zval *b TSRMLS_DC) {
	bool aIsObj = (Z_TYPE_P(a) == IS_OBJECT && instanceof_function(Z_OBJCE_P(a), unicodestring_UString TSRMLS_CC));
	bool bIsObj = (Z_TYPE_P(b) == IS_OBJECT && instanceof_function(Z_OBJCE_P(b), unicodestring_UString TSRMLS_CC));

	if (aIsObj && bIsObj) {
		ustring_obj *usa = getIntern(a);
		ustring_obj *usb = getIntern(b);

		return usa->ustr->compare(*usb->ustr);
	}

	return 1;
}

int php_unicodestring_ustring_count_elements(zval *obj, long *count TSRMLS_DC) {
	ustring_obj *intern = getIntern(obj TSRMLS_CC);
	*count = intern->ustr->length();

	return SUCCESS;
}

// Registration function.
void register_unicodestring_ustring(TSRMLS_C) {
	zend_class_entry ce;

	std::memcpy(&ustring_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	ustring_object_handlers.cast_object = php_unicodestring_ustring_cast_object;
	ustring_object_handlers.clone_obj = php_unicodestring_ustring_clone_object;
	ustring_object_handlers.compare_objects = php_unicodestring_ustring_compare_objects;
	ustring_object_handlers.count_elements = php_unicodestring_ustring_count_elements;

	INIT_CLASS_ENTRY(ce, "unicodestring\\UString", ustring_functions);
	ce.create_object = php_unicodestring_ustring_object_new;
	unicodestring_UString = zend_register_internal_class(&ce TSRMLS_CC);

	zend_class_implements(unicodestring_UString TSRMLS_CC, 1, zend_ce_arrayaccess);
}

// Methods.
PHP_METHOD(UString, __construct) {
	char *string = NULL, *charset = NULL;
	int stringLen = 0, charsetLen = 0;
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ss", &string, &stringLen, &charset, &charsetLen) == FAILURE) {
		RETURN_FALSE;
	}

	if (string) {
		if (charset) {
			ustring_set(intern, string, stringLen, charset TSRMLS_CC);
		}
		else {
			ustring_set(intern, string, stringLen, "UTF-8" TSRMLS_CC);
		}
	}
	else {
		ustring_set(intern, "", 0, "UTF-8" TSRMLS_CC);
	}
}

PHP_METHOD(UString, __toString) {
	php_unicodestring_ustring_cast_object(getThis(), return_value, IS_STRING TSRMLS_CC);
}

PHP_METHOD(UString, offsetExists) {
	long index;
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE) {
		RETURN_FALSE;
	}

	RETURN_BOOL(index >= 0 && index < intern->ustr->length());
}

PHP_METHOD(UString, offsetGet) {
	long index;
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE) {
		RETURN_FALSE;
	}

	try {
		UChar32 ch = intern->ustr->charAt(index);
		zval blank;

		INIT_ZVAL(blank);
		ZVAL_STRING(&blank, "", 0);

		Z_TYPE_P(return_value) = IS_OBJECT;
		object_init_ex(return_value, unicodestring_UString TSRMLS_CC);

		zend_call_method_with_1_params(&return_value, unicodestring_UString, &unicodestring_UString->constructor, "__construct", NULL, &blank);
		ustring_obj *return_value_intern = getIntern(return_value);

		return_value_intern->ustr->set(&ch, 1);
	} catch (std::out_of_range e) {
		zend_throw_exception_ex(unicodestring_OutOfRangeException, 0 TSRMLS_CC, "Index %d is out of range", index);
	} catch (MalformedInput e) {
		zend_throw_exception_ex(unicodestring_InternalException, 0 TSRMLS_CC, "%s", e.what());
	} catch (NormalisationError e) {
		zend_throw_exception_ex(unicodestring_InternalException, 0 TSRMLS_CC, "%s", e.what());
	}
}

PHP_METHOD(UString, offsetSet) {
	long index;
	zval *obj = getThis(), *value;
	ustring_obj *intern = getIntern(obj TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &index, &value) == FAILURE) {
		RETURN_FALSE;
	}

	if (index >= 0 && index < intern->ustr->length()) {
		if (Z_TYPE_P(value) == IS_OBJECT && instanceof_function(Z_OBJCE_P(value), unicodestring_UString TSRMLS_CC)) {
			ustring_obj *value_intern = getIntern(value TSRMLS_CC);
			intern->ustr->setCharAt(index, *(value_intern->ustr));
		}
		else {
			convert_to_string(value);
			UString us(Z_STRVAL_P(value), Z_STRLEN_P(value), "UTF-8");

			intern->ustr->setCharAt(index, us);
		}
	}
	else {
		zend_throw_exception_ex(unicodestring_OutOfRangeException, 0 TSRMLS_CC, "Index %d is out of range", index);
	}
}

PHP_METHOD(UString, offsetUnset) {
	long index;
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE) {
		RETURN_FALSE;
	}

	if (index >= 0 && index < intern->ustr->length()) {
		intern->ustr->remove(index);
	}
	else {
		zend_throw_exception_ex(unicodestring_OutOfRangeException, 0 TSRMLS_CC, "Index %d is out of range", index);
	}
}

PHP_METHOD(UString, toLower) {
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);

	Z_TYPE_P(return_value) = IS_OBJECT;
	object_init_ex(return_value, unicodestring_UString TSRMLS_CC);

	zend_call_method_with_0_params(&return_value, unicodestring_UString, &unicodestring_UString->constructor, "__construct", NULL);
	*(getIntern(return_value)->ustr) = intern->ustr->toLower();
}

PHP_METHOD(UString, toUpper) {
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);

	Z_TYPE_P(return_value) = IS_OBJECT;
	object_init_ex(return_value, unicodestring_UString TSRMLS_CC);

	zend_call_method_with_0_params(&return_value, unicodestring_UString, &unicodestring_UString->constructor, "__construct", NULL);
	*(getIntern(return_value)->ustr) = intern->ustr->toUpper();
}

// vim: set ai cin noet ts=8 sw=8:
