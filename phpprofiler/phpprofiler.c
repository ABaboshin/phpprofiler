/* phpprofiler extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "Zend/zend_extensions.h"
#include "Zend/zend_vm.h"
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

PHP_RINIT_FUNCTION(phpprofiler)
{
#if defined(ZTS) && defined(COMPILE_DL_PHPPROFILER)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

  fprintf(stdout, "Rinit\n");

  ZEND_VM_SET_OPCODE_HANDLER(EG(exception_op));
  EG(exception_op)->opcode = ZEND_HANDLE_EXCEPTION;

	return SUCCESS;
}

PHP_MINIT_FUNCTION(phpprofiler)
{
  fprintf(stdout, "Minit\n");

  // in order to enable zend_extensions hooks
  Dl_info infos;
  zend_register_extension(&phpprofiler_zend_extension_entry, phpprofiler_module_entry.handle);
  dladdr(ZEND_MODULE_STARTUP_N(phpprofiler), &infos);
  dlopen(infos.dli_fname, RTLD_LAZY);

  // TODO signal handler for SIGSEGV on an alternate stack
  // TODO init a client for the metrics storage (prometheus, jaeger)
  // TODO intercept opcodes like ZEND_DO_UCALL/ZEND_DO_FCALL/ZEND_DO_FCALL_BY_NAME
  // TODO register integrations

	return SUCCESS;
}

PHP_MINFO_FUNCTION(phpprofiler)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "phpprofiler support", "enabled");
	php_info_print_table_end();
}

static int phpprofiler_zend_extension_startup(struct _zend_extension *extension) {
    phpprofiler_resource = zend_get_resource_handle(extension);

    fprintf(stdout, "phpprofiler_zend_extension_startup\n");

    zend_handlers_startup();
    return SUCCESS;
}

static void phpprofiler_zend_extension_shutdown(struct _zend_extension *extension) {}

static void phpprofiler_zend_extension_activate(void) {}
static void phpprofiler_zend_extension_deactivate(void) {}

static const zend_function_entry phpprofiler_functions[] = {
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
