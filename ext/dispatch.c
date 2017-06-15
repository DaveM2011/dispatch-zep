
/* This file was generated automatically by Zephir do not modify it! */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#include "php_ext.h"
#include "dispatch.h"

#include <ext/standard/info.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/globals.h"
#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"



zend_class_entry *dispatch_0__closure_ce;
zend_class_entry *dispatch_action_ce;
zend_class_entry *dispatch_dispatch_ce;
zend_class_entry *dispatch_response_ce;

ZEND_DECLARE_MODULE_GLOBALS(dispatch)

PHP_INI_BEGIN()
	
PHP_INI_END()

static PHP_MINIT_FUNCTION(dispatch)
{
	REGISTER_INI_ENTRIES();
	zephir_module_init();
	ZEPHIR_INIT(Dispatch_Action);
	ZEPHIR_INIT(Dispatch_Dispatch);
	ZEPHIR_INIT(Dispatch_Response);
	ZEPHIR_INIT(dispatch_0__closure);
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(dispatch)
{
	zephir_deinitialize_memory(TSRMLS_C);
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
#endif

/**
 * Initialize globals on each request or each thread started
 */
static void php_zephir_init_globals(zend_dispatch_globals *dispatch_globals TSRMLS_DC)
{
	dispatch_globals->initialized = 0;

	/* Memory options */
	dispatch_globals->active_memory = NULL;

	/* Virtual Symbol Tables */
	dispatch_globals->active_symbol_table = NULL;

	/* Cache Enabled */
	dispatch_globals->cache_enabled = 1;

	/* Recursive Lock */
	dispatch_globals->recursive_lock = 0;

	/* Static cache */
	memset(dispatch_globals->scache, '\0', sizeof(zephir_fcall_cache_entry*) * ZEPHIR_MAX_CACHE_SLOTS);


}

/**
 * Initialize globals only on each thread started
 */
static void php_zephir_init_module_globals(zend_dispatch_globals *dispatch_globals TSRMLS_DC)
{

}

static PHP_RINIT_FUNCTION(dispatch)
{

	zend_dispatch_globals *dispatch_globals_ptr;
#ifdef ZTS
	tsrm_ls = ts_resource(0);
#endif
	dispatch_globals_ptr = ZEPHIR_VGLOBAL;

	php_zephir_init_globals(dispatch_globals_ptr TSRMLS_CC);
	zephir_initialize_memory(dispatch_globals_ptr TSRMLS_CC);


	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(dispatch)
{
	
	zephir_deinitialize_memory(TSRMLS_C);
	return SUCCESS;
}

static PHP_MINFO_FUNCTION(dispatch)
{
	php_info_print_box_start(0);
	php_printf("%s", PHP_DISPATCH_DESCRIPTION);
	php_info_print_box_end();

	php_info_print_table_start();
	php_info_print_table_header(2, PHP_DISPATCH_NAME, "enabled");
	php_info_print_table_row(2, "Author", PHP_DISPATCH_AUTHOR);
	php_info_print_table_row(2, "Version", PHP_DISPATCH_VERSION);
	php_info_print_table_row(2, "Build Date", __DATE__ " " __TIME__ );
	php_info_print_table_row(2, "Powered by Zephir", "Version " PHP_DISPATCH_ZEPVERSION);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

static PHP_GINIT_FUNCTION(dispatch)
{
	php_zephir_init_globals(dispatch_globals TSRMLS_CC);
	php_zephir_init_module_globals(dispatch_globals TSRMLS_CC);
}

static PHP_GSHUTDOWN_FUNCTION(dispatch)
{

}


zend_function_entry php_dispatch_functions[] = {
ZEND_FE_END

};

zend_module_entry dispatch_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	NULL,
	PHP_DISPATCH_EXTNAME,
	php_dispatch_functions,
	PHP_MINIT(dispatch),
#ifndef ZEPHIR_RELEASE
	PHP_MSHUTDOWN(dispatch),
#else
	NULL,
#endif
	PHP_RINIT(dispatch),
	PHP_RSHUTDOWN(dispatch),
	PHP_MINFO(dispatch),
	PHP_DISPATCH_VERSION,
	ZEND_MODULE_GLOBALS(dispatch),
	PHP_GINIT(dispatch),
	PHP_GSHUTDOWN(dispatch),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_DISPATCH
ZEND_GET_MODULE(dispatch)
#endif
