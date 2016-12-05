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

    // have a list of FunctionInfo.
    // request
    //serial.requestFunctionInfo();
    // while hasNext
    //for (int i=0; i<10; ++i)
    //    auto reply = serial.getLine();

    // ...
    // serial.receiveFunctionInfo

    std::map<std::string, int> testable_functions;
    testable_functions.insert(serial.extractFunctionInfo("add 2"));
    testable_functions.insert(serial.extractFunctionInfo("sub 2"));
    testable_functions.insert(serial.extractFunctionInfo("mult 2"));



    std::string str_assertion_tmp;
    while(std::getline(std::cin, str_assertion_tmp))
    {

        // skip empty lines
        if (str_assertion_tmp.size() == 0)
            continue;

        TeleAssertion ta(str_assertion_tmp);

        if (ta.syntax_is_valid() == false)
        {
            std::cout << "invalid" << std::endl;
            continue;
        }

        // test if ta is in testsable functions
        auto func_elem = testable_functions.find(ta.get_name());
        if (func_elem == testable_functions.end())
        {
            std::cerr << "function " << ta.get_name() << " is not testable" << std::endl;
            continue;
        }

        else if(func_elem->second != ta.get_num_args()) // num args mismatch
        {
            std::cerr << "Assertion for function \"" << ta.get_name() << "\" has " << ta.get_num_args() << " args,";
            std::cerr << " but expects " << func_elem->second << " args." << std::endl;
            continue;
        }

        // at this point everything should be fine, the type will not be checked


        // send assertion to uC
        serial.sendAssertion(ta);

        // receive respond
        auto result_msg = serial.receiveResult(ta);

        // TODO compare results
        std::cout << "actual: " << result_msg << " - expect: " << ta.get_expected_value() << std::endl;

    }

    serial.closePort();
    return 0;
    // TODO print summary

    return 0;
}
