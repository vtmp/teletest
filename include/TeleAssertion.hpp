#ifndef TELEASSERTION_H
#define TELEASSERTION_H

#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <sstream>

class TeleAssertion
{
public:
    TeleAssertion(const std::string& str_assertion);

    bool parse_syntax(const std::string& str_assertion);
    std::string to_serial_msg() const;
    bool compare_values(const std::string& return_value_str);

    bool syntax_is_valid() {return m_syntax_is_valid;}
    std::string get_expected_value() {return m_expected_value;}

private:
    std::string m_function_name;
    std::vector<std::string> m_args;
    std::string m_expected_value;

    bool m_syntax_is_valid;
};

#endif // TELEASSERTION_H
