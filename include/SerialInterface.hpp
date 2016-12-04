#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <libserialport.h>
#include <iostream>
#include <unistd.h>
#include <string>

class SerialInterface
{
public:
    const int PORTNUM = 0;
    const int BAUDRATE = 115200;
    const char* PORTNAME = "/dev/ttyUSB0";
    const int MAX_ATTEMPTS = 3;
    const int MAX_BUFFER_SIZE = 16;

    SerialInterface();

    bool openPort();
    int closePort();

    int requestFunctionInfo();

    bool isOpen() {return m_is_open;}
    void listPorts();

private:
    struct sp_port *m_port_ptr;
    bool m_is_open;

    bool toBool(sp_return return_value);

};

#endif // SERIALINTERFACE_H
