#include "serial.h"


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
