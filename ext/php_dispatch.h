
/* This file was generated automatically by Zephir do not modify it! */

#ifndef PHP_DISPATCH_H
#define PHP_DISPATCH_H 1

#ifdef PHP_WIN32
#define ZEPHIR_RELEASE 1
#endif

#include "kernel/globals.h"

#define PHP_DISPATCH_NAME        "dispatch"
#define PHP_DISPATCH_VERSION     "0.0.1"
#define PHP_DISPATCH_EXTNAME     "dispatch"
#define PHP_DISPATCH_AUTHOR      ""
#define PHP_DISPATCH_ZEPVERSION  "0.9.7-edf9433136"
#define PHP_DISPATCH_DESCRIPTION ""



ZEND_BEGIN_MODULE_GLOBALS(dispatch)

	int initialized;

	/* Memory */
	zephir_memory_entry *start_memory; /**< The first preallocated frame */
	zephir_memory_entry *end_memory; /**< The last preallocate frame */
	zephir_memory_entry *active_memory; /**< The current memory frame */

	/* Virtual Symbol Tables */
	zephir_symbol_table *active_symbol_table;

	/** Function cache */
	HashTable *fcache;

	zephir_fcall_cache_entry *scache[ZEPHIR_MAX_CACHE_SLOTS];

	/* Cache enabled */
	unsigned int cache_enabled;

	/* Max recursion control */
	unsigned int recursive_lock;

	
ZEND_END_MODULE_GLOBALS(dispatch)

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_EXTERN_MODULE_GLOBALS(dispatch)

#ifdef ZTS
	#define ZEPHIR_GLOBAL(v) ZEND_MODULE_GLOBALS_ACCESSOR(dispatch, v)
#else
	#define ZEPHIR_GLOBAL(v) (dispatch_globals.v)
#endif

#ifdef ZTS
	void ***tsrm_ls;
	#define ZEPHIR_VGLOBAL ((zend_dispatch_globals *) (*((void ***) tsrm_get_ls_cache()))[TSRM_UNSHUFFLE_RSRC_ID(dispatch_globals_id)])
#else
	#define ZEPHIR_VGLOBAL &(dispatch_globals)
#endif

#define ZEPHIR_API ZEND_API

#define zephir_globals_def dispatch_globals
#define zend_zephir_globals_def zend_dispatch_globals

extern zend_module_entry dispatch_module_entry;
#define phpext_dispatch_ptr &dispatch_module_entry

#endif
