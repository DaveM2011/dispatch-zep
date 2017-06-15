
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
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Dispatch_Response) {

	ZEPHIR_REGISTER_CLASS(Dispatch, Response, dispatch, response, dispatch_response_method_entry, 0);

	zend_declare_property_null(dispatch_response_ce, SL("body"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(dispatch_response_ce, SL("code"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(dispatch_response_ce, SL("headers"), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Dispatch_Response, __construct) {

	zval *body, body_sub, *code, code_sub, *headers, headers_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&body_sub);
	ZVAL_UNDEF(&code_sub);
	ZVAL_UNDEF(&headers_sub);

	zephir_fetch_params(0, 3, 0, &body, &code, &headers);



	zephir_update_property_zval(this_ptr, SL("body"), body);
	zephir_update_property_zval(this_ptr, SL("code"), code);
	zephir_update_property_zval(this_ptr, SL("headers"), headers);

}

PHP_METHOD(Dispatch_Response, render) {

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
	zephir_read_property(&_2, this_ptr, SL("body"), PH_NOISY_CC | PH_READONLY);
	zephir_read_property(&_3, this_ptr, SL("code"), PH_NOISY_CC | PH_READONLY);
	zephir_read_property(&_4, this_ptr, SL("headers"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_CALL_METHOD(NULL, &_0, "render", NULL, 0, &_2, &_3, &_4);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Dispatch_Response, response) {

	zval _0, _2, _3, _4, _5;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_1 = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_CE_STATIC(&_0, dispatch_dispatch_ce, "context", &_1, 0);
	zephir_check_call_status();
	ZEPHIR_CALL_CE_STATIC(&_2, dispatch_dispatch_ce, "context", &_1, 0);
	zephir_check_call_status();
	zephir_read_property(&_4, this_ptr, SL("body"), PH_NOISY_CC | PH_READONLY);
	zephir_read_property(&_5, this_ptr, SL("code"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_CALL_METHOD(&_3, &_2, "phtml", NULL, 0, &_4, &_5);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(&_0, "response", NULL, 0, &_3);
	zephir_check_call_status();
	RETURN_MM();

}

