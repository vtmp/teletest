#ifndef TELEASSERTION_H
#define TELEASSERTION_H

#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>

class TeleAssertion
{
public:
    TeleAssertion(const std::string& str_assertion);

    bool parse_syntax(const std::string& str_assertion);
    std::string to_serial_msg() const;
    bool evaluate_assertion_result(const std::string& return_value_str, double precision = 1e5);
    bool compare_values(float actual, float expected, double precision);
    bool compare_values(int actual, int expected);

    bool syntax_is_valid() {return m_syntax_is_valid;}
    std::string get_expected_value() {return m_expected_value;}

private:
    std::string m_assertion_str;
    std::string m_function_name;
    std::vector<std::string> m_args;
    std::string m_expected_value;

    bool m_syntax_is_valid;
};

#endif // TELEASSERTION_H
