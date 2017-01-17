#include "TeleAssertion.hpp"

TeleAssertion::TeleAssertion(std::string str_assertion)
{
    m_syntax_is_valid = this->parse_syntax(str_assertion);
}

//TODO write test for this function
bool TeleAssertion::parse_syntax(std::string str_assertion)
{
    std::istringstream iss(str_assertion);
    std::vector<std::string> vec_of_str_assertion(std::istream_iterator<std::string>{iss},
                                                  std::istream_iterator<std::string>{});


    // TODO skip empty lines without complaining
    // TODO allow GROUP and COMMENT keyword?

    int idx_run = 0;
    int idx_with = 2;
    int idx_expects = vec_of_str_assertion.size()-2;

    if (vec_of_str_assertion.size() > 5
            && vec_of_str_assertion[idx_run] == "RUN"
            && vec_of_str_assertion[idx_with] == "WITH"
            && vec_of_str_assertion[idx_expects] == "EXPECT")
    {

        m_function_name = std::string(vec_of_str_assertion[idx_run+1]);

        for (int i=idx_with+1; i<idx_expects; ++i)
            m_args.push_back(vec_of_str_assertion[i]);

        m_expected_value = vec_of_str_assertion[idx_expects+1];
        //std::cout << m_expected_value << std::endl;
    }
    else
    {
        return false;
    }
    // TODO parse

    return true;
}

std::string TeleAssertion::toSerialMsg() const
{
    std::ostringstream oss;
    oss << "RUN " << this->m_function_name;
    for (auto s : this->m_args)
        oss << " " << s;

    return oss.str();
}
