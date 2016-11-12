#ifndef TELEASSERTION_H
#define TELEASSERTION_H

#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

class TeleAssertion
{
public:
    TeleAssertion(std::string str_assertion);

    bool parse_syntax(std::string str_assertion);
    void send();
    bool syntax_is_valid() {return m_syntax_is_valid;}
private:
    std::string m_function_name;
    std::vector<std::string> m_parameters;
    std::string m_expected_value;

    std::string m_str_assertion;
    bool m_syntax_is_valid;

};

#endif // TELEASSERTION_H
