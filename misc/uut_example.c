#include "serial.h"

#include "string.h"
#include "stdio.h"

typedef enum {CHAR, FLOAT, INT} DataType;


const int STR_MAX_LEN = 64;

int parse_msg(const char* msg, int length);


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


Function functions_under_test[] = {
    {"add", add, 2, {FLOAT, FLOAT}, FLOAT},
    {"sub", sub, 2, {FLOAT, FLOAT}, FLOAT},
    {"mult", mult, 2, {FLOAT, FLOAT}, FLOAT}};

ReturnValue run_assertion(char *msg);
void send_result(ReturnValue* ret_value_ptr);


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
        ReturnValue ret_val = run_assertion(msg);
        if (ret_val.valid == 0)
        {
            send_msg("ERR1"); // bad assertion
            continue;
        }

        // send results
        send_result(&ret_val);

    }

    return 0;
}

int parse_msg(const char* msg, int length)
{
    // is this needed?

    return 0;
}

// NOT READY
int run_function(char **arg_list, ReturnValue* ret_val_ptr)
{
    // TODO FIXME how can you pass function pointer with unknown arguments
    ret_val_ptr->fval = add(atof(arg_list[0]), atof(arg_list[1]));
    ret_val_ptr->valid;
    ret_val_ptr->type = FLOAT;
}

ReturnValue run_assertion(char* msg)
{
    // TODO parse msg
    // strok
    ReturnValue ret_val;

    char fun_name[20];

    const int MAX_ARGS = 5;
    const int MAX_ARG_LEN = 10;
    char arg_list[MAX_ARGS][MAX_ARG_LEN];


    char* token = strtok(msg, " ");
    if (strcmp(token, "RUN") == 0)
    {
        // get function name, is this save?
        token = strtok(NULL, " ");
        strcpy(fun_name, token);
        token = strtok(NULL, " ");

        // get all arguments
        int count = 0;
        while (token && count < MAX_ARGS)
        {
            strcpy(arg_list[count++], token);
            printf("%s\n", &token);
            token = strtok(NULL, " ");
        }

        //functions_under_test
        // NOT SAFE, TODO FIXME,
        // TODO FIXME no good design
        if (strcmp(fun_name, "add") == 0)
        {
            ret_val.fval = add(atof(arg_list[0]), atof(arg_list[1]));
            ret_val.valid = 1;
            ret_val.type = FLOAT;

        }
        if (strcmp(fun_name, "sub") == 0)
        {
            ret_val.fval = sub(atof(arg_list[0]), atof(arg_list[1]));
            ret_val.valid = 1;
            ret_val.type = FLOAT;
        }
        if (strcmp(fun_name, "mult") == 0)
        {
            ret_val.fval = sub(atof(arg_list[0]), atof(arg_list[1]));
            ret_val.valid = 1;
            ret_val.type = FLOAT;
        }

    }

    return ret_val;
}

void send_result(ReturnValue* ret_value_ptr)
{
    char ret_value_str[16];
    ret_value_str[0] = '\0';

    char result_msg[STR_MAX_LEN];
    strcpy(result_msg, "RET ");

    if (ret_value_ptr->type == FLOAT)
        sprintf(ret_value_str, "%a", ret_value_ptr->fval);

    if (ret_value_ptr->type == INT)
        sprintf(ret_value_str, "%d", ret_value_ptr->ival);

    strcat(result_msg, ret_value_str);

    send_msg(result_msg);
}
