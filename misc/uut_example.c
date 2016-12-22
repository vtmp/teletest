#include "serial.h"

#include "string.h"
#include "stdio.h"

#define STR_MAX_LEN 64
#define MAX_ARGS 5
#define MAX_ARG_LEN 10

typedef enum {CHAR, FLOAT, INT} DataType;

// testable functions
float add(float a, float b) {return a+b;}
float sub(float a, float b) {return a-b;}
float mult(float a, float b) {return a*b;}

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


Function functions_under_test[] = {
    {"add", add, 2, {FLOAT, FLOAT}, FLOAT},
    {"sub", sub, 2, {FLOAT, FLOAT}, FLOAT},
    {"mult", mult, 2, {FLOAT, FLOAT}, FLOAT}};

int parse_from_assertion_msg(char* msg, AssertionInfo* info_ptr);
int run_assertion(AssertionInfo *info_ptr, ReturnValue *ret_val_ptr);
void parse_to_result_msg(ReturnValue* ret_value_ptr, char* msg);


int main()
{
    for(;;)
    {
        // wait for incoming message
        char msg[STR_MAX_LEN];
        while (receive_msg(msg, STR_MAX_LEN) == 0)
        {
            send_msg("ERR0"); // error while transmitting
            continue;
        }

        // run assertion
        AssertionInfo info;
        int success = parse_from_assertion_msg(msg, &info);
        if (success == 0)
        {
            send_msg("ERR1"); // bad assertion 1
            continue;
        }

        ReturnValue ret_val;

        success = run_assertion(&info, &ret_val);
        if (success == 0)
        {
            send_msg("ERR2"); // bad assertion 2
            continue;
        }

        // send results
        char result_msg[STR_MAX_LEN];
        parse_to_result_msg(&ret_val, result_msg);
        send_msg(result_msg);

    }

    return 0;
}

int parse_from_assertion_msg(char* assertion_msg, AssertionInfo *info_ptr)
{

    char* token = strtok(assertion_msg, " ");

    if (strcmp(token, "RUN") != 0)
        return 0;


    // get function name, is this save?
    token = strtok(NULL, " ");
    strcpy(info_ptr->func_name, token);
    token = strtok(NULL, " ");

    // get all arguments
    int count = 0;
    while (token && count < MAX_ARGS)
    {
        strcpy(info_ptr->arg_list[count++], token);
        token = strtok(NULL, " ");
    }

    info_ptr->num_args = count;
    info_ptr->valid = 1;

    return 1;
}

// UNUSED, NOT READY YET
int run_function(char **arg_list, ReturnValue* ret_val_ptr)
{
    // TODO FIXME how can you pass function pointer with unknown arguments
    ret_val_ptr->fval = add(atof(arg_list[0]), atof(arg_list[1]));
    ret_val_ptr->valid;
    ret_val_ptr->type = FLOAT;
}

int run_assertion(AssertionInfo* info_ptr, ReturnValue* ret_val_ptr)
{
    // TODO parse msg
    // strok
    //ReturnValue ret_val;

    if (strcmp(info_ptr->func_name, "add") == 0)
    {
        ret_val_ptr->fval = add(atof(info_ptr->arg_list[0]), atof(info_ptr->arg_list[1]));
        ret_val_ptr->type = FLOAT;
        return 1;
    }
    if (strcmp(info_ptr->func_name, "sub") == 0)
    {
        ret_val_ptr->fval = sub(atof(info_ptr->arg_list[0]), atof(info_ptr->arg_list[1]));
        ret_val_ptr->type = FLOAT;
        return 1;
    }
    if (strcmp(info_ptr->func_name, "mult") == 0)
    {
        ret_val_ptr->fval = sub(atof(info_ptr->arg_list[0]), atof(info_ptr->arg_list[1]));
        ret_val_ptr->type = FLOAT;
        return 1;
    }

    return 0;
}

void parse_to_result_msg(ReturnValue* ret_value_ptr, char* result_msg)
{
    char ret_value_str[16];
    ret_value_str[0] = '\0';

    strcpy(result_msg, "RET ");

    if (ret_value_ptr->type == FLOAT)
        sprintf(ret_value_str, "%a", ret_value_ptr->fval);

    if (ret_value_ptr->type == INT)
        sprintf(ret_value_str, "%d", ret_value_ptr->ival);

    strcat(result_msg, ret_value_str);

}
