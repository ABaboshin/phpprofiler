#include <iostream>
#include "profiler.h"
#include "profiler_c.h"

Profiler* createProfilerInstance()
{
  Profiler* result = new Profiler();
  std::cout << "profiler " << std::hex << result << std::endl;
  return result;
}

void injectLoader(Profiler* profiler)
{
  profiler->injectLoader();
}
