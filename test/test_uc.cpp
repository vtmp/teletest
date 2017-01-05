#include <string>
#include <vector>
#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include <iostream>

#include "crc.h"
#include "crc.c"
#include "serial.h"
#include "serial.c"
#include "teletest.h"
#include "teletest.c" // TODO FIXME this is very ugly! but it solves the linker problem

float result_msg_to_float(char* result_msg);

TEST (teletest_uc, correct_assertion)
{

    char assertion_msg[] = "RUN add 0.1 0.2";
    AssertionInfo info;

    int success = parse_from_assertion_msg(assertion_msg, &info);
    EXPECT_TRUE (success);
    EXPECT_STREQ (info.func_name, "add");
    EXPECT_STREQ (info.arg_list[0], "0.1");
    EXPECT_STREQ (info.arg_list[1], "0.2");

    ReturnValue ret_value;
    success = run_assertion(&info, &ret_value);
    EXPECT_TRUE (success);
    EXPECT_FLOAT_EQ (ret_value.fval, 0.3f);
    EXPECT_EQ (ret_value.type, FLOAT);

    char result_msg[20];
    parse_to_result_msg(&ret_value, result_msg);
    EXPECT_FLOAT_EQ (result_msg_to_float(result_msg), 0.3f);

}

TEST (teletest_uc, crc_correct_check)
{
    char assertion_msg[] = "RUN add 0.1 0.2";
    unsigned int crc_actual = calc_crc(assertion_msg, strlen(assertion_msg));
    char crc_actual_str[CRC_MAX_LEN];
    sprintf(crc_actual_str, "%u", crc_actual);

    char assertion_str[STR_MAX_LEN];

    // TODO create msg_crc_concat function
    strcpy(assertion_str, assertion_msg);
    strcat(assertion_str, "|");
    strcat(assertion_str, crc_actual_str);

    char msg_out[STR_MAX_LEN];

    std::cout << assertion_str << std::endl;

    EXPECT_TRUE(verify_and_split_str(assertion_str, msg_out));
    std::cout << msg_out << std::endl;

    //EXPECT_STREQ(assertion_msg, msg_out);



}

TEST (teletest_uc, bad_message) // ERR1
{
    char assertion_msg[] = "FOO add 0.1 0.2";
    AssertionInfo info;

    int success = parse_from_assertion_msg(assertion_msg, &info);
    EXPECT_FALSE (success);
}

TEST (teletest_uc, not_testable) // ERR2
{
    char assertion_msg[] = "RUN div -0.1 0.2";
    AssertionInfo info;
    strcpy(info.func_name, "div");
    strcpy(info.arg_list[0], "-0.1");
    strcpy(info.arg_list[1], "0.2");

    ReturnValue ret_value;
    int success = run_assertion(&info, &ret_value);
    EXPECT_FALSE (success);
}

TEST (teletest_uc, parse_to_result)
{
    ReturnValue ret_value;
    ret_value.fval = 9.999;
    ret_value.type = FLOAT;

    char result_msg[20];
    parse_to_result_msg(&ret_value, result_msg);
    EXPECT_FLOAT_EQ (result_msg_to_float(result_msg), ret_value.fval);
}

float result_msg_to_float(char* result_msg)
{
    char ret_value_str[16];
    memcpy(ret_value_str, &result_msg[4], 16);
    return atof(ret_value_str);
}
