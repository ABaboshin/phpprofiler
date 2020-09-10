#pragma once

#include "hooks.h"
#include "zend_handlers.h"

extern zend_module_entry phpprofiler_module_entry;
extern zend_extension phpprofiler_zend_extension_entry;
# define phpext_phpprofiler_ptr &phpprofiler_module_entry

# define PHP_PHPPROFILER_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_PHPPROFILER)
ZEND_TSRMLS_CACHE_EXTERN()
# endif
