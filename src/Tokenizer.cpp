#include "IO.hpp"
#include "Tokenizer.hpp"
#include <ctype.h>
#include <exception>
#include <map>
#include <iostream>

using namespace std;
using namespace Tokenizer;
//oh the horror! TWO using namespaces!
const map<string, TokenType> SYMBOL_TOKEN_IDS
{
    {"=", operator_assignment},
    {"+", operator_addition},
    {"-", operator_subtraction},
    {"*", operator_multiplication},
    {"/", operator_division},
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
    {"#",reserved_hash},
    {"!",reserved_exclamation},
    {"?",reserved_questionmark},
    {":",reserved_colon},
    {";",reserved_semicolon},
    
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
        Token parsed = TestForToken(text, current_pos);
        current_pos += parsed.m_length;
        while (isspace(text[current_pos])) 
        {
            current_pos++;
        }
        token_list.push_back(parsed);
    }
    return token_list;
}

size_t FindNextWordTerminator(const string& text, size_t start_pos)
{
    while (start_pos != text.length())
    {
        if (SYMBOL_TOKEN_IDS.contains(text.substr(start_pos, 2)) 
        or SYMBOL_TOKEN_IDS.contains(text.substr(start_pos, 1)) 
        or isspace(text[start_pos]))
        {
            return start_pos;
        }
        start_pos++;
    }
    return start_pos;
}

Token Tokenizer::TestForToken(const string& text, size_t position)
{

    if (SYMBOL_TOKEN_IDS.contains(text.substr(position, 2)))
    {
        return Token{nullopt, 2, SYMBOL_TOKEN_IDS.at(text.substr(position, 2))};
    }

    if (SYMBOL_TOKEN_IDS.contains(text.substr(position, 1))) 
    {
        return Token{nullopt, 1, SYMBOL_TOKEN_IDS.at(text.substr(position, 1))};
    }
    if (text[position] == '"' or text[position] == '\'')
    {
        return ProcessTextLiteral(text, position);
    }
    if (text[position] >= '0' and text[position] <= '9')
    {
        return ProcessNumberLiteral(text, position);
    }
    string next_word = text.substr(position, FindNextWordTerminator(text, position) - position);
    if (KEYWORD_TOKEN_IDS.contains(next_word))
    {
        return Token{nullopt, next_word.length(), KEYWORD_TOKEN_IDS.at(next_word)};
    }
    if (next_word == "false")
    {
        return Token{false, 5, literal_bool};
    }
    if (next_word == "true")
    {
        return Token{true, 4, literal_bool};
    }


    return Token{next_word, next_word.length(), identifier};
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
    size_t length = 0;
    if (text.substr(position, 2) == "0x")
    {
        double value = 0;
        position += 2;
        try 
        {
            value = stoull(text.substr(position, string::npos), &length, 16);
        }
        catch (exception e)
        {
            IO::AddError({IO::current_file, position, "Integer literal too THICC (value higher than 2^64)"});
        }
        

        return Token{value, length+2, literal_integer};
    } 
    else 
    {
        double float_value = stod(text.substr(position, string::npos), &length);
        optional<uint64_t> int_value = nullopt;
        try 
        {
            int_value = stoull(text.substr(position, string::npos));
        }
        catch (exception e){}
        if (int_value.has_value() and int_value == float_value) 
        {
            return Token{int_value.value(), length, literal_integer};
        }
        else 
        {
            return Token{float_value, length, literal_float};
        }
    }
}