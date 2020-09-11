#include <iostream>
#include <Zend/zend_compile.h>
#include "Zend/zend_types.h"
#include <php.h>

user_opcode_handler_t orig_icall;
user_opcode_handler_t orig_ucall;
user_opcode_handler_t orig_fcall;
user_opcode_handler_t orig_fcall_by_name;
user_opcode_handler_t orig_return;
user_opcode_handler_t orig_return_by_ref;
user_opcode_handler_t orig_yield;
user_opcode_handler_t orig_yield_from;
user_opcode_handler_t orig_handle_exception;

extern "C" ZEND_HOT int icall(zend_execute_data *data)
{
  if (ZEND_DO_ICALL == data->opline->opcode && data->call->func) {
    std::cout << "ZEND_DO_ICALL " << ZSTR_VAL(data->call->func->common.function_name) << std::endl;
  }

  if (orig_icall) {
    return orig_icall(data);
  } else {
    return ZEND_USER_OPCODE_DISPATCH;
  }
}

extern "C" ZEND_HOT int ucall(zend_execute_data *data)
{
  if (ZEND_DO_UCALL == data->opline->opcode && data->call->func) {
    std::cout << "ZEND_DO_UCALL " << ZSTR_VAL(data->call->func->common.function_name) << std::endl;
  }

  if (orig_ucall) {
    return orig_ucall(data);
  } else {
    return ZEND_USER_OPCODE_DISPATCH;
  }
}

extern "C" ZEND_HOT int fcall(zend_execute_data *data)
{
  if (ZEND_DO_FCALL == data->opline->opcode && data->call->func) {
    std::cout << "ZEND_DO_FCALL " << ZSTR_VAL(data->call->func->common.function_name) << std::endl;
  }

  if (orig_fcall) {
    return orig_fcall(data);
  } else {
    return ZEND_USER_OPCODE_DISPATCH;
  }
}

extern "C" ZEND_HOT int fcall_by_name(zend_execute_data *data)
{
  if (ZEND_DO_FCALL_BY_NAME == data->opline->opcode && data->call->func) {
    std::cout << "ZEND_DO_FCALL_BY_NAME " << ZSTR_VAL(data->call->func->common.function_name) << std::endl;
  }

  if (orig_fcall_by_name) {
    return orig_fcall_by_name(data);
  } else {
    return ZEND_USER_OPCODE_DISPATCH;
  }
}

extern "C" ZEND_HOT int _return(zend_execute_data *data)
{
  if (ZEND_RETURN == data->opline->opcode) {
    std::cout << "ZEND_RETURN " << std::endl;
  }

  if (orig_return) {
    return orig_return(data);
  } else {
    return ZEND_USER_OPCODE_DISPATCH;
  }
}

extern "C" ZEND_HOT int _return_by_ref(zend_execute_data *data)
{
  if (ZEND_RETURN_BY_REF == data->opline->opcode) {
    std::cout << "ZEND_RETURN_BY_REF " << std::endl;
  }

  if (orig_return_by_ref) {
    return orig_return_by_ref(data);
  } else {
    return ZEND_USER_OPCODE_DISPATCH;
  }
}

extern "C" ZEND_HOT int _yield(zend_execute_data *data)
{
  if (ZEND_YIELD == data->opline->opcode) {
    std::cout << "ZEND_YIELD " << std::endl;
  }

  if (orig_yield) {
    return orig_yield(data);
  } else {
    return ZEND_USER_OPCODE_DISPATCH;
  }
}

extern "C" ZEND_HOT int _yield_from(zend_execute_data *data)
{
  if (ZEND_YIELD_FROM == data->opline->opcode) {
    std::cout << "ZEND_YIELD_FROM " << std::endl;
  }

  if (orig_yield_from) {
    return orig_yield_from(data);
  } else {
    return ZEND_USER_OPCODE_DISPATCH;
  }
}

extern "C" ZEND_HOT int handle_exception(zend_execute_data *data)
{
  if (ZEND_HANDLE_EXCEPTION == data->opline->opcode) {
    std::cout << "ZEND_HANDLE_EXCEPTION " << std::endl;
  }

  if (orig_handle_exception) {
    return orig_handle_exception(data);
  } else {
    return ZEND_USER_OPCODE_DISPATCH;
  }
}

extern "C" void intercept_opcodes()
{
  orig_icall = zend_get_user_opcode_handler(ZEND_DO_ICALL);
  orig_ucall = zend_get_user_opcode_handler(ZEND_DO_UCALL);
  orig_fcall = zend_get_user_opcode_handler(ZEND_DO_FCALL);
  orig_fcall_by_name = zend_get_user_opcode_handler(ZEND_DO_FCALL_BY_NAME);
  orig_return = zend_get_user_opcode_handler(ZEND_RETURN);
  orig_return_by_ref = zend_get_user_opcode_handler(ZEND_RETURN_BY_REF);
  orig_yield = zend_get_user_opcode_handler(ZEND_YIELD);
  orig_yield_from = zend_get_user_opcode_handler(ZEND_YIELD_FROM);
  orig_handle_exception = zend_get_user_opcode_handler(ZEND_HANDLE_EXCEPTION);

  zend_set_user_opcode_handler(ZEND_DO_ICALL, icall);
  zend_set_user_opcode_handler(ZEND_DO_UCALL, ucall);
  zend_set_user_opcode_handler(ZEND_DO_FCALL, fcall);
  zend_set_user_opcode_handler(ZEND_DO_FCALL_BY_NAME, fcall_by_name);
  zend_set_user_opcode_handler(ZEND_RETURN, _return);
  zend_set_user_opcode_handler(ZEND_RETURN_BY_REF, _return_by_ref);
  zend_set_user_opcode_handler(ZEND_YIELD, _yield);
  zend_set_user_opcode_handler(ZEND_YIELD_FROM, _yield_from);
  zend_set_user_opcode_handler(ZEND_HANDLE_EXCEPTION, handle_exception);
}
