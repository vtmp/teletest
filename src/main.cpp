#include <iostream>
#include <libserialport.h>
#include <map>

#include "TeleAssertion.hpp"
#include "SerialInterface.hpp"
#include "FunctionInfo.hpp"

int main(int argc, char *argv[])
{

    // TODO some cli for passing option, print help, etc ...
    // add option --no-halt for skipping syntax/semantic errors

    // init serial connection
    SerialInterface serial;
    //serial.list_ports();

    // send request
    serial.openPort();
    if (serial.isOpen() == false)
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
        auto result_str = serial.teletestAssertion(ta);

        // compare results
        // TODO cast to float or int and compare
        std::cout << "actual: " << result_str << " - expect: " << ta.get_expected_value() << std::endl;

    }

    serial.closePort();

    // TODO print summary

    return 0;
}
