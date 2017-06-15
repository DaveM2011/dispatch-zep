
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
#include "kernel/main.h"
#include "kernel/string.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/exception.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/require.h"


ZEPHIR_INIT_CLASS(Dispatch_Dispatch) {

	ZEPHIR_REGISTER_CLASS(Dispatch, Dispatch, dispatch, dispatch, dispatch_dispatch_method_entry, 0);

	zend_declare_property_null(dispatch_dispatch_ce, SL("_inst"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(dispatch_dispatch_ce, SL("routes"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(dispatch_dispatch_ce, SL("vars"), ZEND_ACC_PRIVATE TSRMLS_CC);

	dispatch_dispatch_ce->create_object = zephir_init_properties_Dispatch_Dispatch;
	return SUCCESS;

}

PHP_METHOD(Dispatch_Dispatch, __construct) {

	zval *this_ptr = getThis();


	zend_update_static_property(dispatch_dispatch_ce, ZEND_STRL("_inst"), this_ptr);

}

PHP_METHOD(Dispatch_Dispatch, context) {

	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	zephir_read_static_property_ce(&_0, dispatch_dispatch_ce, SL("_inst"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);

}

PHP_METHOD(Dispatch_Dispatch, dispatch) {

	zval _8, _10, _13;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *_SERVER, *_POST, args, method, path, responder, _0, _1, _2, _3, _4, _5, _9, _11, _12, _14, _6$$4, _7$$5;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&args);
	ZVAL_UNDEF(&method);
	ZVAL_UNDEF(&path);
	ZVAL_UNDEF(&responder);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_9);
	ZVAL_UNDEF(&_11);
	ZVAL_UNDEF(&_12);
	ZVAL_UNDEF(&_14);
	ZVAL_UNDEF(&_6$$4);
	ZVAL_UNDEF(&_7$$5);
	ZVAL_UNDEF(&_8);
	ZVAL_UNDEF(&_10);
	ZVAL_UNDEF(&_13);

	ZEPHIR_MM_GROW();
	zephir_get_global(&_POST, SL("_POST"));
	if (!_POST) {
		ZEPHIR_THROW_EXCEPTION_STR(zend_exception_get_default(), "Invalid superglobal");
		return;
	}
	zephir_get_global(&_SERVER, SL("_SERVER"));
	if (!_SERVER) {
		ZEPHIR_THROW_EXCEPTION_STR(zend_exception_get_default(), "Invalid superglobal");
		return;
	}

	ZEPHIR_INIT_VAR(&args);
	zephir_get_args(&args);
	zephir_array_fetch_string(&_0, _SERVER, SL("REQUEST_METHOD"), PH_NOISY | PH_READONLY, "dispatch/dispatch.zep", 18 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&method);
	zephir_fast_strtoupper(&method, &_0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_array_fetch_string(&_2, _SERVER, SL("REQUEST_URI"), PH_NOISY | PH_READONLY, "dispatch/dispatch.zep", 19 TSRMLS_CC);
	ZVAL_LONG(&_3, 5);
	ZEPHIR_CALL_FUNCTION(&_4, "parse_url", NULL, 3, &_2, &_3);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&_5);
	ZVAL_STRING(&_5, "/");
	zephir_fast_trim(&_1, &_4, &_5, ZEPHIR_TRIM_BOTH TSRMLS_CC);
	ZEPHIR_INIT_VAR(&path);
	ZEPHIR_CONCAT_SV(&path, "/", &_1);
	if (ZEPHIR_IS_STRING(&method, "POST")) {
		if (zephir_array_isset_string(_SERVER, SL("HTTP_X_HTTP_METHOD_OVERRIDE"))) {
			zephir_array_fetch_string(&_6$$4, _SERVER, SL("HTTP_X_HTTP_METHOD_OVERRIDE"), PH_NOISY | PH_READONLY, "dispatch/dispatch.zep", 23 TSRMLS_CC);
			ZEPHIR_INIT_NVAR(&method);
			zephir_fast_strtoupper(&method, &_6$$4);
		} else if (zephir_array_isset_string(_POST, SL("_method"))) {
			zephir_array_fetch_string(&_7$$5, _POST, SL("_method"), PH_NOISY | PH_READONLY, "dispatch/dispatch.zep", 25 TSRMLS_CC);
			ZEPHIR_INIT_NVAR(&method);
			zephir_fast_strtoupper(&method, &_7$$5);
		}
	}
	ZEPHIR_INIT_VAR(&_8);
	zephir_create_array(&_8, 2, 0 TSRMLS_CC);
	zephir_array_fast_append(&_8, this_ptr);
	ZEPHIR_INIT_VAR(&_9);
	ZVAL_STRING(&_9, "serve");
	zephir_array_fast_append(&_8, &_9);
	ZEPHIR_INIT_NVAR(&_9);
	ZEPHIR_INIT_VAR(&_10);
	zephir_create_array(&_10, 3, 0 TSRMLS_CC);
	ZEPHIR_OBS_VAR(&_11);
	zephir_read_property(&_11, this_ptr, SL("routes"), PH_NOISY_CC);
	zephir_array_fast_append(&_10, &_11);
	zephir_array_fast_append(&_10, &method);
	zephir_array_fast_append(&_10, &path);
	zephir_fast_array_merge(&_9, &_10, &args TSRMLS_CC);
	ZEPHIR_INIT_VAR(&responder);
	ZEPHIR_CALL_USER_FUNC_ARRAY(&responder, &_8, &_9);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&_12);
	ZEPHIR_INIT_VAR(&_13);
	zephir_create_array(&_13, 2, 0 TSRMLS_CC);
	zephir_array_fast_append(&_13, &responder);
	ZEPHIR_INIT_VAR(&_14);
	ZVAL_STRING(&_14, "render");
	zephir_array_fast_append(&_13, &_14);
	ZEPHIR_CALL_USER_FUNC(&_12, &_13);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Dispatch_Dispatch, route) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *method, method_sub, *path, path_sub, *func, func_sub, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&method_sub);
	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&func_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &method, &path, &func);



	zephir_read_property(&_0, this_ptr, SL("routes"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "action", NULL, 0, method, path, func);
	zephir_check_call_status();
	ZEPHIR_MAKE_REF(&_0);
	ZEPHIR_CALL_FUNCTION(NULL, "array_push", NULL, 4, &_0, &_1);
	ZEPHIR_UNREF(&_0);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Dispatch_Dispatch, action) {

	zval _0;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *method, method_sub, *path, path_sub, *func, func_sub, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&method_sub);
	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&func_sub);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &method, &path, &func);



	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	zephir_array_fast_append(&_0, method);
	zephir_array_fast_append(&_0, path);
	zephir_array_fast_append(&_0, func);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "\\Dispatch\\Action");
	ZEPHIR_LAST_CALL_STATUS = zephir_create_instance_params(return_value, &_1, &_0 TSRMLS_CC);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Dispatch_Dispatch, match) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *actions_param = NULL, *method, method_sub, *path, path_sub, __$null, cpath, action, match, _0, _1, _2, _3, _4, *_5, _9, _7$$3;
	zval actions, _6$$3, _8$$3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&actions);
	ZVAL_UNDEF(&_6$$3);
	ZVAL_UNDEF(&_8$$3);
	ZVAL_UNDEF(&method_sub);
	ZVAL_UNDEF(&path_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&cpath);
	ZVAL_UNDEF(&action);
	ZVAL_UNDEF(&match);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_9);
	ZVAL_UNDEF(&_7$$3);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &actions_param, &method, &path);

	zephir_get_arrval(&actions, actions_param);


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_1, 5);
	ZEPHIR_CALL_FUNCTION(&_2, "parse_url", NULL, 3, path, &_1);
	zephir_check_call_status();
	ZEPHIR_CALL_FUNCTION(&_3, "rawurldecode", NULL, 5, &_2);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_STRING(&_4, "/");
	zephir_fast_trim(&_0, &_3, &_4, ZEPHIR_TRIM_BOTH TSRMLS_CC);
	ZEPHIR_INIT_VAR(&cpath);
	ZEPHIR_CONCAT_SV(&cpath, "/", &_0);
	zephir_is_iterable(&actions, 0, "dispatch/dispatch.zep", 50);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&actions), _5)
	{
		ZEPHIR_INIT_NVAR(&action);
		ZVAL_COPY(&action, _5);
		ZEPHIR_INIT_NVAR(&_6$$3);
		zephir_create_array(&_6$$3, 2, 0 TSRMLS_CC);
		zephir_array_fast_append(&_6$$3, &action);
		ZEPHIR_INIT_NVAR(&_7$$3);
		ZVAL_STRING(&_7$$3, "match");
		zephir_array_fast_append(&_6$$3, &_7$$3);
		ZEPHIR_INIT_NVAR(&_8$$3);
		zephir_create_array(&_8$$3, 2, 0 TSRMLS_CC);
		zephir_array_fast_append(&_8$$3, method);
		zephir_array_fast_append(&_8$$3, &cpath);
		ZEPHIR_INIT_NVAR(&match);
		ZEPHIR_CALL_USER_FUNC_ARRAY(&match, &_6$$3, &_8$$3);
		zephir_check_call_status();
		if (zephir_is_true(&match)) {
			RETURN_CCTOR(&match);
		}
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&action);
	zephir_create_array(return_value, 2, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_9);
	ZEPHIR_INIT_NVAR(&_9);
	zephir_create_closure_ex(&_9, NULL, dispatch_0__closure_ce, SL("__invoke"));
	zephir_array_fast_append(return_value, &_9);
	zephir_array_fast_append(return_value, &__$null);
	RETURN_MM();

}

