/* phpprofiler extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "Zend/zend_extensions.h"
#include "Zend/zend_vm.h"
#include "Zend/zend_closures.h"
#include "ext/standard/info.h"
#include "php_phpprofiler.h"
#include <stdio.h>
#include <stdatomic.h>

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

ZEND_DECLARE_MODULE_GLOBALS(phpprofiler)

void injectLoader ()
{
  char* loaderPath = getenv("PHPPROFILER_CONFIGURATION");

  fprintf(stdout, "Rinit %s\n", loaderPath);

  zend_file_handle loadFile;
  memset(&loadFile, 0, sizeof(zend_file_handle));
  loadFile.type = ZEND_HANDLE_FILENAME;
  loadFile.filename = loaderPath;
  zend_execute_scripts(ZEND_REQUIRE TSRMLS_CC, NULL, 1, &loadFile);
}

PHP_RINIT_FUNCTION(phpprofiler)
{
#if defined(ZTS) && defined(COMPILE_DL_PHPPROFILER)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

  fprintf(stdout, "PHP_RINIT_FUNCTION\n");

  // catch exceptions
  ZEND_VM_SET_OPCODE_HANDLER(EG(exception_op));
  EG(exception_op)->opcode = ZEND_HANDLE_EXCEPTION;

  injectLoader();

	return SUCCESS;
}

PHP_MINIT_FUNCTION(phpprofiler)
{
  fprintf(stdout, "PHP_MINIT_FUNCTION\n");

  // in order to enable zend_extensions hooks
  Dl_info infos;
  zend_register_extension(&phpprofiler_zend_extension_entry, phpprofiler_module_entry.handle);
  dladdr(ZEND_MODULE_STARTUP_N(phpprofiler), &infos);
  dlopen(infos.dli_fname, RTLD_LAZY);

  // TODO signal handler for SIGSEGV on an alternate stack

  intercept_opcodes();

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(phpprofiler)
{
  fprintf(stdout, "PHP_MSHUTDOWN_FUNCTION\n");

  cancel_intercept_opcodes();

	return SUCCESS;
}

PHP_MINFO_FUNCTION(phpprofiler)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "phpprofiler support", "enabled");
	php_info_print_table_end();
}

PHP_FUNCTION(trace_method)
{
  zval* className;
  zval* methodName;
  zval* interceptorClass;

  // fprintf(stdout, "trace_method %d %d %d\n",
  // zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &className, &methodName,
  //                                &interceptorClass, zend_ce_closure),
  //                                Z_TYPE_P(className) != IS_STRING,
  //                                Z_TYPE_P(methodName) != IS_STRING);

  // https://www.php.net/manual/de/internals2.funcs.php
  if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &className, &methodName,
                                 &interceptorClass, zend_ce_closure) != SUCCESS
                                 || Z_TYPE_P(className) != IS_STRING
                                 || Z_TYPE_P(methodName) != IS_STRING
                                 )
                                 {
                                  //  fprintf(stdout, "oops\n");
                                   RETURN_BOOL(0);
                                 }

  zend_bool result = registerInterceptor(className, methodName, interceptorClass);

  RETURN_BOOL(result);
}

PHP_FUNCTION(trace_function)
{
  zval* functionName;
  zval* interceptorClass;

  // https://www.php.net/manual/de/internals2.funcs.php
  if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "sO", &functionName,
                                 &interceptorClass, zend_ce_closure) != SUCCESS
                                 || Z_TYPE_P(functionName) != IS_STRING
                                 )
                                 {
                                   RETURN_BOOL(0);
                                 }

  zend_bool result = registerInterceptor(NULL, functionName, interceptorClass);

  RETURN_BOOL(result);
}

int phpprofiler_zend_extension_startup(struct _zend_extension *extension) {
    fprintf(stdout, "phpprofiler_zend_extension_startup\n");

    zend_handlers_startup();
    return SUCCESS;
}

void phpprofiler_zend_extension_shutdown(struct _zend_extension *extension) {}

void phpprofiler_zend_extension_activate(void) {}
void phpprofiler_zend_extension_deactivate(void) {}

ZEND_BEGIN_ARG_INFO_EX(arginfo_trace_method, 0, 0, 3)
ZEND_ARG_INFO(0, className)
ZEND_ARG_INFO(0, methodName)
ZEND_ARG_INFO(0, interceptorClass)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_trace_function, 0, 0, 2)
ZEND_ARG_INFO(0, functionName)
ZEND_ARG_INFO(0, interceptorClass)
ZEND_END_ARG_INFO()

static const zend_function_entry phpprofiler_functions[] = {
  PHP_FE(trace_method, arginfo_trace_method)
  PHP_FE(trace_function, arginfo_trace_function)
	PHP_FE_END
};

zend_module_entry phpprofiler_module_entry = {
	STANDARD_MODULE_HEADER,
	"phpprofiler",					/* Extension name */
	phpprofiler_functions,			/* zend_function_entry */
	PHP_MINIT(phpprofiler),							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(phpprofiler),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(phpprofiler),			/* PHP_MINFO - Module info */
	PHP_PHPPROFILER_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};

zend_extension phpprofiler_zend_extension_entry = {
    "phpprofiler",
    "1.0",
    "Andrey Baboshin",
    "https://github.com/ABaboshin/phpprofiler",
    "Andrey Baboshin",
    phpprofiler_zend_extension_startup,                               /* startup() : module startup */
    phpprofiler_zend_extension_shutdown,                               /* shutdown() : module shutdown */
    phpprofiler_zend_extension_activate,        /* activate() : request startup */
    phpprofiler_zend_extension_deactivate,      /* deactivate() : request shutdown */
    NULL, /* message_handler() */
    NULL,      /* compiler op_array_handler() */
    NULL,                                     /* VM statement_handler() */
    NULL,   /* VM fcall_begin_handler() */
    NULL,                                     /* VM fcall_end_handler() */
    NULL,                                     /* compiler op_array_ctor() */
    NULL,                                     /* compiler op_array_dtor() */
    STANDARD_ZEND_EXTENSION_PROPERTIES        /* Structure-ending macro */
};

#ifdef COMPILE_DL_PHPPROFILER
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(phpprofiler)
#endif
