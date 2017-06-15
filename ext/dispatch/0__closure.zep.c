
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(dispatch_0__closure) {

	ZEPHIR_REGISTER_CLASS(dispatch, 0__closure, dispatch, 0__closure, dispatch_0__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	return SUCCESS;

}

PHP_METHOD(dispatch_0__closure, __invoke) {

	zval _0, _2, _3, _4;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_1 = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_CE_STATIC(&_0, dispatch_dispatch_ce, "context", &_1, 0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&_2);
	array_init(&_2);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_STRING(&_3, "");
	ZVAL_LONG(&_4, 404);
	ZEPHIR_RETURN_CALL_METHOD(&_0, "response", NULL, 0, &_3, &_4, &_2);
	zephir_check_call_status();
	RETURN_MM();

}

