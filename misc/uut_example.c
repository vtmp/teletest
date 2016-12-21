#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "crc.h"

typedef enum {CHAR, FLOAT, INT} DataType;

const int CRC_MAX_LEN = 8;
const int STR_MAX_LEN = 64;

int receive_char_by_UART();
int receive_msg(char* buffer, int length);
int send_char_by_UART(char c);
void send_msg(char* buffer, int length);
int parse_msg(const char* msg, int length);
int run_assertion(const char* msg);

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


int receive_char_by_UART()
{
    // TODO
    return 0;
}

int receive_msg(char* buffer, int length)
{
    // NOTE without termination character the program runs into a deadlock!!!
    char crc_str[CRC_MAX_LEN];
    int crc_count = 0;

    int count = 0;
    char c;


    // read till crc or unexpected null character
    while (count < length-1)
    {
        // wait for incoming character
        c = receive_char_by_UART();

        // crc is starting now
        if (c == '|')
            break;

        // save character
        buffer[count++] = c;

        // unexpected end
        if (c == '\0')
            return 0;
    }
    buffer[count] = '\0';

    while (crc_count < 8)
    {
        c = receive_char_by_UART();

        // save character
        crc_str[crc_count++] = c;

        // expected end
        if (c == '\0')
            break;

    }

    // calculate the crc
    int crc_value_actual = calc_crc(buffer, count);
    int crc_value_expected = atoi(crc_str);

    // compare
    return (crc_value_actual == crc_value_expected);
}

int send_char_by_UART(char c)
{
    // TODO
    return 0;
}

void send_msg(char* buffer, int length)
{
    // ensure that buffer is a valid string
    buffer[length-1] = '\0';

    // calculate crc
    unsigned int crc_actual = calc_crc(buffer, length);
    char crc_actual_str[CRC_MAX_LEN];
    sprintf(crc_actual_str, "%ld", crc_actual);

    // append crc to msg
    int new_length = length + CRC_MAX_LEN + 1;
    char msg[new_length];
    msg[0] = '\0';
    strcat(msg, buffer);
    strcat(msg, "|");
    strcat(msg, crc_actual_str);

    int count = 0;
    while (count < new_length && msg[count] != '\0')
    {
        send_char_by_UART(msg[count++]);
    }

    // terminate msg
    send_char_by_UART('\0');
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
