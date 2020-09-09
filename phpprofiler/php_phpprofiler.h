/* phpprofiler extension for PHP */

#ifndef PHP_PHPPROFILER_H
# define PHP_PHPPROFILER_H

extern zend_module_entry phpprofiler_module_entry;
# define phpext_phpprofiler_ptr &phpprofiler_module_entry

# define PHP_PHPPROFILER_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_PHPPROFILER)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_PHPPROFILER_H */
