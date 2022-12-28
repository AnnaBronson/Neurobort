#ifndef _CALL_H_
#define _CALL_H_
#include <string>
#include <iostream>
#include "nmdl.h"
#include "nmdl_compiler.h"

using namespace std;

auto Call(NMDL_COMPILER_RESULT result, const string& function_name) {
	switch (result) {
	case NMDL_COMPILER_RESULT_OK:
		return NMDL_RESULT_OK;
	case NMDL_COMPILER_RESULT_MEMORY_ALLOCATION_ERROR:
		throw runtime_error(function_name + ": MEMORY_ALLOCATION_ERROR");
	case NMDL_COMPILER_RESULT_MODEL_LOADING_ERROR:
		throw runtime_error(function_name + ": MODEL_LOADING_ERROR");
	case NMDL_COMPILER_RESULT_INVALID_PARAMETER:
		throw runtime_error(function_name + ": INVALID_PARAMETER");
	case NMDL_COMPILER_RESULT_INVALID_MODEL:
		throw runtime_error(function_name + ": INVALID_MODEL");
	case NMDL_COMPILER_RESULT_UNSUPPORTED_OPERATION:
		throw runtime_error(function_name + ": UNSUPPORTED_OPERATION");
	default:
		throw runtime_error(function_name + ": UNKNOWN ERROR");
	}
}

auto Call(NMDL_RESULT result, const string& function_name) {
	switch (result) {
	case NMDL_RESULT_OK:
		return NMDL_RESULT_OK;
	case NMDL_RESULT_INVALID_FUNC_PARAMETER:
		throw runtime_error(function_name + ": INVALID_FUNC_PARAMETER");
	case NMDL_RESULT_NO_LOAD_LIBRARY:
		throw runtime_error(function_name + ": NO_LOAD_LIBRARY");
	case NMDL_RESULT_NO_BOARD:
		throw runtime_error(function_name + ": NO_BOARD");
	case NMDL_RESULT_BOARD_RESET_ERROR:
		throw runtime_error(function_name + ": BOARD_RESET_ERROR");
	case NMDL_RESULT_INIT_CODE_LOADING_ERROR:
		throw runtime_error(function_name + ": INIT_CODE_LOADING_ERROR");
	case NMDL_RESULT_CORE_HANDLE_RETRIEVAL_ERROR:
		throw runtime_error(function_name + ": CORE_HANDLE_RETRIEVAL_ERROR");
	case NMDL_RESULT_FILE_LOADING_ERROR:
		throw runtime_error(function_name + ": FILE_LOADING_ERROR");
	case NMDL_RESULT_MEMORY_WRITE_ERROR:
		throw runtime_error(function_name + ": MEMORY_WRITE_ERROR");
	case NMDL_RESULT_MEMORY_READ_ERROR:
		throw runtime_error(function_name + ": MEMORY_READ_ERROR");
	case NMDL_RESULT_MEMORY_ALLOCATION_ERROR:
		throw runtime_error(function_name + ": MEMORY_ALLOCATION_ERROR");
	case NMDL_RESULT_MODEL_LOADING_ERROR:
		throw runtime_error(function_name + ": MODEL_LOADING_ERROR");
	case NMDL_RESULT_INVALID_MODEL:
		throw runtime_error(function_name + ": INVALID_MODEL");
	case NMDL_RESULT_BOARD_SYNC_ERROR:
		throw runtime_error(function_name + ": BOARD_SYNC_ERROR");
	case NMDL_RESULT_BOARD_MEMORY_ALLOCATION_ERROR:
		throw runtime_error(function_name + ": BOARD_MEMORY_ALLOCATION_ERROR");
	case NMDL_RESULT_NN_CREATION_ERROR:
		throw runtime_error(function_name + ": NN_CREATION_ERROR");
	case NMDL_RESULT_NN_LOADING_ERROR:
		throw runtime_error(function_name + ": NN_LOADING_ERROR");
	case NMDL_RESULT_NN_INFO_RETRIEVAL_ERROR:
		throw runtime_error(function_name + ": NN_INFO_RETRIEVAL_ERROR");
	case NMDL_RESULT_MODEL_IS_TOO_BIG:
		throw runtime_error(function_name + ": MODEL_IS_TOO_BIG");
	case NMDL_RESULT_NOT_INITIALIZED:
		throw runtime_error(function_name + ": NOT_INITIALIZED");
	case NMDL_RESULT_INCOMPLETE:
		throw runtime_error(function_name + ": INCOMPLETE");
	case NMDL_RESULT_UNKNOWN_ERROR:
		throw runtime_error(function_name + ": UNKNOWN_ERROR");
	default:
		throw runtime_error(function_name + ": UNKNOWN ERROR");
	};
}

#endif