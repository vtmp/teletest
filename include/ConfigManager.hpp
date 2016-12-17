#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <INIReader.h>
#include <iostream>


class ConfigManager
{
public:
    ConfigManager();
    bool load();

    std::string get_port() {return m_port;}
    int get_baudrate() {return m_baudrate;}
    int data_bits() {return m_data_bits;}
    bool get_parity_bit() {return m_parity_bit;}
    int get_stop_bits() {return m_stop_bits;}
    double get_precision() {return m_precision;}

private:
    std::string m_port;
    int m_baudrate;
    int m_data_bits;
    bool m_parity_bit;
    int m_stop_bits;
    double m_precision;

};

#endif // CONFIGMANAGER_H
