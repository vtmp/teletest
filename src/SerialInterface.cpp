#include "SerialInterface.hpp"

SerialInterface::SerialInterface()
{
    m_port_ptr = nullptr;
    m_is_open = false;
}


bool SerialInterface::openPort()
{

    // find port by name
    sp_get_port_by_name(this->PORTNAME, &m_port_ptr);

    // config and open port
    if (m_port_ptr == NULL)
    {
        std::cerr << "Unable to find port name " << PORTNAME << std::endl;
        return false;
    }
    // use 8-N-1 configuration
    sp_set_bits(m_port_ptr, 8);
    sp_set_parity(m_port_ptr, SP_PARITY_NONE);
    sp_set_stopbits(m_port_ptr, 1);
    sp_set_baudrate(m_port_ptr, 115200);
    sp_set_flowcontrol(m_port_ptr, SP_FLOWCONTROL_NONE);

    auto return_value = sp_open(m_port_ptr, SP_MODE_READ_WRITE);
    m_is_open = this->toBool(return_value);
    return m_is_open;
}

int SerialInterface::requestFunctionInfo()
{
    for (int i=0; i<MAX_ATTEMPTS; ++i)
    {
        sp_event_set *event;
        sp_new_event_set(&event);
        sp_add_port_events(event, m_port_ptr, SP_EVENT_RX_READY);
        void* buffer = malloc(MAX_BUFFER_SIZE);

        for (int j=0; j<100; ++j)
        {
            const char* foo = "foo\n\r\0";

            sp_nonblocking_write(m_port_ptr, foo, sizeof(foo));
            sp_drain(m_port_ptr); //waits till send
            sp_flush(m_port_ptr, SP_BUF_OUTPUT);
            usleep(1e5);
        }

        sp_free_event_set(event);
    }

    return 0;
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
