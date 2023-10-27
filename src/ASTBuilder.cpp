#include "ASTBuilder.hpp"
#include "IO.hpp"
#include <iostream>
using namespace AST;
using namespace std;


const map<TokenType, string> UNARY_OPERATORS
{
   {operator_bitwise_not, "bitwise_not"},
   {operator_logical_not, "logical_not"},
   {operator_pointer, "dereference"},
   {operator_address, "addressof"},
   {operator_subtraction, "negation"}
};
const map<TokenType, string> BINARY_OPERATORS
{
    {operator_addition, "addition"},
    {operator_subtraction, "subtraction"},
    {operator_multiplication, "multiplication"},
    {operator_division, "division"},
    {operator_modulo, "modulo"},
    {operator_bitwise_and, "bitwise_and"},
    {operator_bitwise_or, "bitwise_or"},
    {operator_bitwise_xor, "bitwise_xor"},
    {operator_shift_left, "bitshift_left"},
    {operator_shift_right, "bitshift_right"},
    {operator_logical_and, "logical_and"},
    {operator_logical_or, "logical_or"},
    {operator_equality, "equals"},
    {operator_inequality, "notequals"},
    {operator_lessthan, "lessthan"},
    {operator_greaterthan, "greaterthan"},
    {operator_less_or_equal, "lessthan_equals"},
    {operator_greater_or_equal, "greaterthan_equals"}
};



Node Builder::MakeType()
{
    Node type;
    type.id = "Type";
    type.properties["category"] = "basic";
    if (token_index >= tokens.size())
    {
        IO::AddError({filename, tokens[token_index].file_position, "Unexpected end of file during type."});
        return type;
    }
    if (tokens[token_index].type == keyword_function)
    {
        type.properties["category"] = "function";
        token_index++;
        if (token_index >= tokens.size())
        {
            IO::AddError({filename, tokens[token_index].file_position, "Unexpected end of file during type."});
            return type;
        }
    }
    else
    {
        type.properties["is_const"] = true ? tokens[token_index].type == keyword_const : false;
        if (tokens[token_index].type != keyword_var and tokens[token_index].type != keyword_const) 
        {
            IO::AddError({filename, tokens[token_index].file_position, "Must specity mustability with 'var' or 'const'. (this is pretty dumb for function return types but techinical difficulties apply)"});

        }
        else
            token_index++;
    
        if (tokens[token_index].type == operator_pointer)
        {
            type.properties["category"] = "pointer";
            token_index++;
            type.properties["pointed_to_type"] = MakeType();
        } 
        else if (tokens[token_index].type == identifier) 
        {
            type.properties["base"] = get<string>(tokens[token_index].contents);
        }
        else
        {
            IO::AddError({filename, tokens[token_index].file_position, "Expected type name here."});
        }
    }
    
    return type;
}

