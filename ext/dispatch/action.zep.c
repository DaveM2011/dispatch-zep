
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
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/concat.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Dispatch_Action) {

	ZEPHIR_REGISTER_CLASS(Dispatch, Action, dispatch, action, dispatch_action_method_entry, 0);

	zend_declare_property_null(dispatch_action_ce, SL("method"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(dispatch_action_ce, SL("path"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(dispatch_action_ce, SL("func"), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Dispatch_Action, __construct) {

	zval *method, method_sub, *path, path_sub, *func, func_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&method_sub);
	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&func_sub);

	zephir_fetch_params(0, 3, 0, &method, &path, &func);



	zephir_update_property_zval(this_ptr, SL("method"), method);
	zephir_update_property_zval(this_ptr, SL("path"), path);
	zephir_update_property_zval(this_ptr, SL("func"), func);

}

PHP_METHOD(Dispatch_Action, match) {

	zend_bool _4;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *method, method_sub, *path, path_sub, rexp, caps, _0, _1, _2, _3, _5, _6, _7, _8;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&method_sub);
	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&rexp);
	ZVAL_UNDEF(&caps);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_8);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &method, &path);



	zephir_read_property(&_0, this_ptr, SL("path"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "@:(\\w+)@");
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "(?<\\1>[^/]+)");
	ZEPHIR_CALL_FUNCTION(&rexp, "preg_replace", NULL, 1, &_1, &_2, &_0);
	zephir_check_call_status();
	zephir_read_property(&_3, this_ptr, SL("method"), PH_NOISY_CC | PH_READONLY);
	_4 = !ZEPHIR_IS_EQUAL(method, &_3);
	if (!(_4)) {
		ZEPHIR_INIT_NVAR(&_1);
		ZEPHIR_INIT_VAR(&_5);
		ZEPHIR_CONCAT_SVS(&_5, "@^", &rexp, "$@");
		zephir_preg_match(&_1, &_5, path, &caps, 0, 0 , 0  TSRMLS_CC);
		_4 = !zephir_is_true(&_1);
	}
	if (_4) {
		RETURN_MM_NULL();
	}
	zephir_create_array(return_value, 2, 0 TSRMLS_CC);
	ZEPHIR_OBS_VAR(&_6);
	zephir_read_property(&_6, this_ptr, SL("func"), PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_6);
	ZVAL_LONG(&_7, 1);
	ZEPHIR_CALL_FUNCTION(&_8, "array_slice", NULL, 2, &caps, &_7);
	zephir_check_call_status();
	zephir_array_fast_append(return_value, &_8);
	RETURN_MM();

}

