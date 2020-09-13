#pragma once

#include "Zend/zend_types.h"

#ifndef __cplusplus
typedef struct Profiler Profiler;
#endif

#ifdef __cplusplus
extern "C"
#else
extern
#endif

Profiler* createProfilerInstance();

#ifdef __cplusplus
extern "C"
#else
extern
#endif
zend_bool registerInterceptor(Profiler*, zval *className, zval *functionName, zval *interceptorClass);
