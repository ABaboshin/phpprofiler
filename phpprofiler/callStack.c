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
}

void pushStack(CallStack* stack, zend_execute_data* data, HashTable* interceptors)
{
  fprintf(stdout, "pushStack\n");
  CallStack* cs = malloc(sizeof(struct CallStack));
  cs->data = data;
  cs->interceptors = interceptors;
  cs->next = NULL;
  cs->prev = stack;
  cs->isFirst = 0;

  fprintf(stdout, "pushStack2\n");

  stack->next = cs;

  fprintf(stdout, "pushStack3\n");
}
