#pragma once

#include <string>
#include <optional>
#include <vector>
namespace Tokenizer 
{
enum TokenType
{
    identifier,

    literal_integer,
    literal_float,
    literal_char,
    literal_string,

    keyword_var,
    keyword_const,
    keyword_function,
    keyword_class,

    keyword_do,
    keyword_end,
    keyword_if,
    keyword_while,
    keyword_for,

    operator_plus,
    operator_minus,
    operator_multiply,
    operator_divide,

    operator_logical_or,
    operator_logical_and,
    operator_logical_not,

    operator_equality,
    operator_inequality,
    operator_lessthan,
    operator_greaterthan,
    operator_less_or_equal,
    operator_greater_or_equal,


    operator_bitwise_or,
    operator_bitwise_and,
    operator_bitwise_not,
    operator_shift_left,
    operator_shift_right

    //ones i can think of rn, some of these will take a while to be added
};
class Token
{
    TokenType m_type;
    std::optional<std::string> m_Contents;
};

std::vector<Token> TokenizeText(std::string text);

}
