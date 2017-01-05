#ifndef SERIAL_H
#define SERIAL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "crc.h"

const static int CRC_MAX_LEN = 8;
const static int STR_MAX_LEN = 32;

int receive_char_by_UART();
int receive_str_by_UART(char* buffer, int length);
int verify_and_split_str(char* str_in, char* msg_out);
int receive_msg(char* msg_out, int length);

void send_char_by_UART(char c);
void send_msg(char* msg);

void msg_crc_concat(char* msg_in, char* str_out);

#endif // SERIAL_H
