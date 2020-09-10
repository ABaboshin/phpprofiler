#pragma once

#include "Zend/zend_types.h"

extern int phpprofiler_resource;

zend_bool register_hook(zval *class_name, zval *function_name, zval *callable);
