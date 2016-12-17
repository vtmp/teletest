#include "SerialInterface.hpp"


SerialInterface::SerialInterface(const ConfigManager& config) :
    m_config(config)
{
    m_port_ptr = nullptr;
    m_is_open = false;
}


bool SerialInterface::openPort()
{

    // find port by name
    sp_get_port_by_name(m_config.get_port().c_str(), &m_port_ptr);

    // config and open port
    if (m_port_ptr == NULL)
    {
        std::cerr << "Unable to find port name " << m_config.get_port() << std::endl;
        return false;
    }
    // use 8-N-1 configuration
    sp_set_bits(m_port_ptr, m_config.get_stop_bits());
    sp_set_parity(m_port_ptr, (sp_parity)m_config.get_parity_bit());
    sp_set_stopbits(m_port_ptr, m_config.get_stop_bits());
    sp_set_baudrate(m_port_ptr, m_config.get_baudrate());
    sp_set_flowcontrol(m_port_ptr, SP_FLOWCONTROL_NONE);

    auto return_value = sp_open(m_port_ptr, SP_MODE_READ_WRITE);
    m_is_open = this->toBool(return_value);
    return m_is_open;
}


int SerialInterface::closePort()
{
    auto return_value = sp_close(m_port_ptr);
    return this->toBool(return_value);
}


void SerialInterface::listPorts()
{
    struct sp_port **ports;
    sp_list_ports(&ports);

    for (int i = 0; ports[i]; i++)
        std::cout << sp_get_port_name(ports[i]) << std::endl;
}


std::string  SerialInterface::teletestAssertion(const TeleAssertion& ta)
{
    auto msg = ta.toSerialMsg();

    for (int i=0; i<MAX_ATTEMPTS; ++i)
    {
        this->sendMsg(msg);

        // wait for respondse
        auto response = receiveMsg();

        // try again?
        if (response == "")
            continue;

        // evaluate respond
        auto idx_ws = response.find(' ');
        auto msg_type = response.substr(0,idx_ws);

        if (msg_type == "RET")
            return response.substr(idx_ws+1);

        else if(msg_type == "ERR0") // transmitting error, try again
            continue;

        else if (msg_type == "ERR1") // assertion incorrect, skip
            break;

        // this should not happen
        std::cerr << "received " << msg_type << ", try again" << std::endl;

    }
    std::cerr << "maximum of attempts reached" << std::endl;
    return "";

}


void SerialInterface::sendMsg(const std::string& msg)
{
    // append crc
    auto crc = calculateCrc(msg);
    std::string msg_crc = msg + CRC_DELIMITER + std::to_string(crc);

    sp_nonblocking_write(m_port_ptr, msg_crc.c_str(), msg_crc.size());
    sp_drain(m_port_ptr); //waits till send
    sp_flush(m_port_ptr, SP_BUF_BOTH);
    //usleep(1e5); // some extra time?
}


// returns received msg OR nothing
std::string SerialInterface::receiveMsg()
{
    // setup event
    sp_event_set *event;
    sp_new_event_set(&event);
    sp_add_port_events(event, m_port_ptr, SP_EVENT_RX_READY);
    char buffer[BUFFER_SIZE];

    // read
    sp_wait(event, 1e3);
    if (sp_input_waiting(m_port_ptr) > 0)
    {
        // wait 1 ms
        usleep(1e3);
        sp_nonblocking_read(m_port_ptr, buffer, BUFFER_SIZE);
        std::cout << std::string(buffer) << std::endl;
        sp_flush(m_port_ptr, SP_BUF_INPUT);
    }

    sp_free_event_set(event);

    // check for errors
    auto msg_crc = std::string(buffer);
    auto crc_delimiter_idx = msg_crc.find(CRC_DELIMITER);

    if (crc_delimiter_idx == std::string::npos)
        return "";

    auto msg = msg_crc.substr(0, crc_delimiter_idx);
    unsigned int crc_actual = stoi(msg_crc.substr(crc_delimiter_idx+1));
    std::cout << crc_actual << std::endl;

    auto crc_expected = calculateCrc(msg);
    std::cout << msg << "|" << crc_expected << std::endl;

    if (crc_actual != crc_expected)
        return "";

    return msg;
}


bool SerialInterface::toBool(sp_return return_value)
{
    switch(return_value)
    {

    case SP_OK:
        return true;

    case SP_ERR_ARG:
        std::cerr << "Invalid arguments were passed to the function." << std::endl;
        return false;

    case SP_ERR_FAIL:
        std::cerr << "A system error occurred while executing the operation." << std::endl;
        return false;

    case SP_ERR_MEM:
        std::cerr << "A memory allocation failed while executing the operation." << std::endl;
        return false;

    case SP_ERR_SUPP:
        std::cerr << "The requested operation is not supported by this system or device." << std::endl;
        return false;
    }

    return false;
}


unsigned int SerialInterface::calculateCrc(const std::string& msg)
{
    std::vector<unsigned char> buff(msg.begin(), msg.end());

    unsigned char* data = &buff[0];
    unsigned int crc = 0xffff;
    unsigned int length = msg.length();
    unsigned int count;
    unsigned int temp;

    for (count = 0; count < length; ++count)
    {
      temp = (*data++ ^ (crc >> 8)) & 0xff;
      crc = CRC_TABLE[temp] ^ (crc << 8);
    }

    return crc & 0xffff;
}
