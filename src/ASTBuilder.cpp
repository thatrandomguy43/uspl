#include "ASTBuilder.hpp"
#include "IO.hpp"
#include <iostream>
using namespace AST;
using namespace std;


Expression ASTBuilder::MakeExpression()
{
    Expression expr;

    const set<TokenType> UNARY_OPERATORS
    {
        operator_bitwise_not, 
        operator_logical_not,
        operator_address,
        operator_pointer,
        operator_subtraction
    };

    const set<TokenType> BINARY_OPERATORS
    {
        operator_addition,
        operator_subtraction,
        operator_multiplication,
        operator_division,
        operator_modulo,
        operator_bitwise_and,
        operator_bitwise_or,
        operator_bitwise_xor,
        operator_shift_left,
        operator_shift_right,
        operator_logical_and,
        operator_logical_or,
        operator_equality,
        operator_inequality,
        operator_lessthan,
        operator_greaterthan,
        operator_less_or_equal,
        operator_greater_or_equal
    };
    if (UNARY_OPERATORS.contains(tokens[token_index].type)){
        switch (tokens[token_index].type) 
        {
            case operator_bitwise_not:
                expr.value = make_unique<std::variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression>>(UnaryExpression{});
                get<UnaryExpression>(*expr.value).operation = bit_not;
            break;
            case operator_logical_not:
                expr.value = make_unique<std::variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression>>(UnaryExpression{});
                get<UnaryExpression>(*expr.value).operation = logic_not;
            break;
            case operator_address:
                expr.value = make_unique<std::variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression>>(UnaryExpression{});
                get<UnaryExpression>(*expr.value).operation = address;
            break;
            case operator_pointer:
                expr.value = make_unique<std::variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression>>(UnaryExpression{});
                get<UnaryExpression>(*expr.value).operation = dereference;
            break;
            case operator_subtraction:
                expr.value = make_unique<std::variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression>>(UnaryExpression{});
                get<UnaryExpression>(*expr.value).operation = AST::minus;
            break;
        }
        token_index++;
        if (tokens[token_index].type == open_parentheses)
        {
            token_index++;
            get<UnaryExpression>(*expr.value).operand = MakeExpression();
            if (tokens[token_index].type == close_parentheses)
            {
                IO::AddError({filename, tokens[token_index].file_position, "Expected parentheses to be closed. (Note that chaining multiple operations together without parentheses like 3*4+2-1 iis not supported due to technical difficulties.)"});
            }
        }
        else if (tokens[token_index].type == literal_value)
        {
            LiteralExpression literal = tokens[token_index].contents;
            get<UnaryExpression>(*expr.value).operand.value = make_unique<LiteralExpression>(literal);
        }
        
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
        definition.type.base.id = get<string>(tokens[token_index].contents);
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
    tokens = token_source;
    filename = source_filename;
    while (token_index != tokens.size())
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