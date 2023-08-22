#pragma once

#include <string>
#include <optional>
#include <vector>
namespace Tokenizer 
{
enum TokenType
{
    identifier,

    literal_bool,
    literal_integer,
    literal_float,
    literal_char,
    literal_string,

    operator_assignment,
    keyword_var,
    keyword_const,
    keyword_function,
    keyword_class,
    keyword_namespace,

    keyword_do,
    keyword_end,
    keyword_if,
    keyword_else,
    keyword_while,
    keyword_for,
    keyword_in,
    keyword_break,

    operator_plus,
    operator_minus,
    operator_multiply,
    operator_divide,
    operator_modulo,

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
    operator_bitwise_xor,
    operator_shift_left,
    operator_shift_right,

    operator_address,
    operator_pointer,
    operator_reference,
    operator_member_access,
    operator_scope_resolution,
    keyword_new,
    keyword_delete,
    keyword_this,


    //these can't really be handled like operators, because they have stuff inside them and are somewhat ambiguous
    open_parentheses,
    close_parentheses,
    open_bracket,
    close_bracket,
    open_brace,
    close_brace,

    seperator,


    //ones i can think of rn, some of these will take a while to be added
};
class Token
{
    public:
    TokenType m_type;
    std::optional<std::string> m_contents;
    Token(TokenType type, std::optional<std::string> contents);
};

std::vector<Token> TokenizeText(std::string text);

}
