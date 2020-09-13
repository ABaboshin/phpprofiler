#pragma once

#include <string>
#include "Zend/zend_types.h"

struct Interceptor
{
  std::string className;
  std::string methodName;
  std::string interceptorClass;

  Interceptor(std::string& c, std::string& m, std::string& i) : className(c), methodName(m), interceptorClass(i){}
};
