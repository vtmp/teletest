#include <gtest/gtest.h>
#include <iostream>
#include <stdio.h>

#include "TeleAssertion.hpp"
#include "SerialInterface.hpp"

// test toStr method
TEST (TestPcClient, valid_to_str)
{

    TeleAssertion ta("RUN foo WITH 1.5 5.1 EXPECT 0");
    EXPECT_TRUE(ta.syntax_is_valid());
    EXPECT_STREQ(ta.to_serial_msg().c_str(), "RUN foo 1.5 5.1");
}

// test nonvalid assertions
TEST (TestPcClient, nonvalid_assertions)
{
    TeleAssertion ta1("RUN foo WITH 1.5 5.1 EXPECT");
    TeleAssertion ta2("RUN foo WITH 1.5 5.1");
    TeleAssertion ta3("RUN WITH 1.5 5.1 EXPECT 0");
    TeleAssertion ta4("RUN WITH EXPECT 0");

    EXPECT_FALSE(ta1.syntax_is_valid());
    EXPECT_FALSE(ta2.syntax_is_valid());
    EXPECT_FALSE(ta3.syntax_is_valid());
    EXPECT_FALSE(ta4.syntax_is_valid());
}

// test appending crc
TEST (TestPcClient, valid_crc_append)
{
    TeleAssertion ta("RUN foo WITH 1.5 5.1 EXPECT 0");
    EXPECT_TRUE(ta.syntax_is_valid());

    auto assertion_msg = ta.to_serial_msg();

    CrcUtil util;
    auto crc = util.calculate_crc(assertion_msg);
    std::string msg_crc = assertion_msg + CRC_DELIMITER + std::to_string(crc);

    std::string verified_msg = util.verify_and_split_str(msg_crc);
    EXPECT_STREQ(assertion_msg.c_str(), verified_msg.c_str());
}

// test comparison
TEST (TestPcClient, valid_comparison_int)
{
    TeleAssertion ta("RUN foo WITH 1.5 5.1 EXPECT -12452");
    std::string value_actual_str = "-12452";
    EXPECT_TRUE(ta.evaluate_assertion_result(value_actual_str));
}

TEST (TestPcClient, valid_comparison_float)
{
    TeleAssertion ta("RUN foo WITH 1.5 5.1 EXPECT 0.12345");
    char value_actial_str[32];
    sprintf(value_actial_str, "%a", 0.12346);
    EXPECT_TRUE(ta.evaluate_assertion_result(std::string(value_actial_str), 1e5));
}

TEST (TestPcClient, nonvalid_comparison_int)
{
    TeleAssertion ta("RUN foo WITH 1.5 5.1 EXPECT -12452");
    std::string value_actual_str = "-12451";
    EXPECT_FALSE(ta.evaluate_assertion_result(value_actual_str));
}

TEST (TestPcClient, nonvalid_comparison_float)
{
    TeleAssertion ta("RUN foo WITH 1.5 5.1 EXPECT 0.12345");
    char value_actial_str[32];
    sprintf(value_actial_str, "%a", 0.05);
    EXPECT_FALSE(ta.evaluate_assertion_result(std::string(value_actial_str), 1e3));



}
