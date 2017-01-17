#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <libserialport.h>
#include <iostream>
#include <unistd.h>
#include <string>

#include "ConfigManager.hpp"
#include "TeleAssertion.hpp"
#include "CrcUtil.hpp"

const static int BUFFER_SIZE = 64;
const static int MAX_ATTEMPTS = 10;

class SerialInterface
{
public:

    SerialInterface(const ConfigManager &config);

    bool open_port();
    int close_port();

    bool is_open() {return m_is_open;}
    void list_ports();

    std::string teletest_assertion(const TeleAssertion& ta);
    std::string receive_result(const TeleAssertion& ta);

    void send_msg(const std::string& msg);
    std::string receive_str();

private:
    struct sp_port *m_port_ptr;
    bool m_is_open;
    ConfigManager m_config;

    bool to_bool(sp_return return_value);


};

#endif // SERIALINTERFACE_H
