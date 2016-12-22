#include <string>
#include <vector>
#include <gtest/gtest.h>
//#include <gmock/gmock.h>

#include "teletest.h"
#include "teletest.c" // TODO FIXME this is very ugly! but it solves the linker problem



TEST (teletest_uc, calc_crc) {

    std::string foo = "RUN add 0.1 0.2";
    std::vector<char> foo2(foo.begin(), foo.end());
    foo2.push_back('\0');

    char* msg = &foo2[0];
    AssertionInfo info;

    // is const char* here ok?
    int success = parse_from_assertion_msg(msg, &info);
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
    char ret_value_str[16];
    parse_to_result_msg(&ret_value, result_msg);
    memcpy(ret_value_str, &result_msg[4], 16);
    EXPECT_FLOAT_EQ (atof(ret_value_str), 0.3f);

} //*/
