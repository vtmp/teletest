#include "teletest.h"


Function functions_under_test[] = {
    {"add", add, 2, {FLOAT, FLOAT}, FLOAT},
    {"sub", sub, 2, {FLOAT, FLOAT}, FLOAT},
    {"mult", mult, 2, {FLOAT, FLOAT}, FLOAT}};

int main()
{
    for(;;)
    {
        // wait for incoming message
        char msg[STR_MAX_LEN];
        while (receive_msg(msg, STR_MAX_LEN) == 0)
        {
            send_msg("ERR0"); // error while transmitting
            continue;
        }

        // run assertion
        AssertionInfo info;
        int success = parse_from_assertion_msg(msg, &info);
        if (success == 0)
        {
            send_msg("ERR1"); // bad assertion 1
            continue;
        }

        ReturnValue ret_val;

        success = run_assertion(&info, &ret_val);
        if (success == 0)
        {
            send_msg("ERR2"); // bad assertion 2
            continue;
        }

        // send results
        char result_msg[STR_MAX_LEN];
        parse_to_result_msg(&ret_val, result_msg);
        send_msg(result_msg);

    }

    return 0;
}
