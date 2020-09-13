#include <stdio.h>
#include "php.h"
#include "Zend/zend_extensions.h"
#include "Zend/zend_vm.h"
#include "Zend/zend_closures.h"
#include "ext/standard/info.h"
#include "php_phpprofiler.h"

#include "interceptor.h"

HashTable* getClassLookup(zval *className TSRMLS_DC)
{
  HashTable* lookup = zend_hash_find_ptr(GET(lookup), Z_STR_P(className));
  if (!lookup)
  {
    ALLOC_HASHTABLE(lookup);
    zend_hash_init(lookup, 8, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_update_ptr(GET(lookup), Z_STR_P(className), lookup);
  }

  return lookup;
}

HashTable* getMethodLookup(HashTable* table, zval *methodName)
{
  HashTable* lookup = zend_hash_find_ptr(table, Z_STR_P(methodName));
  if (!lookup)
  {
    ALLOC_HASHTABLE(lookup);
    zend_hash_init(lookup, 8, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_update_ptr(table, Z_STR_P(methodName), lookup);
  }

  return lookup;
}

zend_bool registerInterceptor(zval *className, zval *functionName, zval *interceptorClass)
{
  HashTable* classLookup = getClassLookup(className TSRMLS_CC);
  fprintf(stdout, "classLookup %p\n", classLookup);

  HashTable* methodLookup = getMethodLookup(classLookup, functionName);
  fprintf(stdout, "methodLookup %p\n", methodLookup);

  zend_hash_add_new_ptr(methodLookup, Z_STR_P(interceptorClass), interceptorClass);

  return SUCCESS;
}

void initInterceptors()
{
  fprintf(stdout, "initInterceptors\n");

  ALLOC_HASHTABLE(GET(lookup));
  zend_hash_init(GET(lookup), 8, NULL, ZVAL_PTR_DTOR, 0);
}

void processCall(zend_execute_data *data)
{

}
