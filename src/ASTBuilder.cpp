#include "ASTBuilder.hpp"
#include "IO.hpp"
#include <iostream>
using namespace AST;
using namespace std;

using ExprValue = variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression, FunctionCallExpression>;
using StatValue = variant<BlockStatement, ReturnStatement, IfStatement, WhileLoop, AssignmentStatement, FunctionCallExpression, VariableDefinition, FunctionDefinition>;

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

VariableType Expression::GetType()
{
    switch (value->index()) 
    {
        case 0:
            return get<AST::SymbolNameExpression>(*value).type;
        break;
        case 1:
            return get<AST::LiteralExpression>(*value).type;
        break;
        case 2:
            return get<AST::LiteralExpression>(*value).type;
        break;
        case 3:
            return get<AST::BinaryExpression>(*value).type;
        break;
        case 4:
            return get<AST::FunctionCallExpression>(*value).type;
        break;
    }
    return {};
}

VariableType ASTBuilder::MakeVariableType()
{
    VariableType type;
    type.is_const = true ? tokens[token_index].type == keyword_const : false;
    token_index++;
    if (token_index >= tokens.size() or tokens[token_index].type != identifier)
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected type name after var or const."});
        return type;
    } 
    else 
    {
        type.base.identifier = get<string>(tokens[token_index].contents);
    }
    token_index++;
    while (tokens[token_index].type == operator_pointer)
    {
        type.level_of_indirection++;
        token_index++;
    }
        
    return type;
}

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
    if (tokens[token_index].type != open_parentheses) 
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
        }
        else if (tokens[token_index].type != close_parentheses)
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
        if (token_index >= tokens.size() or tokens[token_index].type != close_parentheses)
        {
            IO::AddError({filename, tokens[token_index].file_position, "Expected \')\' to close expression."});
            return expr;
        }
        token_index++;
    } 
    else 
    {
        auto simple_expr = MakeSimpleExpression();
        if (simple_expr.index() == 0)
        {
            expr.left_operand.value = make_unique<ExprValue>(get<SymbolNameExpression>(simple_expr));
        }
        else {
            expr.left_operand.value = make_unique<ExprValue>(get<LiteralExpression>(simple_expr));
        }
    }
    expr.operation = BINARY_OPERATORS.at(tokens[token_index].type);
    token_index++;
    if (tokens[token_index].type == open_parentheses)
    {
        token_index++;
        expr.right_operand = MakeExpression();
        if (token_index >= tokens.size() or tokens[token_index].type != close_parentheses)
        {
            IO::AddError({filename, tokens[token_index].file_position, "Expected \')\' to close expression."});
            return expr;
        }
        token_index++;
    } 
    else 
    {
        auto simple_expr = MakeSimpleExpression();
        if (simple_expr.index() == 0)
        {
            expr.right_operand.value = make_unique<ExprValue>(get<SymbolNameExpression>(simple_expr));
        }
        else {
            expr.right_operand.value = make_unique<ExprValue>(get<LiteralExpression>(simple_expr));
        }
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
            IO::AddError({filename, tokens[token_index].file_position, "Expected \')\' to close expression."});
        }
        token_index++;
    } 
    else 
    {
        auto simple_expr = MakeSimpleExpression();
        if (simple_expr.index() == 0)
        {
            expr.operand.value = make_unique<ExprValue>(get<SymbolNameExpression>(simple_expr));
        }
        else {
            expr.operand.value = make_unique<ExprValue>(get<LiteralExpression>(simple_expr));
        }
    }
    return expr;
}

variant<SymbolNameExpression, LiteralExpression> ASTBuilder::MakeSimpleExpression()
{
    variant<SymbolNameExpression, LiteralExpression> expr;

    if (tokens[token_index].type == identifier)
    {
        expr = SymbolNameExpression{};
        get<SymbolNameExpression>(expr).name = get<string>(tokens[token_index].contents);
        token_index++;
    } 
    else if (tokens[token_index].type == literal_value)
    {
        expr = LiteralExpression{nullopt};
        get<LiteralExpression>(expr).value = tokens[token_index].contents;
        token_index++;
    }
    else
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected simple expression (literal or variable name)."});
        token_index++;
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
        expr.value = make_unique<ExprValue>(MakeUnaryExpression());
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
    if (cursor >= tokens.size())
    {
        IO::AddError({filename, tokens.back().file_position, "Expected expression, but reached end of file."});
        return expr;
    }
    if (BINARY_OPERATORS.contains(tokens[cursor].type)) 
    {
        expr.value = make_unique<ExprValue>(MakeBinaryExpression());
    } 
    else if (tokens[cursor].type == open_parentheses)
    {
        expr.value = make_unique<ExprValue>(MakeFunctionCallExpression());
    }
    else
    {
        auto simple_expr = MakeSimpleExpression();
        if (simple_expr.index() == 0)
        {
            expr.value = make_unique<ExprValue>(get<SymbolNameExpression>(simple_expr));
        }
        else {
            expr.value = make_unique<ExprValue>(get<LiteralExpression>(simple_expr));
        }
    }
    return expr;
}

