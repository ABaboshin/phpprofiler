/* phpprofiler extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_phpprofiler.h"

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

	return SUCCESS;
}

PHP_MINIT_FUNCTION(phpprofiler)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(phpprofiler)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "phpprofiler support", "enabled");
	php_info_print_table_end();
}

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

#ifdef COMPILE_DL_PHPPROFILER
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(phpprofiler)
#endif
