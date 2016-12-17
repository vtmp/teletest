#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <libserialport.h>
#include <iostream>
#include <unistd.h>
#include <string>

#include "ConfigManager.hpp"
#include "Definitions.hpp"
#include "TeleAssertion.hpp"


class SerialInterface
{
public:

    SerialInterface(const ConfigManager &config);

    bool openPort();
    int closePort();

    int requestFunctionInfo();

    bool isOpen() {return m_is_open;}
    void listPorts();

    // TODO move elsewhere
    std::pair<std::string,int> extractFunctionInfo(const std::string& msg);

    std::string teletestAssertion(const TeleAssertion& ta);
    std::string receiveResult(const TeleAssertion& ta);



    void sendMsg(const std::string& msg);
    std::string receiveMsg();

    unsigned int calculateCrc(const std::string& msg);

    // TODO
    MsgType parseMsg(const std::string& msg);

private:
    struct sp_port *m_port_ptr;
    bool m_is_open;
    ConfigManager m_config;


    bool toBool(sp_return return_value);


};

#endif // SERIALINTERFACE_H
