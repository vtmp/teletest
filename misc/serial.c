#include "serial.h"


int receive_char_by_UART()
{
    // TODO
    return 0;
}

int receive_str_by_UART(char* buffer, int length)
{
    // NOTE without termination character the program could run into a deadlock
    int count = 0;
    char c;

    // read till crc or unexpected null character
    while (count < length-1)
    {
        // wait for incoming character
        c = receive_char_by_UART();

        // save character
        buffer[count++] = c;

        // unexpected end
        if (c == '\0')
            return 1;
    }

    // str reach max len
    buffer[count] = '\0';
    return 0;
}

// expects a string with termination char
int verify_and_split_str(char* str_in, char* msg_out)
{
    char crc_str[CRC_MAX_LEN];

    char* token = strtok(str_in, "|");

    // malformed str
    if (token == NULL)
        return 0;

    // copy msg
    strcpy(msg_out, token);

    token = strtok(NULL, "|");

    // no splitting char found
    if (token == NULL)
        return 0;

    printf("%s\n", token);

    // copy crc str
    strcpy(crc_str, token);


    int crc_value_actual = calc_crc(msg_out, strlen(msg_out));
    int crc_value_expected = atoi(crc_str);

    // compare
    return (crc_value_actual == crc_value_expected);

}


int receive_msg(char* msg_out, int length)
{

    char buffer[length];
    receive_str_by_UART(buffer, length);
    return verify_and_split_str(buffer, msg_out);
}

void send_char_by_UART(char c)
{
    // TODO
}

void send_msg(char* msg)
{
    // expect correct string
    int length = strlen(msg);

    // ensure that buffer is a valid string
    msg[length-1] = '\0';

    // calculate crc
    unsigned int crc_actual = calc_crc(msg, length);
    char crc_actual_str[CRC_MAX_LEN];
    sprintf(crc_actual_str, "%u", crc_actual);

    // append crc to msg
    int new_length = length + CRC_MAX_LEN + 1;
    char msg_crc[new_length];
    msg_crc[0] = '\0';
    strcat(msg_crc, msg);
    strcat(msg_crc, "|");
    strcat(msg_crc, crc_actual_str);

    int count = 0;
    while (count < new_length && msg_crc[count] != '\0')
    {
        send_char_by_UART(msg_crc[count++]);
    }

    // terminate msg
    send_char_by_UART('\0');
}
