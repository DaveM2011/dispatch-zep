
extern zend_class_entry *dispatch_response_ce;

ZEPHIR_INIT_CLASS(Dispatch_Response);

PHP_METHOD(Dispatch_Response, __construct);
PHP_METHOD(Dispatch_Response, render);
PHP_METHOD(Dispatch_Response, response);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dispatch_response___construct, 0, 0, 3)
	ZEND_ARG_INFO(0, body)
	ZEND_ARG_INFO(0, code)
	ZEND_ARG_INFO(0, headers)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(dispatch_response_method_entry) {
	PHP_ME(Dispatch_Response, __construct, arginfo_dispatch_response___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dispatch_Response, render, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Dispatch_Response, response, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
