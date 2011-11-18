#include "php_unicodestring.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>

#include "lib/ustring.h"

extern "C" {
#	include "ext/standard/html.h"
#	include "Zend/zend_exceptions.h"
#	include "Zend/zend_interfaces.h"

	// Callback function prototypes.
	int php_unicodestring_ustring_cast_object(zval *src, zval *dst, int type TSRMLS_DC);
	zend_object_value php_unicodestring_ustring_clone_object(zval *obj TSRMLS_DC);
	int php_unicodestring_ustring_compare_objects(zval *a, zval *b TSRMLS_DC);
	int php_unicodestring_ustring_count_elements(zval *obj, long *count TSRMLS_DC);
	HashTable *php_unicodestring_ustring_get_properties(zval *obj TSRMLS_DC);
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

static void ustring_create_object(zval *zv) {
	Z_TYPE_P(zv) = IS_OBJECT;
	object_init_ex(zv, unicodestring_UString TSRMLS_CC);

	zend_call_method_with_0_params(&zv, unicodestring_UString, &unicodestring_UString->constructor, "__construct", NULL);
}

static void ustring_create_object(zval *zv, const char *data, size_t length, const char *charset) {
	ustring_create_object(zv);
	getIntern(zv)->ustr->set(data, length, charset);
}

static void ustring_create_object(zval *zv, const char *data, size_t length) {
	ustring_create_object(zv, data, length, "UTF-8");
}

static void ustring_create_object(zval *zv, const UString &us) {
	ustring_create_object(zv);
	*(getIntern(zv)->ustr) = us;
}

static bool ustring_parse_stringy_zval(zval *zv, UString &us TSRMLS_DC) {
	switch (Z_TYPE_P(zv)) {
		case IS_LONG:
		case IS_DOUBLE:
		case IS_BOOL:
			convert_to_string(zv);
			
		case IS_STRING:
			us.set(Z_STRVAL_P(zv), Z_STRLEN_P(zv), "UTF-8");
			return true;

		case IS_OBJECT:
			if (instanceof_function(Z_OBJCE_P(zv), unicodestring_UString TSRMLS_CC)) {
				us = *(getIntern(zv)->ustr);
				return true;
			}
	}

	return false;
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
	} catch (NormalisationError e) {
		php_error(E_ERROR, "%s", e.what());
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

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_chr_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, codepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_encode_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, charset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_explode_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, delimiter)
	ZEND_ARG_INFO(0, limit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_htmlentities_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_htmlspecialchars_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_html_entity_decode_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, quote_style)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_htmlspecialchars_decode_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, quote_style)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_unicodestring_ustring_length_arginfo, 0, 0, 0)
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

	PHP_ME(UString, chr, php_unicodestring_ustring_chr_arginfo, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(UString, encode, php_unicodestring_ustring_encode_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, explode, php_unicodestring_ustring_explode_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, htmlentities, php_unicodestring_ustring_htmlentities_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, htmlspecialchars, php_unicodestring_ustring_htmlspecialchars_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, html_entity_decode, php_unicodestring_ustring_html_entity_decode_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, htmlspecialchars_decode, php_unicodestring_ustring_htmlspecialchars_decode_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, length, php_unicodestring_ustring_length_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, offsetExists, php_unicodestring_ustring_offsetExists_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, offsetGet, php_unicodestring_ustring_offsetGet_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, offsetSet, php_unicodestring_ustring_offsetSet_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, offsetUnset, php_unicodestring_ustring_offsetUnset_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, toLower, php_unicodestring_ustring_toLower_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(UString, toUpper, php_unicodestring_ustring_toUpper_arginfo, ZEND_ACC_PUBLIC)

	PHP_MALIAS(UString, set, __construct, php_unicodestring_ustring_construct_arginfo, ZEND_ACC_PUBLIC)
	PHP_MALIAS(UString, strlen, length, php_unicodestring_ustring_length_arginfo, ZEND_ACC_PUBLIC)
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
		char format[] = "%s";
		zend_throw_exception_ex(unicodestring_ConversionException, 0 TSRMLS_CC, format, e.what());
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

HashTable *php_unicodestring_ustring_get_properties(zval *obj TSRMLS_DC) {
	ustring_obj *intern = getIntern(obj TSRMLS_CC);
	HashTable *props;
	zval *temp;

	props = zend_std_get_properties(obj TSRMLS_CC);

	std::string utf8(intern->ustr->toUTF8());
	MAKE_STD_ZVAL(temp);
	ZVAL_STRINGL(temp, utf8.c_str(), utf8.size(), 1);
	zend_hash_update(props, "string", sizeof("string"), &temp, sizeof(zval), NULL);

	MAKE_STD_ZVAL(temp);
	ZVAL_LONG(temp, intern->ustr->length());
	zend_hash_update(props, "length", sizeof("length"), &temp, sizeof(zval), NULL);

	return props;
}

// Registration function.
void register_unicodestring_ustring(TSRMLS_C) {
	zend_class_entry ce;

	std::memcpy(&ustring_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	ustring_object_handlers.cast_object = php_unicodestring_ustring_cast_object;
	ustring_object_handlers.clone_obj = php_unicodestring_ustring_clone_object;
	ustring_object_handlers.compare_objects = php_unicodestring_ustring_compare_objects;
	ustring_object_handlers.count_elements = php_unicodestring_ustring_count_elements;
	ustring_object_handlers.get_properties = php_unicodestring_ustring_get_properties;

	INIT_CLASS_ENTRY(ce, "unicodestring\\UString", ustring_functions);
	ce.create_object = php_unicodestring_ustring_object_new;
	unicodestring_UString = zend_register_internal_class(&ce TSRMLS_CC);

	zend_class_implements(unicodestring_UString TSRMLS_CC, 1, zend_ce_arrayaccess);
}

// Methods.
PHP_METHOD(UString, __construct) {
	char *string = NULL, *charset = NULL;
	int stringSize = 0, charsetSize = 0;
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ss", &string, &stringSize, &charset, &charsetSize) == FAILURE) {
		RETURN_FALSE;
	}

	if (string) {
		if (charset) {
			ustring_set(intern, string, stringSize, charset TSRMLS_CC);
		}
		else {
			ustring_set(intern, string, stringSize, "UTF-8" TSRMLS_CC);
		}
	}
	else {
		ustring_set(intern, "", 0, "UTF-8" TSRMLS_CC);
	}
}

PHP_METHOD(UString, __toString) {
	php_unicodestring_ustring_cast_object(getThis(), return_value, IS_STRING TSRMLS_CC);
}

PHP_METHOD(UString, chr) {
	long codepoint = 0;
	UChar32 cp32;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &codepoint) == FAILURE) {
		RETURN_FALSE;
	}

	cp32 = (UChar32) codepoint;
	ustring_create_object(return_value);
	getIntern(return_value)->ustr->set(&cp32, 1);
}