Node Builder::MakeFunctionCallExpression()
{
    Node expr;
    expr.id = "Expression";
    expr.properties["operation_type"] = "function_call";
    expr.properties["arguments"] = Node{"ExpressionList", {}};

    if (tokens[token_index].type == identifier)
    {
        expr.properties["identifier"] = get<string>(tokens[token_index].contents);
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
    int16_t argument_idx = 0;
    while (tokens[token_index].type != close_parentheses) 
    {
        get<Node>(expr.properties["arguments"]).properties[argument_idx] = MakeExpression();
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

Node Builder::MakeBinaryExpression()
{
    Node expr;
    expr.id = "Expression";
    expr.properties["operation_type"] = "binary";
    if (tokens[token_index].type == open_parentheses)
    {
        token_index++;
        expr.properties["left_operand"] = MakeExpression();
        if (token_index >= tokens.size() or tokens[token_index].type != close_parentheses)
        {
            IO::AddError({filename, tokens[token_index].file_position, "Expected \')\' to close expression."});
            return expr;
        }
        token_index++;
    } 
    else 
    {
        expr.properties["left_operand"] = MakeSimpleExpression();
    }
    expr.properties["operation"] = BINARY_OPERATORS.at(tokens[token_index].type);
    token_index++;
    if (tokens[token_index].type == open_parentheses)
    {
        token_index++;
        expr.properties["right_operand"] = MakeExpression();
        if (token_index >= tokens.size() or tokens[token_index].type != close_parentheses)
        {
            IO::AddError({filename, tokens[token_index].file_position, "Expected \')\' to close expression."});
            return expr;
        }
        token_index++;
    } 
    else 
    {
        expr.properties["right_operand"] = MakeSimpleExpression();
    }
    return expr;
}
Node Builder::MakeUnaryExpression()
{
    Node expr;
    expr.id = "Expression";
    expr.properties["operation_type"] = "unary";
    expr.properties["operation"] = BINARY_OPERATORS.at(tokens[token_index].type);
    token_index++;
    if (tokens[token_index].type == open_parentheses)
    {
        token_index++;
        expr.properties["operand"] = MakeExpression();
        if (token_index >= tokens.size() or tokens[token_index].type != close_parentheses)
        {
            IO::AddError({filename, tokens[token_index].file_position, "Expected \')\' to close expression."});
            return expr;
        }
        token_index++;
    } 
    else 
    {
        expr.properties["operand"] = MakeSimpleExpression();
    }
    return expr;
}

Node Builder::MakeSimpleExpression()
{
    Node expr;
    expr.id = "Expression";
    if (tokens[token_index].type == identifier)
    {
        expr.properties["operation_type"] = "symbol";
        expr.properties["identifier"] = get<string>(tokens[token_index].contents);
        token_index++;
    } 
    else if (tokens[token_index].type == literal_value)
    {   
        switch (tokens[token_index].contents.index()) 
        {
            case 1:
                expr.properties["value"] = get<bool>(tokens[token_index].contents);
                expr.properties["operation_type"] = "literal_bool";
            break;
            case 2:
                expr.properties["value"] = get<int64_t>(tokens[token_index].contents);
                expr.properties["operation_type"] = "literal_integer";
            break;
            case 3:
                expr.properties["value"] = get<double>(tokens[token_index].contents);
                expr.properties["operation_type"] = "literal_float";
            break;
            case 4:
                expr.properties["value"] = string{get<char>(tokens[token_index].contents)};
                expr.properties["operation_type"] = "literal_char";
            break;
            case 5:
                expr.properties["value"] = get<string>(tokens[token_index].contents);
                expr.properties["operation_type"] = "literal_string";
            break;
        }
        token_index++;
    }
    else
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected simple expression (literal or variable name)."});
        token_index++;
    }
    return expr;
}

Node Builder::MakeExpression()
{
    Node expr;

    if (token_index >= tokens.size())
    {
       IO::AddError({filename, tokens.back().file_position, "Expected expression but got end of file"});
    }

    if (UNARY_OPERATORS.contains(tokens[token_index].type))
    {
        return MakeUnaryExpression();
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
        return MakeBinaryExpression();
    } 
    else if (tokens[cursor].type == open_parentheses)
    {
        return MakeFunctionCallExpression();
    }
    else
    {
       return MakeSimpleExpression();
    }
    return expr;
}

Node Builder::MakeBlockStatement()
{
    Node block;
    block.id = "BlockStatement";
    if (tokens[token_index].type == keyword_do)
    {
        token_index++;
    }
    else 
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected \'do\' to begin block statement."});
    }
    for (int statement_idx = 0; tokens[token_index].type != keyword_end; statement_idx++)
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
                block.properties[statement_idx] = MakeVariableDefinition();
            break;
            case keyword_function:
                IO::AddError({filename, tokens[token_index].file_position, "Function definition inside function definition not allowed."});
                token_index++;
            break;
            case identifier:
                if (token_index + 1 < tokens.size()) 
                {
                    if (tokens[token_index + 1].type == open_parentheses)
                    {
                       block.properties[statement_idx] = MakeFunctionCallExpression();
                    } 
                    else if (tokens[token_index + 1].type == operator_assignment)
                    {
                        block.properties[statement_idx] = MakeAssignmentStatement();
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
                block.properties[statement_idx] = MakeIfStatement();
            break;
            case keyword_while:
                block.properties[statement_idx] = MakeWhileLoop();
            break;
            case keyword_return:
                block.properties[statement_idx] = MakeReturnStatement();
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

Node Builder::MakeReturnStatement()
{
    Node statement;
    statement.id = "ReturnStatement";
    token_index++;
    statement.properties["value"] = MakeExpression();
    return statement;
}

Node Builder::MakeWhileLoop()
{
    Node loop;
    loop.id = "WhileLoop";
    token_index++;
    loop.properties["condition"] = MakeExpression();
    loop.properties["content"] = MakeBlockStatement();
    return loop;
}


Node Builder::MakeIfStatement()
{
    Node statement;
    statement.id = "IfStatement";
    token_index++;
    statement.properties["condition"] = MakeExpression();
    statement.properties["content"] = MakeBlockStatement();
    return statement;
}

Node Builder::MakeAssignmentStatement()
{
    Node statement;

    statement.properties["target"] = get<string>(tokens[token_index].contents);
    //if we're in this function, we can be sure there was an =, so we dont need to test for it
    token_index += 2;
    if (token_index >= tokens.size())
    {
        IO::AddError({filename, tokens.back().file_position, "Unexpected end of file in assignment."});
    }
    statement.properties["value"] = MakeExpression();
    return statement;
}

Node Builder::MakeSymbolDeclaration()
{
    Node declaration;
    declaration.id = "SymbolDeclaration";
    declaration.properties["type"] = MakeType();
    if (token_index >= tokens.size() or tokens[token_index].type != identifier)
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected name after type."});
        return declaration;
    }
    else 
    {
        declaration.properties["name"] = get<string>(tokens[token_index].contents);
        token_index++;
    }
    return declaration;
}

Node Builder::MakeVariableDefinition()
{
    Node definition;
    definition.id = "VariableDefinition";

    definition.properties["declaration"] = MakeSymbolDeclaration();
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
    definition.properties["value"] = MakeExpression();
    return definition;
}
Node Builder::MakeFunctionDefinition()
{
    Node definition;
    definition.id = "FunctionDefinition";
    
    definition.properties["declaration"] = MakeSymbolDeclaration();

    if (token_index >= tokens.size())
    {
        IO::AddError({filename, tokens.back().file_position, "Unexpected end of file instead of function body."});
        return definition;
    }

    definition.properties["body"] = MakeBlockStatement();

    return definition;
}

void Builder::BuildFile(Node& root, const std::vector<Token>& token_source, const std::string& source_filename)
{
    token_index = 0;
    tokens = token_source;
    filename = source_filename;
    int statement_idx = 0;
    while (token_index < tokens.size() and tokens[token_index].type != error_token)
    {
        switch (tokens[token_index].type) 
        {
            case keyword_var:                                                                                             
            case keyword_const:
                root.properties[statement_idx] = MakeVariableDefinition();
            break;
            case keyword_function:
                root.properties[statement_idx] = MakeFunctionDefinition();
            break;
            default:
            IO::AddError({filename, tokens[token_index].file_position, "Expected symbol declaration (nothing else allowed outside a function)"});
            token_index++;
            break;
        }
        statement_idx++;
    }
    return;
}