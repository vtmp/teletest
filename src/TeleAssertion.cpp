#include "TeleAssertion.hpp"

TeleAssertion::TeleAssertion(const std::string &str_assertion)
{
    m_assertion_str = str_assertion;
    m_syntax_is_valid = this->parse_syntax(str_assertion);
}

//TODO write test for this function
bool TeleAssertion::parse_syntax(const std::string &str_assertion)
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

    return true;
}

std::string TeleAssertion::to_serial_msg() const
{
    std::ostringstream oss;
    oss << "RUN " << this->m_function_name;
    for (auto s : this->m_args)
        oss << " " << s;

    return oss.str();
}

bool TeleAssertion::evaluate_assertion_result(const std::string& return_value_str, double precision)
{
    bool assertion_result = false;
    // float
    if (return_value_str.find('x') != std::string::npos)
    {
        float value_expected = std::stof(m_expected_value);
        float value_actual = std::stof(return_value_str);
        assertion_result = (std::abs(value_expected - value_actual) < 1/precision);
        if (assertion_result == false)
            std::cout << "Assertion \"" << m_assertion_str << "\" failed! " << value_actual << " (actual)"
                      << " != " << value_expected << " (expected)" << std::endl;
    }
    else
    {
        // int
        int value_expected = std::stof(m_expected_value);
        int value_actual = std::stof(return_value_str);
        assertion_result = value_expected == value_actual;
        if (assertion_result == false)
            std::cout << "Assertion \"" << m_assertion_str << "\" failed! " << value_actual << " (actual)"
                      << " != " << value_expected << " (expected)" << std::endl;
    }

    return assertion_result;

}

