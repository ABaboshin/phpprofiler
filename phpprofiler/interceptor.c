#include <stdio.h>
#include "php.h"
#include "Zend/zend_extensions.h"
#include "Zend/zend_vm.h"
#include "Zend/zend_closures.h"
#include "ext/standard/info.h"
#include "php_phpprofiler.h"

#include "interceptor.h"

HashTable* getClassLookupStr(zend_string *className TSRMLS_DC)
{
  if (!className) {
    return getClassLookupStr(zend_empty_string TSRMLS_CC);
  }

  HashTable* lookup = zend_hash_find_ptr(GET(lookup), className);
  if (!lookup)
  {
    ALLOC_HASHTABLE(lookup);
    zend_hash_init(lookup, 8, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_update_ptr(GET(lookup), className, lookup);
  }

  return lookup;
}

HashTable* getClassLookup(zval *className TSRMLS_DC)
{
  return getClassLookupStr(Z_STR_P(className) TSRMLS_CC);
}

HashTable* getMethodLookupStr(HashTable* table, zend_string *methodName)
{
  HashTable* lookup = zend_hash_find_ptr(table, methodName);
  if (!lookup)
  {
    ALLOC_HASHTABLE(lookup);
    zend_hash_init(lookup, 8, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_update_ptr(table, methodName, lookup);
  }

  return lookup;
}

HashTable* getMethodLookup(HashTable* table, zval *methodName)
{
  return getMethodLookupStr(table, Z_STR_P(methodName));
}

zend_bool registerInterceptor(zval *className, zval *functionName, zval *interceptorClass)
{
  HashTable* classLookup = getClassLookup(className TSRMLS_CC);
  // fprintf(stdout, "registerInterceptor classLookup %p\n", classLookup);

  HashTable* methodLookup = getMethodLookup(classLookup, functionName);
  // fprintf(stdout, "registerInterceptor methodLookup %p\n", methodLookup);

  zend_hash_add_new(methodLookup, Z_STR_P(interceptorClass), interceptorClass);

  return SUCCESS;
}

void initInterceptors()
{
  // fprintf(stdout, "initInterceptors\n");

  ALLOC_HASHTABLE(GET(lookup));
  zend_hash_init(GET(lookup), 8, NULL, ZVAL_PTR_DTOR, 0);
}

void processICall(zend_execute_data *data)
{
  fprintf(stdout, "processICall\n");
}

void CallFunctionByName(zend_class_entry* interceptorClass, zval* obj, zend_string* methodName, zval *params)
{
  zend_function* ctorMethod = zend_std_get_static_method(interceptorClass, methodName, NULL);

  zval retval;

  zend_fcall_info fci;
  fci.size = sizeof(zend_fcall_info);
  fci.retval = &retval;
  fci.params = params;
  fci.object = Z_OBJ_P(obj);
  fci.no_separation = 0;
  fci.param_count = params ? 1 : 0;

  ZVAL_STR(&fci.function_name, ctorMethod->common.function_name);

  zend_fcall_info_cache fcc;

  fcc.function_handler = ctorMethod;
  fcc.calling_scope = interceptorClass;
  fcc.called_scope = interceptorClass;
  fcc.object = Z_OBJ_P(obj);

  zend_call_function(&fci, &fcc);
}

void CallInterceptor(zval *interceptor, zend_execute_data *data)
{
  zend_class_entry* interceptorClass = zend_lookup_class_ex(Z_STR_P(interceptor), NULL, ZEND_FETCH_CLASS_NO_AUTOLOAD);
  zval obj;
  object_init_ex(&obj, interceptorClass);

  zend_string* ctr = zend_string_init(ZEND_STRL("__construct"), 0);
  CallFunctionByName(interceptorClass, &obj, ctr, NULL);
  zend_string_release(ctr);

  zval params;
  array_init(&params);

  for (int i = 1; i <= ZEND_CALL_NUM_ARGS(data->call); i++)
  {
    add_next_index_zval(&params, ZEND_CALL_ARG(data->call, i));
    // fprintf(stdout, "param %d %d\n", i, Z_TYPE_P(ZEND_CALL_ARG(data->call, i)));
  }

  zend_string* eb = zend_string_init(ZEND_STRL("ExecuteBefore"), 0);
  CallFunctionByName(interceptorClass, &obj, eb, &params);
  zend_string_release(eb);
}

void ProcessMethodCall (zend_execute_data *data)
{
  // fprintf(stdout, "ProcessMethodCall %s\n", ZSTR_VAL(data->call->func->common.function_name));

  zend_string* className = data->call->func->common.scope ? data->call->func->common.scope->name : NULL;

  // if (className)
  // {
  //   fprintf(stdout, "ProcessMethodCall from class %s\n", ZSTR_VAL(className));
  // } else {
  //   fprintf(stdout, "ProcessMethodCall from class NULL\n");
  // }

  HashTable* classLookup = getClassLookupStr(className TSRMLS_CC);
  // fprintf(stdout, "classLookup %p\n", classLookup);
  if (!classLookup) return;
  HashTable* methodLookup = getMethodLookupStr(classLookup, data->call->func->common.function_name);
  // fprintf(stdout, "methodLookup %p\n", methodLookup);
  if (!methodLookup) return;

  zval *interceptor;

  zend_hash_internal_pointer_reset(methodLookup);
  while ((interceptor = zend_hash_get_current_data(methodLookup)) != NULL)
  {
    fprintf(stdout, "Found %s\n", ZSTR_VAL(Z_STR_P(interceptor)));

    CallInterceptor(interceptor, data);

    zend_hash_move_forward(methodLookup);
  }
}

void processUCall(zend_execute_data *data)
{
  // fprintf(stdout, "processUCall\n");
  ProcessMethodCall(data);
}

void processFCall(zend_execute_data *data)
{
  // fprintf(stdout, "processFCall\n");
  ProcessMethodCall(data);
}

void processFCallByName(zend_execute_data *data)
{
  fprintf(stdout, "processFCallByName\n");
}

void processReturn(zend_execute_data *data)
{
  fprintf(stdout, "processReturn\n");
}

void processReturnByRef(zend_execute_data *data)
{
  fprintf(stdout, "processReturnByRef\n");
}

void processYield(zend_execute_data *data)
{
  fprintf(stdout, "processYield\n");
}

void processYieldFrom(zend_execute_data *data)
{
  fprintf(stdout, "processYieldFrom\n");
}

void processException(zend_execute_data *data)
{
  fprintf(stdout, "processException\n");
}
