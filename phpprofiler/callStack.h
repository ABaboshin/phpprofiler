#pragma once

#include "php.h"
#include "Zend/zend_extensions.h"
#include "Zend/zend_vm.h"
#include "Zend/zend_closures.h"
#include "ext/standard/info.h"

struct CallStack {
  zend_execute_data* data;
  zend_execute_data* call;
  HashTable* interceptors;
  struct CallStack* prev;
  struct CallStack* next;
  int isFirst;
};

typedef struct CallStack CallStack;

CallStack* createNewStack(zend_execute_data* data, zend_execute_data* call, HashTable* interceptors);
CallStack* pushStack(CallStack* stack, zend_execute_data* data, zend_execute_data* call, HashTable* interceptors);
CallStack* popStack(CallStack* stack);
