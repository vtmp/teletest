#include <iostream>
#include <libserialport.h>
#include <map>

#include "ConfigManager.hpp"
#include "TeleAssertion.hpp"
#include "SerialInterface.hpp"

int main(int argc, char *argv[])
{

    // TODO some cli for passing option, print help, etc ...
    // add option --no-halt for skipping syntax/semantic errors

    ConfigManager config;
    auto conf_loaded = config.load();

    // init serial connection
    SerialInterface serial(config);
    //serial.list_ports();

    // send request
    serial.open_port();
    if (serial.is_open() == false)
        exit(1); // error -> quit


    std::string str_assertion_tmp;
    while(std::getline(std::cin, str_assertion_tmp))
    {

        // skip empty lines
        if (str_assertion_tmp.size() == 0)
            continue;

        TeleAssertion ta(str_assertion_tmp);

        if (ta.syntax_is_valid() == false)
        {
            std::cout << "cannot parse \"" << str_assertion_tmp << "\"" << std::endl;
            continue;
        }


        // send & receive assertion from uC
        auto return_value_str = serial.teletest_assertion(ta);

        // compare results
        ta.evaluate_assertion_result(return_value_str);
    }

    serial.close_port();
    return 0;
}
