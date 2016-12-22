#ifndef TELETEST_H
#define TELETEST_H

#include <string.h>
#include <stdio.h>

#include "serial.h"
#include "testables.h"

#define STR_MAX_LEN 64
#define MAX_ARGS 5
#define MAX_ARG_LEN 10



typedef enum {FLOAT, INT} DataType;

typedef struct
{
    const char fun_name[10];
    float (*fun_ptr)(float, float); //TODO fixme use general function pointer and cast
    int num_args;
    const DataType type_args[2];
    const  DataType type_ret;
} Function;

typedef struct
{
    float fval;
    float ival;
    int valid;
    DataType type;
} ReturnValue;

typedef struct
{
    char func_name[16];
    char arg_list[MAX_ARGS][MAX_ARG_LEN];
    int num_args;
    int valid;
} AssertionInfo;

int parse_from_assertion_msg(char* msg, AssertionInfo* info_ptr);
int run_assertion(AssertionInfo *info_ptr, ReturnValue *ret_val_ptr);
void parse_to_result_msg(ReturnValue* ret_value_ptr, char* msg);

#endif // TELETEST_H
