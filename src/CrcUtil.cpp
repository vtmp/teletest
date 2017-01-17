#include "CrcUtil.hpp"


std::string CrcUtil::append_crc(const std::string &input_msg)
{
    auto crc = this->calculateCrc(input_msg);
    return input_msg + CRC_DELIMITER + std::to_string(crc);
}

std::string CrcUtil::verify_and_split_str(const std::string& input_str)
{
    auto crc_delimiter_idx = input_str.find(CRC_DELIMITER);

    if (crc_delimiter_idx == std::string::npos)
        return "";

    auto msg = input_str.substr(0, crc_delimiter_idx);
    unsigned int crc_actual = stoi(input_str.substr(crc_delimiter_idx+1));
    std::cout << crc_actual << std::endl;

    auto crc_expected = calculateCrc(msg);
    std::cout << msg << "|" << crc_expected << std::endl;

    if (crc_actual != crc_expected)
        return "";

    return msg;
}


unsigned int CrcUtil::calculateCrc(const std::string& msg)
{
    std::vector<unsigned char> buff(msg.begin(), msg.end());

    unsigned char* data = &buff[0];
    unsigned int crc = 0xffff;
    unsigned int length = msg.length();
    unsigned int count;
    unsigned int temp;

    for (count = 0; count < length; ++count)
    {
      temp = (*data++ ^ (crc >> 8)) & 0xff;
      crc = CRC_TABLE[temp] ^ (crc << 8);
    }

    return crc & 0xffff;
}
