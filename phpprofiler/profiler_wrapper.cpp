#include <iostream>
#include <memory>
#include "profiler.h"
#include "profiler_c.h"

Profiler* createProfilerInstance()
{
  auto result = std::make_unique<Profiler>();
  std::cout << "profiler " << std::hex << &*result << " " << sizeof(&*result) << " " << result->interceptors.size() << std::endl;
  return &*result;
}

void injectLoader(Profiler* profiler)
{
  profiler->injectLoader();
}

zend_bool registerInterceptor(Profiler* profiler, zval *className, zval *functionName, zval *interceptorClass)
{
  std::cout << "registerInterceptor " << std::hex << profiler << std::endl;
  std::cout << "registerInterceptor " << std::hex << profiler << " " << profiler->interceptors.size() << std::endl;
  return profiler->registerInterceptor(className, functionName, interceptorClass);
}