PHP_METHOD(Dispatch_Dispatch, response) {

	zval headers, _0;
	zend_long code, ZEPHIR_LAST_CALL_STATUS;
	zval *body, body_sub, *code_param = NULL, *headers_param = NULL, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&body_sub);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&headers);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 2, &body, &code_param, &headers_param);

	if (!code_param) {
		code = 200;
	} else {
		code = zephir_get_intval(code_param);
	}
	if (!headers_param) {
		ZEPHIR_INIT_VAR(&headers);
		array_init(&headers);
	} else {
		zephir_get_arrval(&headers, headers_param);
	}


	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	zephir_array_fast_append(&_0, body);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, code);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&_0, &headers);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "\\Dispatch\\Response");
	ZEPHIR_LAST_CALL_STATUS = zephir_create_instance_params(return_value, &_1, &_0 TSRMLS_CC);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Dispatch_Dispatch, redirect) {

	zval _0, _2;
	zend_long code, ZEPHIR_LAST_CALL_STATUS;
	zval *location, location_sub, *code_param = NULL, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&location_sub);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &location, &code_param);

	if (!code_param) {
		code = 302;
	} else {
		code = zephir_get_intval(code_param);
	}


	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, code);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 1, 0 TSRMLS_CC);
	zephir_array_update_string(&_2, SL("location"), location, PH_COPY | PH_SEPARATE);
	zephir_array_fast_append(&_0, &_2);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "\\Dispatch\\Response");
	ZEPHIR_LAST_CALL_STATUS = zephir_create_instance_params(return_value, &_1, &_0 TSRMLS_CC);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Dispatch_Dispatch, serve) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *actions_param = NULL, *method, method_sub, *path, path_sub, args, pair, caps, _0, _1, _3, _2$$3;
	zval actions;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&actions);
	ZVAL_UNDEF(&method_sub);
	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&args);
	ZVAL_UNDEF(&pair);
	ZVAL_UNDEF(&caps);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_2$$3);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &actions_param, &method, &path);

	zephir_get_arrval(&actions, actions_param);


	ZEPHIR_INIT_VAR(&_0);
	zephir_get_args(&_0);
	ZVAL_LONG(&_1, 3);
	ZEPHIR_CALL_FUNCTION(&args, "array_slice", NULL, 2, &_0, &_1);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&pair, this_ptr, "match", NULL, 0, &actions, method, path);
	zephir_check_call_status();
	ZEPHIR_OBS_VAR(&caps);
	if (zephir_array_isset_long_fetch(&caps, &pair, 1, 0 TSRMLS_CC)) {
		ZEPHIR_MAKE_REF(&args);
		ZEPHIR_CALL_FUNCTION(NULL, "array_unshift", NULL, 6, &args, &caps);
		ZEPHIR_UNREF(&args);
		zephir_check_call_status();
		zephir_array_fetch_long(&_2$$3, &pair, 0, PH_NOISY | PH_READONLY, "dispatch/dispatch.zep", 67 TSRMLS_CC);
		ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, &_2$$3, &args);
		zephir_check_call_status();
		RETURN_MM();
	}
	zephir_array_fetch_long(&_3, &pair, 0, PH_NOISY | PH_READONLY, "dispatch/dispatch.zep", 69 TSRMLS_CC);
	ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, &_3, &args);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Dispatch_Dispatch, render) {

	zval headers, _1;
	zend_long code, ZEPHIR_LAST_CALL_STATUS;
	zval *body, body_sub, *code_param = NULL, *headers_param = NULL, _0, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&body_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&headers);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 2, &body, &code_param, &headers_param);

	if (!code_param) {
		code = 200;
	} else {
		code = zephir_get_intval(code_param);
	}
	if (!headers_param) {
		ZEPHIR_INIT_VAR(&headers);
		array_init(&headers);
	} else {
		zephir_get_arrval(&headers, headers_param);
	}


	ZVAL_LONG(&_0, code);
	ZEPHIR_CALL_FUNCTION(NULL, "http_response_code", NULL, 7, &_0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 2, 0 TSRMLS_CC);
	zephir_array_fast_append(&_1, this_ptr);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "header");
	zephir_array_fast_append(&_1, &_2);
	ZEPHIR_MAKE_REF(&headers);
	ZEPHIR_CALL_FUNCTION(NULL, "array_walk", NULL, 8, &headers, &_1);
	ZEPHIR_UNREF(&headers);
	zephir_check_call_status();
	zend_print_zval(body, 0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Dispatch_Dispatch, header) {

	zend_bool _7$$4;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *value, value_sub, *key, key_sub, _0, _1, _2, values, val, *_3, _11, _12, _4$$4, _5$$4, _6$$4, _8$$4, _9$$4, _10$$4;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&values);
	ZVAL_UNDEF(&val);
	ZVAL_UNDEF(&_11);
	ZVAL_UNDEF(&_12);
	ZVAL_UNDEF(&_4$$4);
	ZVAL_UNDEF(&_5$$4);
	ZVAL_UNDEF(&_6$$4);
	ZVAL_UNDEF(&_8$$4);
	ZVAL_UNDEF(&_9$$4);
	ZVAL_UNDEF(&_10$$4);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &value, &key);



	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "/^[a-zA-Z0-9\\'`#$%&*+.^_|~!-]+$/");
	zephir_preg_match(&_1, &_2, key, &_0, 0, 0 , 0  TSRMLS_CC);
	if (!(zephir_is_true(&_1))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Invalid header name - {$key}", "dispatch/dispatch.zep", 79);
		return;
	}
	if (Z_TYPE_P(value) == IS_ARRAY) {
		ZEPHIR_CPY_WRT(&values, value);
	} else {
		ZEPHIR_INIT_NVAR(&values);
		zephir_create_array(&values, 1, 0 TSRMLS_CC);
		zephir_array_fast_append(&values, value);
	}
	zephir_is_iterable(&values, 0, "dispatch/dispatch.zep", 88);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&values), _3)
	{
		ZEPHIR_INIT_NVAR(&val);
		ZVAL_COPY(&val, _3);
		ZEPHIR_INIT_NVAR(&_4$$4);
		ZEPHIR_INIT_NVAR(&_5$$4);
		ZEPHIR_INIT_NVAR(&_6$$4);
		ZVAL_STRING(&_6$$4, "#(?:(?:(?<!\r)\n)|(?:\r(?!\n))|(?:\r\n(?![ \t])))#");
		zephir_preg_match(&_5$$4, &_6$$4, &val, &_4$$4, 0, 0 , 0  TSRMLS_CC);
		_7$$4 = zephir_is_true(&_5$$4);
		if (!(_7$$4)) {
			ZEPHIR_INIT_NVAR(&_8$$4);
			ZEPHIR_INIT_NVAR(&_9$$4);
			ZEPHIR_INIT_NVAR(&_10$$4);
			ZVAL_STRING(&_10$$4, "/[^\\x09\\x0a\\x0d\\x20-\\x7E\\x80-\\xFE]/");
			zephir_preg_match(&_9$$4, &_10$$4, &val, &_8$$4, 0, 0 , 0  TSRMLS_CC);
			_7$$4 = zephir_is_true(&_9$$4);
		}
		if (_7$$4) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Invalid header value - {$val}", "dispatch/dispatch.zep", 85);
			return;
		}
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&val);
	ZEPHIR_INIT_VAR(&_11);
	zephir_fast_join_str(&_11, SL(","), &values TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_12);
	ZEPHIR_CONCAT_VSV(&_12, key, ": ", &_11);
	ZEPHIR_CALL_FUNCTION(NULL, "header", NULL, 9, &_12);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Dispatch_Dispatch, page) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval vars, _0;
	zval *path, path_sub, *vars_param = NULL, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&vars);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &path, &vars_param);

	if (!vars_param) {
		ZEPHIR_INIT_VAR(&vars);
		array_init(&vars);
	} else {
		zephir_get_arrval(&vars, vars_param);
	}


	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0 TSRMLS_CC);
	zephir_array_fast_append(&_0, path);
	zephir_array_fast_append(&_0, &vars);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "\\Dispatch\\Response");
	ZEPHIR_LAST_CALL_STATUS = zephir_create_instance_params(return_value, &_1, &_0 TSRMLS_CC);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Dispatch_Dispatch, phtml) {

	zephir_fcall_cache_entry *_2 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval vars;
	zval *path, path_sub, *vars_param = NULL, _0, _1, _3, _4;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&vars);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &path, &vars_param);

	if (!vars_param) {
		ZEPHIR_INIT_VAR(&vars);
		array_init(&vars);
	} else {
		zephir_get_arrval(&vars, vars_param);
	}


	ZEPHIR_CALL_FUNCTION(NULL, "ob_start", NULL, 10);
	zephir_check_call_status();
	zephir_read_property(&_0, this_ptr, SL("vars"), PH_NOISY_CC | PH_READONLY);
	ZVAL_LONG(&_1, 1);
	ZEPHIR_MAKE_REF(&_0);
	ZEPHIR_CALL_FUNCTION(NULL, "extract", &_2, 11, &_0, &_1);
	ZEPHIR_UNREF(&_0);
	zephir_check_call_status();
	ZVAL_LONG(&_1, 1);
	ZEPHIR_MAKE_REF(&vars);
	ZEPHIR_CALL_FUNCTION(NULL, "extract", &_2, 11, &vars, &_1);
	ZEPHIR_UNREF(&vars);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&_3);
	ZEPHIR_CONCAT_VS(&_3, path, ".phtml");
	if (zephir_require_zval(&_3 TSRMLS_CC) == FAILURE) {
		RETURN_MM_NULL();
	}
	ZEPHIR_CALL_FUNCTION(&_4, "ob_get_clean", NULL, 12);
	zephir_check_call_status();
	zephir_fast_trim(return_value, &_4, NULL , ZEPHIR_TRIM_BOTH TSRMLS_CC);
	RETURN_MM();

}

zend_object *zephir_init_properties_Dispatch_Dispatch(zend_class_entry *class_type TSRMLS_DC) {

		zval _0, _2, _1$$3, _3$$4;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_3$$4);

		ZEPHIR_MM_GROW();
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property(&_0, this_ptr, SL("vars"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			array_init(&_1$$3);
			zephir_update_property_zval(this_ptr, SL("vars"), &_1$$3);
		}
		zephir_read_property(&_2, this_ptr, SL("routes"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_2) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_3$$4);
			array_init(&_3$$4);
			zephir_update_property_zval(this_ptr, SL("routes"), &_3$$4);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}

}

