#include "Tokenizer.hpp"
#include <map>
#include <variant>
#include <vcruntime.h>

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




variant<vector<Token>, TokenizationError> Tokenizer::TokenizeText(const string& text){
    vector<Token> token_list;
    for (size_t current_pos = 0; current_pos < text.size();)
    {
        variant<Token, TokenizationError> parsed = TestForToken(current_pos, text);
        if (holds_alternative<TokenizationError>(parsed))
        {
            return get<TokenizationError>(parsed);
        } 
        else 
        {
            current_pos += get<Token>(parsed).m_length;
        }
    }
    return token_list;
}

variant<Token, TokenizationError> Tokenizer::TestForToken(size_t position, const string& text){
        string potential_token = text.substr(position, 2);
        if (TOKEN_IDS.contains(potential_token)){
            return Token{nullopt, 2, TOKEN_IDS.at(potential_token)};
        }
        potential_token = text.substr(position, 1);
        if (TOKEN_IDS.contains(potential_token)) {
            return Token{nullopt, 1, TOKEN_IDS.at(potential_token)};
        }
        if (potential_token == "\""){
            size_t string_lit_end = text.find('"', position + 1);
            if (string_lit_end == string::npos)
            {
                return TokenizationError{string{""}, position, -1};
            }
            string quoted_contents = text.substr(position + 1, string_lit_end - position - 1);
            Token result{nullopt, string_lit_end - position + 1, literal_string};
            variant<string, TokenizationError> processed = ProcessStringLiteral(text);
            if (holds_alternative<TokenizationError>(processed))
            {
                return get<TokenizationError>(processed);
            }
            result.m_contents = get<string>(processed);
            return result;
        }

        TokenizationError error;
        return error;
}
//placeholder for dealing with escape characters later
variant<string, TokenizationError> Tokenizer::ProcessStringLiteral(const std::string &text){
    return text;
}