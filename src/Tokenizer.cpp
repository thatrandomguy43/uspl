#include "IO.hpp"
#include "Tokenizer.hpp"
#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <variant>

using namespace std;
using namespace Tokenizer;
//oh the horror! TWO using namespaces!
const map<string, TokenType> SYMBOL_TOKEN_IDS
{
    {"=", operator_assignment},
    {"+", operator_plus},
    {"-", operator_minus},
    {"*", operator_multiply},
    {"/", operator_divide},
    {"%", operator_modulo},
    {"==", operator_equality},
    {"~=", operator_inequality},
    {"<", operator_lessthan},
    {">", operator_greaterthan},
    {"<=", operator_less_or_equal},
    {">=", operator_greater_or_equal},
    {"|", operator_bitwise_or},
    {"&", operator_bitwise_and},
    {"~", operator_bitwise_not},
    {"^", operator_bitwise_xor},
    {"<<", operator_shift_left},
    {">>", operator_shift_right},
    {"@", operator_address},
    {"$", operator_pointer},
    {"£", operator_reference},
    {".", operator_member_access},
    {"::", operator_scope_resolution},
    {"(",open_parentheses},
    {")",close_parentheses},
    {"[",open_bracket},
    {"]",close_bracket},
    {"{",open_brace},
    {"}",close_brace},
    {",",seperator},
};

const map<string, TokenType> KEYWORD_TOKEN_IDS
{
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
    {"or", operator_logical_or},
    {"and", operator_logical_and},
    {"not", operator_logical_not},
    {"new", keyword_new},
    {"delete", keyword_delete},
    {"this", keyword_this},

};




vector<Token> Tokenizer::TokenizeText(const string& text){
    vector<Token> token_list;
    for (size_t current_pos = 0; current_pos < text.size();)
    {
        Token parsed = TestForToken(current_pos, text);
        current_pos += parsed.m_length;
        current_pos = text.find_first_not_of(" \n\r\t\v\f", current_pos);
        token_list.push_back(parsed);
    }
    return token_list;
}

Token Tokenizer::TestForToken(size_t position, const string& text)
{
    string potential_token = text.substr(position, 2);
    if (SYMBOL_TOKEN_IDS.contains(potential_token))
    {
        return Token{nullopt, 2, SYMBOL_TOKEN_IDS.at(potential_token)};
    }
    potential_token = text.substr(position, 1);
    if (SYMBOL_TOKEN_IDS.contains(potential_token)) 
    {
        return Token{nullopt, 1, SYMBOL_TOKEN_IDS.at(potential_token)};
    }
    if (potential_token == "\"" or potential_token == "'" )
    {
        string literal_substring = text.substr(position, text.find_first_of("\"\'", position + 1));
        return ProcessTextLiteral(literal_substring, position);
    }
    if (potential_token >= "0" and potential_token <= "9" )
    {
        string literal_substring = text.substr(position, text.find_first_not_of("0123456789.x", position) - 1);
        return ProcessNumberLiteral(literal_substring, position);
    }
    potential_token = text.substr(position, text.find_first_of(" \n\r\t\v\f", position) - position);
    if (KEYWORD_TOKEN_IDS.contains(potential_token))
    {
        return Token{nullopt, potential_token.length(), KEYWORD_TOKEN_IDS.at(potential_token)};
    }
    if (potential_token == "false")
    {
        return Token{"false", 5, literal_bool};
    }
    if (potential_token == "true")
    {
        return Token{"true", 4, literal_bool};
    }


    return Token{potential_token, potential_token.length(), identifier};
}



string EscapeText(const string& text)
{
    string escaped_output;
    

    return escaped_output;
}

//oh good lord no
//this needs extreme fixing

Token Tokenizer::ProcessTextLiteral(const std::string &text, size_t position)
{
    string escaped = EscapeText(text);

    
    return Token{};
}

Token Tokenizer::ProcessNumberLiteral(const std::string &text, size_t position)
{
    return Token{};
}