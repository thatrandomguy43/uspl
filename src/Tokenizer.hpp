#pragma once
#include <string>
#include <optional>
#include <vector>
#include <variant>

enum TokenType
{
    identifier,

    literal_value,

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
    keyword_return,

    operator_addition,
    operator_subtraction,
    operator_multiplication,
    operator_division,
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

    reserved_hash,
    reserved_exclamation,
    reserved_questionmark,
    reserved_colon,
    reserved_semicolon,

    error_token

    //ones i can think of rn, some of these will take a while to be added
};

class Token
{

using LiteralExpression = std::variant<std::nullopt_t, bool, uint64_t, double, char, std::string>;
public:
LiteralExpression contents;
size_t length;
size_t file_position;
TokenType type;
Token(LiteralExpression contnt, size_t len, TokenType tp)
: contents(contnt), length(len), type(tp) {}

};

class Tokenizer
{
    
    std::string name;
    std::string text;

    size_t position = 0;

    Token TestForToken();
    Token ProcessTextLiteral();
    Token ProcessNumberLiteral();
    std::pair<std::string, size_t> EscapeText();

    public:
    std::vector<Token> TokenizeText(const std::string& filename, const std::string& filetext);
};