#include <stdio.h>
#include "php.h"
#include "Zend/zend_extensions.h"
#include "Zend/zend_vm.h"
#include "Zend/zend_closures.h"
#include "ext/standard/info.h"
#include "php_phpprofiler.h"

#include "interceptor.h"

zend_bool registerInterceptor(zval *className, zval *functionName, zval *interceptorClass)
{
  return SUCCESS;
}

void initInterceptors()
{
  fprintf(stdout, "initInterceptors\n");

  ALLOC_HASHTABLE(GET(lookup));
  zend_hash_init(GET(lookup), 8, NULL, ZVAL_PTR_DTOR, 0);
}
