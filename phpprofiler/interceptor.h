#pragma once

#include "Zend/zend_types.h"

zend_bool registerInterceptor(zval *className, zval *functionName, zval *interceptorClass);
