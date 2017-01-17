#include <gtest/gtest.h>
#include <iostream>

#include "TeleAssertion.hpp"
#include "SerialInterface.hpp"

// test toStr method
TEST (TestPcClient, valid_to_str)
{

    TeleAssertion ta("RUN foo WITH 1.5 5.1 EXPECT 0");
    EXPECT_TRUE(ta.syntax_is_valid());
    EXPECT_STREQ(ta.toSerialMsg().c_str(), "RUN foo 1.5 5.1");
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

    auto assertion_msg = ta.toSerialMsg();

    CrcUtil util;
    auto crc = util.calculate_crc(assertion_msg);
    std::string msg_crc = assertion_msg + CRC_DELIMITER + std::to_string(crc);

    std::string verified_msg = util.verify_and_split_str(msg_crc);
    EXPECT_STREQ(assertion_msg.c_str(), verified_msg.c_str());
}