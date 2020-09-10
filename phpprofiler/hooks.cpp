// #include "hooks.h"
#include "Zend/zend_types.h"
#include <iostream>

int phpprofiler_resource = -2;

extern "C" zend_bool register_hook(zval *class_name, zval *function_name, zval *callable)
{
  std::cout << "Hallo From c++ " << phpprofiler_resource << std::endl;
}
