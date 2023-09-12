#include "ASTBuilder.hpp"
#include "IO.hpp"
#include <iostream>
using namespace AST;
using namespace std;

const map<TokenType, UnaryOpType> UNARY_OPERATORS
{
   {operator_bitwise_not, AST::bit_not},
   {operator_logical_not, logic_not},
   {operator_pointer, dereference},
   {operator_address, address},
   {operator_subtraction, AST::negation}
};
const map<TokenType, BinaryOpType> BINARY_OPERATORS
{
    {operator_addition, addition},
    {operator_subtraction, subtraction},
    {operator_multiplication, multiplication},
    {operator_division, division},
    {operator_modulo, modulo},
    {operator_bitwise_and, AST::bit_and},
    {operator_bitwise_or, AST::bit_or},
    {operator_bitwise_xor, AST::bit_xor},
    {operator_shift_left, bitshift_left},
    {operator_shift_right, bitshift_right},
    {operator_logical_and, logic_and},
    {operator_logical_or, logic_or},
    {operator_equality, equals},
    {operator_inequality, notequals},
    {operator_lessthan, lessthan},
    {operator_greaterthan, greaterthan},
    {operator_less_or_equal, lessthan_equals},
    {operator_greater_or_equal, greaterthan_equals}
};



FunctionCallExpression ASTBuilder::MakeFunctionCallExpression()
{
    FunctionCallExpression expr;
    if (tokens[token_index].type == identifier)
    {
        expr.identifier = get<string>(tokens[token_index].contents);
    } 
    else 
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected identifier in function call expression"});
    }
    token_index++;
    if (tokens[token_index].type == open_parentheses) 
    {
        cout << "Uhh, what? Why are we here if this token wasn't an opening parentheses? ASTBuilder::MakeFunctionCallExpression" << endl;
    }
    token_index++;
    while (tokens[token_index].type != close_parentheses) 
    {
        expr.args.push_back(MakeExpression());
        if (tokens[token_index].type == seperator)
        {
            token_index++;
        } else 
        {
            IO::AddError({filename, tokens[token_index].file_position, "Missing \',\' between function arguments."});
        }
    }
    token_index++;
    return expr;
}
BinaryExpression ASTBuilder::MakeBinaryExpression()
{
    BinaryExpression expr;
    if (tokens[token_index].type == open_parentheses)
    {
        token_index++;
        expr.left_operand = MakeExpression();
        if (tokens[token_index].type != close_parentheses)
        {
            IO::AddError({filename, tokens[token_index].file_position, "Expected \')\' here to close expression."});
        }
        token_index++;
    } 
    else 
    {
        expr.left_operand = MakeExpression();
    }
    expr.operation = BINARY_OPERATORS.at(tokens[token_index].type);
    token_index++;
    if (tokens[token_index].type == open_parentheses)
    {
        token_index++;
        expr.right_operand = MakeExpression();
        if (tokens[token_index].type != close_parentheses)
        {
            IO::AddError({filename, tokens[token_index].file_position, "Expected \')\' here to close expression."});
        }
        token_index++;
    } 
    else 
    {
        expr.right_operand = MakeExpression();
    }
    return expr;
}
UnaryExpression ASTBuilder::MakeUnaryExpression()
{
    UnaryExpression expr;
    expr.operation = UNARY_OPERATORS.at(tokens[token_index].type);
    token_index++;
    if (tokens[token_index].type == open_parentheses)
    {
        token_index++;
        expr.operand = MakeExpression();
        if (tokens[token_index].type != close_parentheses)
        {
            IO::AddError({filename, tokens[token_index].file_position, "Expected \')\' here to close expression."});
        }
        token_index++;
    } 
    else 
    {
        expr.operand = MakeExpression();
    }
    return expr;
}

Expression ASTBuilder::MakeExpression()
{
    Expression expr;

    if (token_index >= tokens.size())
    {
       IO::AddError({filename, tokens.back().file_position, "Expected expression but got end of file"});
    }

    if (UNARY_OPERATORS.contains(tokens[token_index].type))
    {
        expr.value = make_unique<variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression, FunctionCallExpression>>(MakeUnaryExpression());
        return expr;
    }
    size_t cursor = token_index;
    int16_t parentheses_level = 0;
    switch (tokens[token_index].type)
    {
        case open_parentheses:
            do
            {
                if (tokens[cursor].type == open_parentheses)
                {
                    parentheses_level++;
                }
                else if (tokens[cursor].type == close_parentheses)
                {
                    parentheses_level--;
                }
                cursor++;
            }
            while (parentheses_level != 0 and cursor < tokens.size());
        break;
        case identifier:
        case literal_value:
            cursor++;
        break;
        default:
            IO::AddError({filename, tokens[token_index].file_position, "Expected expression, but location does not contain a valid one."});
        break;
    };
    if (BINARY_OPERATORS.contains(tokens[cursor].type)) 
    {
        expr.value = make_unique<variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression, FunctionCallExpression>>(MakeBinaryExpression());
    } 
    else if (tokens[cursor].type == open_parentheses)
    {
        expr.value = make_unique<variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression, FunctionCallExpression>>(MakeFunctionCallExpression());
    }
    else if (tokens[token_index].type == identifier)
    {
        expr.value = make_unique<variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression, FunctionCallExpression>>(SymbolNameExpression{});
        get<SymbolNameExpression>(*expr.value).name = get<string>(tokens[token_index].contents);
        token_index++;
    } 
    else if (tokens[token_index].type == literal_value)
    {
        expr.value = make_unique<variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression, FunctionCallExpression>>(LiteralExpression{tokens[token_index].contents});
        token_index++;
    }
    return expr;
}

VariableDefinition ASTBuilder::MakeVariableDefinition()
{
    VariableDefinition definition;
    definition.type.is_const = true ? tokens[token_index].type == keyword_const : false;
    token_index++;
    if (tokens[token_index].type != identifier)
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected type name after var or const."});
    } 
    else 
    {
        cout << "ASTBuilder::MakeVariableDefinition says this will need updating to accoount for pointer/reference/array types." << endl;
        definition.type.base.identifier = get<string>(tokens[token_index].contents);
    }
    token_index++;
    if (tokens[token_index].type != identifier)
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected name after variable type."});
    }
    else 
    {
        definition.name = get<string>(tokens[token_index].contents);
    }
    token_index++;
    if (tokens[token_index].type != operator_assignment)
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected assignment after variable name."});
        token_index++;
        definition.value = Expression{};
        return definition;
    }
    token_index++;
    definition.value = MakeExpression();
    return definition;
}

TranslationUnit& ASTBuilder::BuildFile(const std::vector<Token>& token_source, std::string source_filename)
{
    token_index = 0;
    tokens = token_source;
    filename = source_filename;
    while (token_index < tokens.size())
    {
        switch (tokens[token_index].type) 
        {
            case keyword_var:                                                                                             
            case keyword_const:
                root.statements.push_back(make_unique<variant<BlockStatement, AssignmentStatement, VariableDefinition, FunctionDefinition>>(MakeVariableDefinition()));
            break;
            case keyword_function:
            break;
            default:
            IO::AddError({filename, tokens[token_index].file_position, "Token not expected (or, as it is, not implemented yet) at file scope."});
            break;
        }
    }
    return root;
}