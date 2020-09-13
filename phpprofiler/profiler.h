#pragma once

#include <iostream>
#include <vector>
#include "Zend/zend_types.h"
#include <interceptor.h>

class Profiler
{

public:

std::vector<Interceptor> interceptors{};

  Profiler () {
    interceptors = std::vector<Interceptor>();
std::cout << "ctor " << interceptors.size() << std::endl;
  }

  zend_bool registerInterceptor(zval *className, zval *functionName, zval *interceptorClass);
};
