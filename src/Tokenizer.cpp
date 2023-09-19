#include "IO.hpp"
#include "Tokenizer.hpp"
#include <ctype.h>
#include <exception>
#include <map>
#include <iostream>

using namespace std;
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
    {"return", keyword_return},
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


vector<Token> SourceFile::TokenizeText()
{
    vector<Token> token_list;
    while (position < text.size())
    {   

        while (isspace(text[position])) 
        {
            position++;
        }
        Token parsed = TestForToken();
        parsed.file_position = position;
        position += parsed.length;
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

Token SourceFile::TestForToken()
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
        return ProcessTextLiteral();
    }
    if (text[position] >= '0' and text[position] <= '9')
    {
        return ProcessNumberLiteral();
    }
    string next_word = text.substr(position, FindNextWordTerminator(text, position) - position);
    if (KEYWORD_TOKEN_IDS.contains(next_word))
    {
        return Token{nullopt, next_word.length(), KEYWORD_TOKEN_IDS.at(next_word)};
    }
    if (next_word == "false")
    {
        return Token{false, 5, literal_value};
    }
    if (next_word == "true")
    {
        return Token{true, 4, literal_value};
    }


    return Token{next_word, next_word.length(), identifier};
}

pair<string, size_t> SourceFile::EscapeText()
{
    size_t start_pos = position;
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
                    IO::AddError({name, idx, "Invalid escape sequence."});
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

Token SourceFile::ProcessTextLiteral()
{
    pair<string, size_t> escaped = EscapeText();
    if (text[position] == '\'')
    {
        if (escaped.second >= text.length() or text[escaped.second] != '\'')
        {
            IO::AddError({name, position, "Unclosed character literal."});
            return Token{escaped.first, escaped.second - position + 1, literal_value};
        }
        if (escaped.first.empty())
        {
            IO::AddError({name, position, "Empty character literal."});
                return Token{nullopt, 2, error_token};
        }
        if (escaped.first.length() != 1)
        {
            IO::AddError({name, position, "Character literal contains more than 1 character."});
            return Token{nullopt, escaped.second - position + 1, error_token};
        }
        return Token{escaped.first[0], escaped.second - position + 1, literal_value};
    } 
    else if (text[position] == '"')
    {
        if (escaped.second >= text.length() or text[escaped.second] != '\"')
        {
            IO::AddError({name, position, "Unclosed string literal."});
            return Token{escaped.first, escaped.second - position + 1, literal_value};
        }
        return Token{escaped.first, escaped.second - position + 1, literal_value};
    }
    else {
        cout << "Uhh, what happened here? In ProcessTextLiteral despite neither starting with \' or \"" << endl;
    }

    
    return Token{nullopt, 1, error_token};
}

Token SourceFile::ProcessNumberLiteral()
{
    size_t length = 0;
    if (text.substr(position, 2) == "0x")
    {
        double value = 0;
        position += 2;
        try 
        {
            value = stoll(text.substr(position, string::npos), &length, 16);
        }
        catch (exception e)
        {
            IO::AddError({name, position, "Integer literal too THICC (value higher than 2^63 or lower than -2^63)"});
        }
        

        return Token{value, length+2, literal_value};
    } 
    else 
    {
        double float_value = stod(text.substr(position, string::npos), &length);
        optional<uint64_t> int_value = nullopt;
        try 
        {
            int_value = stoll(text.substr(position, string::npos));
        }
        catch (exception e){}
        if (int_value.has_value() and int_value == float_value) 
        {
            return Token{int_value.value(), length, literal_value};
        }
        else 
        {
            return Token{float_value, length, literal_value};
        }
    }
}