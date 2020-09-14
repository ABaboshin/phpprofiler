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

ZEND_FUNCTION(internalFunctionWrapperZ)
{
  fprintf(stdout, "zend internalFunctionWrapper\n");
}

PHP_FUNCTION(internalFunctionWrapperP) {
  fprintf(stdout, "php internalFunctionWrapper\n");
}

zend_bool registerInterceptor(zval *className, zval *functionName, zval *interceptorClass, int asInternal)
{
  HashTable* classLookup = getClassLookup(className TSRMLS_CC);
  HashTable* methodLookup = getMethodLookup(classLookup, functionName);

  zend_hash_add_new(methodLookup, Z_STR_P(interceptorClass), interceptorClass);

  if (asInternal == 1)
  {
    zend_function* function = zend_hash_find(CG(function_table), Z_STR_P(functionName));
    if (function)
    {
      function->internal_function.handler = ZEND_FN(internalFunctionWrapperZ);
    }

    zend_internal_function* internalFunction = zend_hash_find(CG(function_table), Z_STR_P(functionName));
    if (function)
    {
      internalFunction->handler = PHP_FN(internalFunctionWrapperP);
    }
  }

  return SUCCESS;
}

void initInterceptors()
{
  ALLOC_HASHTABLE(GET(lookup));
  zend_hash_init(GET(lookup), 8, NULL, ZVAL_PTR_DTOR, 0);
  GET(callStack) = NULL;
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

void CallInterceptorBefore(zval *interceptor, zend_execute_data *data)
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
  }

  zend_string* eb = zend_string_init(ZEND_STRL("ExecuteBefore"), 0);
  CallFunctionByName(interceptorClass, &obj, eb, &params);
  zend_string_release(eb);
}

void ProcessMethodCall (zend_execute_data *data)
{
  zend_string* className = data->call->func->common.scope ? data->call->func->common.scope->name : NULL;

  HashTable* classLookup = getClassLookupStr(className TSRMLS_CC);
  if (!classLookup) return;

  HashTable* methodLookup = getMethodLookupStr(classLookup, data->call->func->common.function_name);
  if (!methodLookup) return;

  zval *interceptor;

  // fprintf(stdout, "enter %p %p %s\n", data, data->call, ZSTR_VAL(data->call->func->common.function_name));

  if (!GET(callStack)) {
    GET(callStack) = createNewStack(data->call, methodLookup);
  } else {
    GET(callStack) = pushStack(GET(callStack), data->call, methodLookup);
  }

  zend_hash_internal_pointer_reset(methodLookup);
  while ((interceptor = zend_hash_get_current_data(methodLookup)) != NULL)
  {
    CallInterceptorBefore(interceptor, data);

    zend_hash_move_forward(methodLookup);
  }
}

void processUCall(zend_execute_data *data)
{
  ProcessMethodCall(data);
}

void processFCall(zend_execute_data *data)
{
  ProcessMethodCall(data);
}

void processFCallByName(zend_execute_data *data)
{
  ProcessMethodCall(data);
}

void CallInterceptorAfter(zval *interceptor, zend_execute_data *execute_data)
{
  zend_class_entry* interceptorClass = zend_lookup_class_ex(Z_STR_P(interceptor), NULL, ZEND_FETCH_CLASS_NO_AUTOLOAD);
  zval obj;
  object_init_ex(&obj, interceptorClass);

  zend_string* ctr = zend_string_init(ZEND_STRL("__construct"), 0);
  CallFunctionByName(interceptorClass, &obj, ctr, NULL);
  zend_string_release(ctr);

  zval vn;
  zval *retval = NULL;
  switch(execute_data->opline->op1_type) {
    case IS_CONST:
      retval = RT_CONSTANT(execute_data->opline, execute_data->opline->op1);
      break;
    case IS_TMP_VAR:
    case IS_VAR:
    case IS_CV:
      retval = EX_VAR(execute_data->opline->op1.var);
      break;
  }

  if (!retval || Z_TYPE_INFO_P(retval) == IS_UNDEF) {
    ZVAL_NULL(&vn);
    retval = &vn;
  }

  zend_string* eb = zend_string_init(ZEND_STRL("ExecuteAfter"), 0);
  CallFunctionByName(interceptorClass, &obj, eb, retval);
  zend_string_release(eb);
}

void processReturn(zend_execute_data *data)
{
  if (GET(callStack)) {
    CallStack* top = GET(callStack);
    if (top->data == data) {
      zval *interceptor;
      zend_hash_internal_pointer_reset(top->interceptors);
      while ((interceptor = zend_hash_get_current_data(top->interceptors)) != NULL)
      {
        CallInterceptorAfter(interceptor, data);

        zend_hash_move_forward(top->interceptors);
      }

      GET(callStack) = popStack(GET(callStack));
    }
  }
}

void processReturnByRef(zend_execute_data *data)
{
  processReturn(data);
}

void processException(zend_execute_data *data)
{
  fprintf(stdout, "processException\n");
}
