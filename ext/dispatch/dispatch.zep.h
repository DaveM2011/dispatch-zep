
extern zend_class_entry *dispatch_dispatch_ce;

ZEPHIR_INIT_CLASS(Dispatch_Dispatch);

PHP_METHOD(Dispatch_Dispatch, __construct);
PHP_METHOD(Dispatch_Dispatch, context);
PHP_METHOD(Dispatch_Dispatch, dispatch);
PHP_METHOD(Dispatch_Dispatch, route);
PHP_METHOD(Dispatch_Dispatch, action);
PHP_METHOD(Dispatch_Dispatch, match);
PHP_METHOD(Dispatch_Dispatch, response);
PHP_METHOD(Dispatch_Dispatch, redirect);
PHP_METHOD(Dispatch_Dispatch, serve);
PHP_METHOD(Dispatch_Dispatch, render);
PHP_METHOD(Dispatch_Dispatch, header);
PHP_METHOD(Dispatch_Dispatch, page);
PHP_METHOD(Dispatch_Dispatch, phtml);
zend_object *zephir_init_properties_Dispatch_Dispatch(zend_class_entry *class_type TSRMLS_DC);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dispatch_dispatch_route, 0, 0, 3)
	ZEND_ARG_INFO(0, method)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dispatch_dispatch_action, 0, 0, 3)
	ZEND_ARG_INFO(0, method)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dispatch_dispatch_match, 0, 0, 3)
	ZEND_ARG_ARRAY_INFO(0, actions, 0)
	ZEND_ARG_INFO(0, method)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dispatch_dispatch_response, 0, 0, 1)
	ZEND_ARG_INFO(0, body)
	ZEND_ARG_INFO(0, code)
	ZEND_ARG_ARRAY_INFO(0, headers, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dispatch_dispatch_redirect, 0, 0, 1)
	ZEND_ARG_INFO(0, location)
	ZEND_ARG_INFO(0, code)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dispatch_dispatch_serve, 0, 0, 3)
	ZEND_ARG_ARRAY_INFO(0, actions, 0)
	ZEND_ARG_INFO(0, method)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dispatch_dispatch_render, 0, 0, 1)
	ZEND_ARG_INFO(0, body)
	ZEND_ARG_INFO(0, code)
	ZEND_ARG_ARRAY_INFO(0, headers, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dispatch_dispatch_header, 0, 0, 2)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dispatch_dispatch_page, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_ARRAY_INFO(0, vars, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dispatch_dispatch_phtml, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_ARRAY_INFO(0, vars, 1)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(dispatch_dispatch_method_entry) {
	PHP_ME(Dispatch_Dispatch, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dispatch_Dispatch, context, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dispatch_Dispatch, dispatch, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Dispatch_Dispatch, route, arginfo_dispatch_dispatch_route, ZEND_ACC_PUBLIC)
	PHP_ME(Dispatch_Dispatch, action, arginfo_dispatch_dispatch_action, ZEND_ACC_PUBLIC)
	PHP_ME(Dispatch_Dispatch, match, arginfo_dispatch_dispatch_match, ZEND_ACC_PUBLIC)
	PHP_ME(Dispatch_Dispatch, response, arginfo_dispatch_dispatch_response, ZEND_ACC_PUBLIC)
	PHP_ME(Dispatch_Dispatch, redirect, arginfo_dispatch_dispatch_redirect, ZEND_ACC_PUBLIC)
	PHP_ME(Dispatch_Dispatch, serve, arginfo_dispatch_dispatch_serve, ZEND_ACC_PUBLIC)
	PHP_ME(Dispatch_Dispatch, render, arginfo_dispatch_dispatch_render, ZEND_ACC_PUBLIC)
	PHP_ME(Dispatch_Dispatch, header, arginfo_dispatch_dispatch_header, ZEND_ACC_PUBLIC)
	PHP_ME(Dispatch_Dispatch, page, arginfo_dispatch_dispatch_page, ZEND_ACC_PUBLIC)
	PHP_ME(Dispatch_Dispatch, phtml, arginfo_dispatch_dispatch_phtml, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
