PHP_ARG_ENABLE(dispatch, whether to enable dispatch, [ --enable-dispatch   Enable Dispatch])

if test "$PHP_DISPATCH" = "yes"; then

	

	if ! test "x" = "x"; then
		PHP_EVAL_LIBLINE(, DISPATCH_SHARED_LIBADD)
	fi

	AC_DEFINE(HAVE_DISPATCH, 1, [Whether you have Dispatch])
	dispatch_sources="dispatch.c kernel/main.c kernel/memory.c kernel/exception.c kernel/debug.c kernel/backtrace.c kernel/object.c kernel/array.c kernel/string.c kernel/fcall.c kernel/require.c kernel/file.c kernel/operators.c kernel/math.c kernel/concat.c kernel/variables.c kernel/filter.c kernel/iterator.c kernel/time.c kernel/exit.c dispatch/action.zep.c
	dispatch/dispatch.zep.c
	dispatch/response.zep.c
	dispatch/0__closure.zep.c "
	PHP_NEW_EXTENSION(dispatch, $dispatch_sources, $ext_shared,, )
	PHP_SUBST(DISPATCH_SHARED_LIBADD)

	old_CPPFLAGS=$CPPFLAGS
	CPPFLAGS="$CPPFLAGS $INCLUDES"

	AC_CHECK_DECL(
		[HAVE_BUNDLED_PCRE],
		[
			AC_CHECK_HEADERS(
				[ext/pcre/php_pcre.h],
				[
					PHP_ADD_EXTENSION_DEP([dispatch], [pcre])
					AC_DEFINE([ZEPHIR_USE_PHP_PCRE], [1], [Whether PHP pcre extension is present at compile time])
				],
				,
				[[#include "main/php.h"]]
			)
		],
		,
		[[#include "php_config.h"]]
	)

	AC_CHECK_DECL(
		[HAVE_JSON],
		[
			AC_CHECK_HEADERS(
				[ext/json/php_json.h],
				[
					PHP_ADD_EXTENSION_DEP([dispatch], [json])
					AC_DEFINE([ZEPHIR_USE_PHP_JSON], [1], [Whether PHP json extension is present at compile time])
				],
				,
				[[#include "main/php.h"]]
			)
		],
		,
		[[#include "php_config.h"]]
	)

	CPPFLAGS=$old_CPPFLAGS

	PHP_INSTALL_HEADERS([ext/dispatch], [php_DISPATCH.h])

fi
