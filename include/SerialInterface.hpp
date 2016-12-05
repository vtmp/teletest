#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <libserialport.h>
#include <iostream>
#include <unistd.h>
#include <string>

#include "TeleAssertion.hpp"

class SerialInterface
{
public:
    const int PORTNUM = 0;
    const int BAUDRATE = 115200;
    const char* PORTNAME = "/dev/ttyUSB0";
    const int MAX_ATTEMPTS = 3;
    const int BUFFER_SIZE = 64;

    SerialInterface();

    bool openPort();
    int closePort();

    int requestFunctionInfo();

    bool isOpen() {return m_is_open;}
    void listPorts();

    // TODO move elsewhere
    std::pair<std::string,int> extractFunctionInfo(const std::string& msg);

    void sendAssertion(const TeleAssertion& ta);
    std::string receiveResult(const TeleAssertion& ta);

    std::string getLine();

private:
    struct sp_port *m_port_ptr;
    bool m_is_open;

    bool toBool(sp_return return_value);
    void send(const std::string& msg);


};

#endif // SERIALINTERFACE_H
