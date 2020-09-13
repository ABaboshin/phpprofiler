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
