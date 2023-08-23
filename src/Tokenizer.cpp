#include "Tokenizer.hpp"
#include <optional>
#include <vector>
#include <map>
#include <variant>
using namespace std;
using namespace Tokenizer;
//oh the horror! TWO using namespaces!
const map<string, TokenType> TOKEN_IDS
{

    {"=", operator_assignment},
    {"var", keyword_var},
    {"const", keyword_const},
    {"function", keyword_function},
    {"class", keyword_class},
    {"namespace", keyword_namespace},

    {"do", keyword_do},
    {"end", keyword_end},
    {"if", keyword_if},
    {"else", keyword_else},
    {"while", keyword_while},
    {"for", keyword_for},
    {"in", keyword_in},
    {"break", keyword_break},

    {"+", operator_plus},
    {"-", operator_minus},
    {"*", operator_multiply},
    {"/", operator_divide},
    {"%", operator_modulo},

    {"or", operator_logical_or},
    {"and", operator_logical_and},
    {"not", operator_logical_not},

/*
there's really no harm in allowing the 2 versions of inequality, less equal and greater equal
it's easy to swap them around accidentally, and there's really no harm in allowing it
i guess maybe the second not equal could be mistaken for bitwise not like this? 
var int16 x=-32678
var int16 y=~x
y == 32767
but i dont think thats a problem 
the approach im gonna use is going to prioritize 2 length operators
*/
    {"==", operator_equality},
    {"!=", operator_inequality},
    {"=!", operator_inequality},
    {"<", operator_lessthan},
    {">", operator_greaterthan},
    {"<=", operator_less_or_equal},
    {"=<", operator_less_or_equal},
    {">=", operator_greater_or_equal},
    {"=>", operator_greater_or_equal},


    {"|", operator_bitwise_or},
    {"&", operator_bitwise_and},
    {"~", operator_bitwise_not},
    {"^", operator_bitwise_xor},
    {"<<", operator_shift_left},
    {">>", operator_shift_right},

    {"@", operator_address},
    {"$", operator_pointer},
    {"§", operator_reference},
    {".", operator_member_access},
    {"::", operator_scope_resolution},
    {"new", keyword_new},
    {"delete", keyword_delete},
    {"this", keyword_this},


    {"(",open_parentheses},
    {")",close_parentheses},
    {"[",open_bracket},
    {"]",close_bracket},
    {"{",open_brace},
    {"}",close_brace},

    {",",seperator},
};

Token::Token(TokenType type, optional<string> contents) : m_type(type), m_contents(contents)
{}



vector<Token> Tokenizer::TokenizeText(const string& text){
    vector<Token> token_list;
    for (size_t current_pos = 0; current_pos < text.size();)
    {
        TestForToken(current_pos, text);
    }
    return token_list;
}

Token Tokenizer::TestForToken(int pos, const string& text){
        string potential_token = text.substr(pos, 2);
        if (TOKEN_IDS.contains(potential_token)){
            return Token{TOKEN_IDS.at(potential_token), nullopt};;
        }
        potential_token = text.substr(pos, 1);
        if (TOKEN_IDS.contains(potential_token)) {
            return Token{TOKEN_IDS.at(potential_token), nullopt};
        }
        if (potential_token == "\""){
            
        }
        return Token{error_token,nullopt};
}