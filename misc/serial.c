#include "serial.h"

/*
void SWM_init()
{
    // Enable SWM clock
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);

    // TX in PI0_4
    // RX in PI0_0
    LPC_SWM->PINASSIGN[0] = 0xffff0004UL;	// (p. 127-128)

    // Configure fixed-pin functions
    // These are default values -- could be removed
    LPC_SWM->PINENABLE0 = 0xffffffb3UL;		// (p. 132)

    // Disable SWM clock
    LPC_SYSCON->SYSAHBCLKCTRL &= (~(1<<7));
}

void UART_init(uint32_t baudrate)
{
    uint32_t err, uart_fra_multiplier, baudRateGenerator;
    uint32_t systemCoreClock = Chip_Clock_GetMainClockRate();

    LPC_SYSCON->SYSAHBCLKCTRL |=  (1 << 14);	// Enable clock for UART (p. 37)
    LPC_SYSCON -> PRESETCTRL &= ~(1 << 3);		// Assert UART reset (p. 30)
    LPC_SYSCON -> PRESETCTRL |= (1 << 3);		// Clear UART reset (p. 30)

    // Frame configuration
    LPC_USART0->CFG = UART_CFG_DATALEN_8  | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1 | UART_CFG_ENABLE;

    // Calculate baudrate generator value
    baudRateGenerator = systemCoreClock / (16 * baudrate);
    err = systemCoreClock - baudRateGenerator * 16 * baudrate;
    uart_fra_multiplier = (err * 0xFF) / (baudRateGenerator * 16 * baudrate);
    LPC_USART0->BRG = baudRateGenerator - 1;	// baud rate
    LPC_SYSCON->UARTFRGDIV = 0xFF; // value 0xFF is always used
    LPC_SYSCON->UARTFRGMULT = uart_fra_multiplier;
    LPC_SYSCON->UARTCLKDIV = 1;

    // Clear the status bits
    LPC_USART0->STAT = UART_STAT_CTSDEL | UART_STAT_RXBRKDEL;


}//*/

int receive_char_by_UART()
{
    /*
    while(~LPC_USART0->STAT & UART_STAT_RXRDY);
    while(~LPC_USART0->STAT & UART_STAT_RXIDLE);
    uint32_t data = LPC_USART0->RXDATA;
    return (char) data << 24;
    //*/
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
    // TODO while ~txready something...
    // LPC_USART0->TXDATA = c;
}

void send_msg(char* msg)
{
    // expect correct string
    int length = strlen(msg);

    // ensure that buffer is a valid string
    msg[length-1] = '\0';

    char msg_crc_str[STR_MAX_LEN];
    msg_crc_concat(msg, msg_crc_str);


    int count = 0;
    while (msg_crc_str[count] != '\0')
    {
        send_char_by_UART(msg_crc_str[count++]);
    }

    // terminate msg
    send_char_by_UART('\0');
}

void msg_crc_concat(char* msg_in, char* str_out)
{
    // calculate crc and cast to cstr
    unsigned int crc_actual = calc_crc(msg_in, strlen(msg_in));
    char crc_actual_str[8];
    sprintf(crc_actual_str, "%u", crc_actual);

    // concat msg with crc
    strcpy(str_out, msg_in);
    strcat(str_out, "|");
    strcat(str_out, crc_actual_str);
}
