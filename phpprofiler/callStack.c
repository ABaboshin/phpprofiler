#include <stdio.h>
#include "callStack.h"

CallStack* createNewStack(zend_execute_data* data, HashTable* interceptors)
{
  CallStack* cs = malloc(sizeof(struct CallStack));
  cs->data = data;
  cs->interceptors = interceptors;
  cs->next = NULL;
  cs->prev = NULL;
  cs->isFirst = 1;

  return cs;
}

CallStack* pushStack(CallStack* stack, zend_execute_data* data, HashTable* interceptors)
{
  CallStack* cs = malloc(sizeof(struct CallStack));
  cs->data = data;
  cs->interceptors = interceptors;
  cs->next = NULL;
  cs->prev = stack;
  cs->isFirst = 0;

  return cs;
}

CallStack* popStack(CallStack* stack)
{
  CallStack* prev = stack->prev;

  free(stack);

  return prev;
}
