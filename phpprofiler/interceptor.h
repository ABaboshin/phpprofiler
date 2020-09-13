#pragma once

#include "Zend/zend_types.h"

zend_bool registerInterceptor(zval *className, zval *functionName, zval *interceptorClass);
void initInterceptors();

void processUCall(zend_execute_data *data);
void processFCall(zend_execute_data *data);
void processFCallByName(zend_execute_data *data);
void processReturn(zend_execute_data *data);
void processReturnByRef(zend_execute_data *data);
void processException(zend_execute_data *data);