PHP_METHOD(UString, encode) {
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);
	char *charset = NULL;
	int charsetSize = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &charset, &charsetSize) == FAILURE) {
		RETURN_FALSE;
	}

	try {
		std::string encoded(intern->ustr->toEncoding(charset));
		RETURN_STRINGL(encoded.c_str(), encoded.size(), 1);
	} catch (ConversionError e) {
		char format[] = "Error converting string to charset %s: %s";
		zend_throw_exception_ex(unicodestring_ConversionException, 0 TSRMLS_CC, format, charset, e.what());
	}
}

PHP_METHOD(UString, explode) {
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);
	zval *delimiter = NULL;
	long limit = LONG_MAX;
	UString delim;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|l", &delimiter, &limit) == FAILURE) {
		RETURN_FALSE;
	}

	if (!ustring_parse_stringy_zval(delimiter, delim TSRMLS_CC)) {
		php_error(E_WARNING, "Delimiter must be a string or UString object");
		RETURN_FALSE;
	}

	if (delim.length() == 0) {
		php_error(E_WARNING, "Empty delimiter");
		RETURN_FALSE;
	}

	array_init(return_value);

	if (limit > 1 || limit < 0) {
		size_t i = 1;
		size_t loopLimit = limit > 0 ? limit : LONG_MAX;
		size_t pos = 0;
		size_t length = intern->ustr->length();

		for (; i < loopLimit && pos < length; i++) {
			zval *zv;
			MAKE_STD_ZVAL(zv);

			try {
				UString chunk(intern->ustr->substring(pos));
				size_t inc = chunk.find(delim);
				
				ustring_create_object(zv, chunk.substring(0, inc));
				add_next_index_zval(return_value, zv);

				pos += inc + delim.length();
			}
			catch (NotFoundError e) {
				ustring_create_object(zv, intern->ustr->substring(pos));
				add_next_index_zval(return_value, zv);
				break;
			}
		}

		if (i >= loopLimit) {
			zval *zv;
			MAKE_STD_ZVAL(zv);

			ustring_create_object(zv, intern->ustr->substring(pos));
			add_next_index_zval(return_value, zv);
		}

		if (limit < 0) {
			for (size_t j = i + limit; j < i; j++) {
				zend_hash_index_del(Z_ARRVAL_P(return_value), j);
			}
		}
	} else {
		zval *zv;
		MAKE_STD_ZVAL(zv);
		ustring_create_object(zv, *intern->ustr);
		add_next_index_zval(return_value, zv);
	}
}

