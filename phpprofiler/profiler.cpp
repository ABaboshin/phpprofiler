#include <iostream>
#include "Zend/zend_types.h"
#include <php.h>
#include "profiler.h"
#include "profiler_c.h"

void Profiler::injectLoader()
{
  char* loaderPath = getenv("PHPPROFILER_CONFIGURATION");

  std::cout << "inject on start " << loaderPath << std::endl;

  zend_file_handle loadFile = {};
  loadFile.type = ZEND_HANDLE_FILENAME;
  loadFile.filename = loaderPath;
  zend_execute_scripts(ZEND_REQUIRE TSRMLS_CC, NULL, 1, &loadFile);
}

zend_bool Profiler::registerInterceptor(zval *className, zval *functionName, zval *interceptorClass)
{
  // std::cout << "class " << ZSTR_VAL(Z_STR_P(className)) << " method " << ZSTR_VAL(Z_STR_P(functionName)) << " type " << std::hex << ZSTR_VAL(Z_STR_P(interceptorClass)) << std::endl;

  std::cout << 1 << std::endl;

  std::string cn;
  if (className != NULL)
  {
    cn = std::string(ZSTR_VAL(Z_STR_P(className)));
  }

  std::cout << 2 << std::endl;

  std::string fn(ZSTR_VAL(Z_STR_P(functionName)));
  std::string in(ZSTR_VAL(Z_STR_P(interceptorClass)));

std::cout << 3 << std::endl;

  Interceptor interceptor(
    cn,
    fn,
    in
  );

  std::cout << 4 << std::endl;

  interceptors.push_back(interceptor);

std::cout << 5 << std::endl;

  return SUCCESS;
}
