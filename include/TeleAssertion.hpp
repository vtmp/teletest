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
    TeleAssertion(std::string str_assertion);

    bool parse_syntax(std::string str_assertion);
    void send();
    std::string toSerialMsg() const;

    int get_num_args() {return m_args.size();}
    bool syntax_is_valid() {return m_syntax_is_valid;}
    std::string get_name() {return m_function_name;}
    std::string get_expected_value() {return m_expected_value;}

private:
    std::string m_function_name;
    std::vector<std::string> m_args;
    std::vector<char> m_type_args;
    std::string m_expected_value;
    char m_type_ret;

    std::string m_str_assertion;
    bool m_syntax_is_valid;

};

#endif // TELEASSERTION_H
