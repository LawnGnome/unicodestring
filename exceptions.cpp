#include "php_unicodestring.h"

#include <string>
#include <sstream>

extern "C" {
#	include "ext/spl/spl_exceptions.h"
#	include "ext/standard/php_smart_str.h"
#	include "Zend/zend_interfaces.h"
}

zend_class_entry *unicodestring_InvalidInputException;
zend_class_entry *unicodestring_OutOfRangeException;

static function_entry empty_functions[] = {
	{ NULL, NULL, NULL }
};

ZEND_BEGIN_ARG_INFO_EX(unicodestring_InvalidInputException_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, input)
	ZEND_ARG_INFO(0, charset)
ZEND_END_ARG_INFO()


static function_entry InvalidInputException_functions[] = {
	PHP_ME(InvalidInputException, __construct, unicodestring_InvalidInputException_construct, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

zend_class_entry *register_class(const char *name, function_entry *functions, zend_class_entry *parent TSRMLS_DC) {
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, name, functions);
	return zend_register_internal_class_ex(&ce, parent, parent->name TSRMLS_CC);
}

void register_unicodestring_exceptions(TSRMLS_D) {
	unicodestring_InvalidInputException = register_class("unicodestring\\InvalidInputException", InvalidInputException_functions, spl_ce_InvalidArgumentException TSRMLS_CC);
	unicodestring_OutOfRangeException = register_class("unicodestring\\OutOfRangeException", empty_functions, spl_ce_OutOfRangeException TSRMLS_CC);
}

PHP_METHOD(InvalidInputException, __construct) {
	char *input, *charset;
	int inputLen, charsetLen;
	std::ostringstream ss;
	zval message, *obj = getThis();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &input, &inputLen, &charset, &charsetLen) == FAILURE) {
		RETURN_FALSE;
	}

	ss << "Character set '"
		<< std::string(charset, charsetLen)
		<< "' cannot parse input '"
		<< std::string(input, inputLen)
		<< "'";
	std::string s(ss.str());

	INIT_ZVAL(message);
	ZVAL_STRINGL(&message, s.c_str(), s.length(), 0);

	zend_call_method_with_1_params(&obj, spl_ce_InvalidArgumentException, &spl_ce_InvalidArgumentException->constructor, "__construct", NULL, &message);
}

// vim: set ai cin noet ts=8 sw=8:
