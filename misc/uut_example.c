#include <stdio.h>
#include <string.h>
#include <errno.h>


#include "serial.h"


typedef enum {CHAR, FLOAT, INT} DataType;

const int STR_MAX_LEN = 64;

int parse_msg(const char* msg, int length);
int run_assertion(char *msg);

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


Function functions_under_test[] = {
    {"add", add, 2, {FLOAT, FLOAT}, FLOAT},
    {"sub", sub, 2, {FLOAT, FLOAT}, FLOAT},
    {"mult", mult, 2, {FLOAT, FLOAT}, FLOAT}};


int main()
{
    for(;;)
    {
        // wait for incoming message
        char msg[STR_MAX_LEN];
        while (receive_msg(msg, STR_MAX_LEN));

        // run assertion and send results
        run_assertion(msg);
    }

    return 0;
}

int parse_msg(const char* msg, int length)
{
    // is this needed?

    return 0;
}

int run_assertion(char* msg)
{
    // TODO parse msg
    // run function
    // send return value

    int num_args = 2;
    int fun_id = 1;

    float arg1 = 1.4;
    float arg2 = 1.6;

    float ret_val;

    switch(num_args)
    {
    case 0:
        ret_val = functions_under_test[fun_id].fun_ptr(arg1, arg2);
        break;

    case 2:
        ret_val = functions_under_test[fun_id].fun_ptr(arg1, arg2);
        printf("%f\n", ret_val);
        break;
    }
    return 0;
}
