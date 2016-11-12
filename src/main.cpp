#include <iostream>

#include "TeleAssertion.hpp"

int main(int argc, char *argv[])
{

    // TODO some cli for passing option, print help, etc ...
    // add option --no-halt for skipping syntax/semantic errors

    std::string str_assertion_tmp;
    while(std::getline(std::cin, str_assertion_tmp))
    {
        // check if syntax is correct
        // and check if semantic is correct (signature of function)

        TeleAssertion ta(str_assertion_tmp);
        if (ta.syntax_is_valid())
        {
            // TODO send package to uC
            // ta.send()

            // TODO receive response, save result and print assertion fails
            // ta.listen()
            // ...
        }
        else
        {
            // increment failed counter

        }

    }

    // TODO print summary

    return 0;
}
