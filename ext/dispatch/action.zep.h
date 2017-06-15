
extern zend_class_entry *dispatch_action_ce;

ZEPHIR_INIT_CLASS(Dispatch_Action);

PHP_METHOD(Dispatch_Action, __construct);
PHP_METHOD(Dispatch_Action, match);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dispatch_action___construct, 0, 0, 3)
	ZEND_ARG_INFO(0, method)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dispatch_action_match, 0, 0, 2)
	ZEND_ARG_INFO(0, method)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(dispatch_action_method_entry) {
	PHP_ME(Dispatch_Action, __construct, arginfo_dispatch_action___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dispatch_Action, match, arginfo_dispatch_action_match, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
