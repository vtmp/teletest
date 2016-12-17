#include "ConfigManager.hpp"

ConfigManager::ConfigManager() :
    m_port("/dev/ttyUSB0"),
    m_baudrate(115200),
    m_data_bits(8),
    m_parity_bit(false),
    m_stop_bits(1),
    m_precision(1e3)
{

}

bool ConfigManager::load()
{
    //TODO FIXME this is a linux + unix solution. what is with windows?
    auto conf_path = std::string(std::getenv("HOME")) + "/" + ".TeleTest.conf";

    INIReader config_tmp(conf_path);

    if (config_tmp.ParseError() < 0)
    {
        std::cerr << "could find config file \"~/.teletest.conf\". use default settings..." << std::endl;
        return false;
    }

    m_port = config_tmp.Get("serial", "PORT", "UNKNOWN");
    m_baudrate = config_tmp.GetInteger("serial", "BAUDRATE", -1);
    m_data_bits = config_tmp.GetInteger("serial", "DATA_BITS", -1);
    m_parity_bit = config_tmp.GetBoolean("serial", "PARITY_BIT", false);
    m_stop_bits = config_tmp.GetInteger("serial", "STOP_BITS", -1);

    m_precision = config_tmp.GetReal("assertion", "PRECISION", 0.0);

    if (m_port == "UNKNOWN"
            || m_baudrate == -1
            || m_data_bits == -1
            || m_stop_bits == -1
            || m_precision == 0.0)
    {
        std::cerr << "there are missing values in config" << std::endl;
        return false;
    }

    return true;
}