BlockStatement ASTBuilder::MakeBlockStatement()
{
    BlockStatement block;
    if (tokens[token_index].type == keyword_do)
    {
        token_index++;
    }
    else 
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected \'do\' to begin block statement."});
    }

    while (tokens[token_index].type != keyword_end)
    {
        if (token_index >= tokens.size())
        {
            IO::AddError({filename, tokens.back().file_position, "Unexpected end of file inside block."});
            return block;
        }
        switch (tokens[token_index].type) 
        {
            case keyword_var:                                                                                             
            case keyword_const:
                block.statements.push_back(make_unique<StatValue>(MakeVariableDefinition()));
            break;
            case keyword_function:
                block.statements.push_back(make_unique<StatValue>(MakeFunctionDefinition()));
            break;
            case identifier:
                if (token_index + 1 < tokens.size()) 
                {
                    if (tokens[token_index + 1].type == open_parentheses)
                    {
                        block.statements.push_back(make_unique<StatValue>(MakeFunctionCallExpression()));
                    } 
                    else if (tokens[token_index + 1].type == operator_assignment)
                    {
                        block.statements.push_back(make_unique<StatValue>(MakeAssignmentStatement()));
                    }
                    else
                    {
                        IO::AddError({filename, tokens[token_index + 1].file_position, "Identifier is not a valid statement."});
                        token_index++;
                    }
                }
                else 
                {
                    token_index++;
                }
            break;
            case keyword_if:
                block.statements.push_back(make_unique<StatValue>(MakeIfStatement()));
            break;
            case keyword_while:
                block.statements.push_back(make_unique<StatValue>(MakeWhileLoop()));
            break;
            case keyword_return:
                block.statements.push_back(make_unique<StatValue>(MakeReturnStatement()));
            break;
            default:
                IO::AddError({filename, tokens[token_index].file_position, "Keyword does not begin a valid statement."});
                token_index++;
            break;
        }
    }
    token_index++;
    return block;
}

ReturnStatement ASTBuilder::MakeReturnStatement()
{
    ReturnStatement statement;
    token_index++;
    statement.value = MakeExpression();
    return statement;
}

WhileLoop ASTBuilder::MakeWhileLoop()
{
    WhileLoop loop;
    token_index++;
    loop.condition = MakeExpression();
    loop.content = MakeBlockStatement();
    return loop;
}


IfStatement ASTBuilder::MakeIfStatement()
{
    IfStatement statement;
    token_index++;
    statement.condition = MakeExpression();
    statement.content = MakeBlockStatement();
    return statement;
}

AssignmentStatement ASTBuilder::MakeAssignmentStatement()
{
    AssignmentStatement statement;

    statement.target_name = get<string>(tokens[token_index].contents);
    //if we're in this function, we can be sure there was an =, so we dont need to test for it
    token_index += 2;
    if (token_index >= tokens.size())
    {
        IO::AddError({filename, tokens.back().file_position, "Unexpected end of file in assignment."});
    }
    statement.value = MakeExpression();
    return statement;
}

VariableDeclaration ASTBuilder::MakeVariableDeclaration()
{
    VariableDeclaration declaration;
    declaration.type = MakeVariableType();
    if (token_index >= tokens.size() or tokens[token_index].type != identifier)
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected name after variable type."});
        return declaration;
    }
    else 
    {
        declaration.name = get<string>(tokens[token_index].contents);
        token_index++;
    }
    return declaration;
}

VariableDefinition ASTBuilder::MakeVariableDefinition()
{
    VariableDefinition definition;

    definition.declaration = MakeVariableDeclaration();
    if (token_index >= tokens.size() or tokens[token_index].type != operator_assignment)
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected assignment after variable name."});
        token_index++;
        return definition;
    }
    token_index++;
    if (token_index >= tokens.size())
    {
        IO::AddError({filename, tokens.back().file_position, "Unexpected end of file inside variable definition."});
        return definition;
    }
    definition.value = MakeExpression();
    return definition;
}
FunctionDefinition ASTBuilder::MakeFunctionDefinition()
{
    FunctionDefinition definition;
    token_index++;

    if (token_index < tokens.size() and (tokens[token_index].type == keyword_var or tokens[token_index].type == keyword_const)) {
        definition.declation.type.return_type = MakeVariableType();
    }
    else 
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected function return type (must be qualified with either 'var' ot 'const')."});
    }
    if (token_index >= tokens.size())
    {
        IO::AddError({filename, tokens.back().file_position, "Unexpected end of file inside function definition."});
        return definition;
    }
    if (tokens[token_index].type != open_parentheses)
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected \'(\' to open function parameter list"});
    }
    do
    {
        token_index++;
        if (tokens[token_index].type != close_parentheses)
        {
            definition.declation.type.parameters.push_back(MakeVariableDeclaration());
        }
    }
    while (tokens[token_index].type == seperator);
    token_index++;
    if (tokens[token_index].type == identifier) 
    {
        definition.declation.name = get<string>(tokens[token_index].contents);
    }
    else 
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected function name."});
    }

    token_index++;
    definition.body = MakeBlockStatement();

    return definition;
}

void ASTBuilder::BuildFile(TranslationUnit& root, const std::vector<Token>& token_source, const std::string& source_filename)
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
                root.global_scope.statements.push_back(make_unique<StatValue>(MakeVariableDefinition()));
            break;
            case keyword_function:
                root.global_scope.statements.push_back(make_unique<StatValue>(MakeFunctionDefinition()));
            break;
            default:
            IO::AddError({filename, tokens[token_index].file_position, "Token not expected at file scope."});
            token_index++;
            break;
        }
    }
    return;
}