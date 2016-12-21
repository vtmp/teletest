#ifndef SERIAL_H
#define SERIAL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "crc.h"

const static int CRC_MAX_LEN = 8;

int receive_char_by_UART();
int receive_msg(char* buffer, int length);
void send_char_by_UART(char c);
void send_msg(char* buffer, int length);


#endif // SERIAL_H
