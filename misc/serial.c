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