static void php_ustring_html_entities(INTERNAL_FUNCTION_PARAMETERS, int all) {
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);
	long flags = ENT_COMPAT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &flags) == FAILURE) {
		RETURN_FALSE;
	}

	try {
		std::string utf8(intern->ustr->toUTF8());
		unsigned char *utf8Buffer = (unsigned char *) estrndup(utf8.c_str(), utf8.size());
#if PHP_API_VERSION > 20090626
		size_t outputSize = 0;
#else
		int outputSize = 0;
#endif
		char charset[] = "UTF-8";
		char *output = php_escape_html_entities_ex(utf8Buffer, utf8.size(), &outputSize, all, flags, charset, 0 TSRMLS_CC);

		ustring_create_object(return_value, output, outputSize);

		efree(utf8Buffer);
		efree(output);
	} catch (ConversionError e) {
		char format[] = "%s";
		zend_throw_exception_ex(unicodestring_ConversionException, 0 TSRMLS_CC, format, e.what());
	}
}

PHP_METHOD(UString, htmlentities) {
	php_ustring_html_entities(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}

PHP_METHOD(UString, htmlspecialchars) {
	php_ustring_html_entities(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}

static void php_ustring_html_entity_decode(INTERNAL_FUNCTION_PARAMETERS, int all) {
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);
	long flags = ENT_COMPAT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &flags) == FAILURE) {
		RETURN_FALSE;
	}

	try {
		std::string utf8(intern->ustr->toUTF8());
		unsigned char *utf8Buffer = (unsigned char *) estrndup(utf8.c_str(), utf8.size());
#if PHP_API_VERSION > 20090626
		size_t outputSize = 0;
#else
		int outputSize = 0;
#endif
		char charset[] = "UTF-8";
		char *output = php_unescape_html_entities(utf8Buffer, utf8.size(), &outputSize, all, flags, charset TSRMLS_CC);

		ustring_create_object(return_value, output, outputSize);

		efree(utf8Buffer);
		efree(output);
	} catch (ConversionError e) {
		char format[] = "%s";
		zend_throw_exception_ex(unicodestring_ConversionException, 0 TSRMLS_CC, format, e.what());
	}
}

PHP_METHOD(UString, html_entity_decode) {
	php_ustring_html_entity_decode(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}

PHP_METHOD(UString, htmlspecialchars_decode) {
	php_ustring_html_entity_decode(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}

PHP_METHOD(UString, length) {
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);

	RETURN_LONG(intern->ustr->length());
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

		ustring_create_object(return_value);
		getIntern(return_value)->ustr->set(&ch, 1);
	} catch (std::out_of_range e) {
		char format[] = "Index %d is out of range";
		zend_throw_exception_ex(unicodestring_OutOfRangeException, 0 TSRMLS_CC, format, index);
	} catch (MalformedInput e) {
		char format[] = "%s";
		zend_throw_exception_ex(unicodestring_InternalException, 0 TSRMLS_CC, format, e.what());
	} catch (NormalisationError e) {
		char format[] = "%s";
		zend_throw_exception_ex(unicodestring_InternalException, 0 TSRMLS_CC, format, e.what());
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
		char format[] = "Index %d is out of range";
		zend_throw_exception_ex(unicodestring_OutOfRangeException, 0 TSRMLS_CC, format, index);
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
		char format[] = "Index %d is out of range";
		zend_throw_exception_ex(unicodestring_OutOfRangeException, 0 TSRMLS_CC, format, index);
	}
}

PHP_METHOD(UString, toLower) {
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);

	ustring_create_object(return_value, intern->ustr->toLower());
}

PHP_METHOD(UString, toUpper) {
	zval *obj = getThis();
	ustring_obj *intern = getIntern(obj TSRMLS_CC);

	ustring_create_object(return_value, intern->ustr->toUpper());
}

// vim: set ai cin noet ts=8 sw=8:
