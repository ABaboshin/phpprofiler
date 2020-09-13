#include <iostream>
#include "Zend/zend_types.h"
#include <php.h>

int phpprofiler_resource = -2;

extern "C" zend_bool register_hook(zval *class_name, zval *function_name, zval *callable)
{

  // std::cout << "class_name " << std::hex << (Z_TYPE_P(class_name) == IS_STRING) << " " << class_name << std::endl;
  // std::cout << "class_name " << std::hex << (Z_TYPE_P(function_name) == IS_STRING) << " " << function_name << std::endl;
  // std::cout << "class_name " << std::hex << (Z_TYPE_P(callable) == IS_STRING) << " " << callable << std::endl;

  // auto ce = zend_lookup_class_ex(callable->value.str, NULL, ZEND_FETCH_CLASS_NO_AUTOLOAD);
  // zval obj;
  // auto object_init_ex_res = object_init_ex(&obj, ce);
  // std::cout << "result " << (object_init_ex_res == FAILURE) << std::endl;

  // auto ctr = zend_string_init(ZEND_STRL("__construct"), 0);
  // auto method = zend_std_get_static_method(ce, ctr, NULL);
  // zend_string_release(ctr);
  // std::cout << "method " << method << std::endl;

  // zval retval;

  // zend_fcall_info fci;
  // fci.size = sizeof(zend_fcall_info);
  // fci.retval = &retval;
  // fci.params = NULL;
  // fci.object = Z_OBJ_P(&obj);
  // fci.no_separation = 0;
  // fci.param_count = 0;

  // ZVAL_STR(&fci.function_name, method->common.function_name);

  // zend_fcall_info_cache fcc;

  // fcc.function_handler = method;
  // fcc.calling_scope = ce;
  // fcc.called_scope = ce;
  // fcc.object = Z_OBJ_P(&obj);

  // auto result = zend_call_function(&fci, &fcc);

  // // zval result;
  // // auto call_user_function_result = call_user_function(NULL, &obj, ce, ZEND_STRL("__construct"), &result, 0, NULL);
  // std::cout << "result " << (result == FAILURE) << std::endl;

  // // std::cout << "function_name " << std::hex << Z_TYPE_P(function_name) << std::endl;
  // // std::cout << "callable " << std::hex << Z_TYPE_P(callable) << std::endl;
}
