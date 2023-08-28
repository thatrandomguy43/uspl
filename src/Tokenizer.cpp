#include "IO.hpp"
#include "Tokenizer.hpp"
#include <map>
#include <optional>
#include <iostream>
#include <vcruntime.h>

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
        return ProcessTextLiteral(text, position);
    }
    if (potential_token >= "0" and potential_token <= "9" )
    {
        string literal_substring = text.substr(position, text.find_first_not_of("0123456789.x", position) - position - 1);
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



pair<string, size_t> EscapeText(const string& text, size_t start_pos)
{
    string escaped_output;
    size_t idx = start_pos + 1;
    while (idx < text.length() and text[idx] != '\'' and text[idx] != '"')
    {
        if (text[idx] == '\\')
        {
            switch (text[idx + 1]) 
            {
                case 'b':
                    escaped_output.append("\b");
                    break;
                case 'f':
                    escaped_output.append("\f");
                    break;
                case 'r':
                    escaped_output.append("\r");
                    break;
                case 'n':
                    escaped_output.append("\n");
                    break;
                case 't':
                    escaped_output.append("\t");
                    break;
                case 'v':
                    escaped_output.append("\v");
                    break;
                case 'z':
                    escaped_output.append("\0");
                    break;                
                case '\'':
                    escaped_output.append("\'");
                    break;
                case '\\':
                    escaped_output.append("\\");
                    break;
                case '\"':
                    escaped_output.append("\\");
                    break;
                case '0':
                    escaped_output.append("\0");
                    break;
                default:
                    IO::AddError({IO::current_file, idx, "Invalid escape sequence."});
            }
            idx += 2;
        } 
        else
        {
            escaped_output.append(text.substr(idx, 1));
            idx++;
        }
    }

    return {escaped_output, idx};
}

//oh good lord no
//this needs extreme fixing

Token Tokenizer::ProcessTextLiteral(const std::string &text, size_t start_pos)
{
    pair<string, size_t> escaped = EscapeText(text, start_pos);
    if (text[start_pos] == '\'')
    {
        if (escaped.second >= text.length() or text[escaped.second] != '\'')
        {
            IO::AddError({IO::current_file, start_pos, "Unclosed character literal."});
            return Token{escaped.first, escaped.second - start_pos + 1, literal_char};
        }
        if (escaped.first.empty())
        {
            IO::AddError({IO::current_file, start_pos, "Empty character literal."});
            return Token{escaped.first, 2, literal_char};
        }
        if (escaped.first.length() != 1)
        {
            IO::AddError({IO::current_file, start_pos, "Character literal contains more than 1 character."});
            return Token{escaped.first, escaped.second - start_pos + 1, literal_char};
        }
        return Token{escaped.first, escaped.second - start_pos + 1, literal_char};
    } 
    else if (text[start_pos] == '"')
    {
        if (escaped.second >= text.length() or text[escaped.second] != '\"')
        {
            IO::AddError({IO::current_file, start_pos, "Unclosed string literal."});
            return Token{escaped.first, escaped.second - start_pos + 1, literal_string};
        }
        return Token{escaped.first, escaped.second - start_pos + 1, literal_string};
    }
    else {
        cout << "Uhh, what happened here? In ProcessTextLiteral despite neither starting with \' or \"" << endl;
    }

    
    return Token{nullopt, 1, error_token};
}

Token Tokenizer::ProcessNumberLiteral(const std::string &text, size_t position)
{
    char* end_address

    return Token{nullopt, 1, error_token};
}