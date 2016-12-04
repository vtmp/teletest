#include <stdio.h>
#include <string.h>

typedef enum {CHAR, CSTR, FLOAT, INT} DataType;
//enum DataType {CHAR, CSTR, FLOAT, INT};


typedef struct
{
    const char fun_name[10];
    void* fun_ptr;
    int num_args;
    const DataType type_args[2];
    const  DataType type_ret;
} Function;


typedef struct
{
    int a;
    void* b;
} Foo;


float add(float a, float b) {return a+b;}
float sub(float a, float b) {return a-b;}
float mult(float a, float b) {return a*b;}


Function functions_under_test[] = {
    {"add", (void*)&add, 2, {FLOAT, FLOAT}, FLOAT},
    {"sub", (void*)&sub, 2, {FLOAT, FLOAT}, FLOAT},
    {"mult", (void*)&mult, 2, {FLOAT, FLOAT}, FLOAT}};


int run_assertion(const char* msg);


int main()
{
    // some other init

    for(;;)
    {
        // some other stuff


        // send function info
        //IF request info THEN send info
        // wait
        //IF all send THEN send 'fin'
        // wait for last ACK

        // testing
        if (1) // fake receive message
        {
            // just an example
            const char* msg = "0,23de43df:f,3a4a74ef:f"; // fun-id, arg1, arg2
            run_assertion(msg);
        }


    }
    return 0;
}


int run_assertion(const char* msg)
{
    // TODO parse msg
    // run function
    // send return value
    return 0;
}
