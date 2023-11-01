#include "ASTBuilder.hpp"
#include "IO.hpp"
#include <iostream>
using namespace AST;
using namespace std;

size_t Builder::CalcEndPos()
{
    return tokens[token_index-1].file_position + tokens[token_index-1].length - 1;
}

ValueType Builder::MakeValueType()
{
    ValueType type;
    if (token_index >= tokens.size())
    {
        IO::AddError({filename, tokens[token_index].file_position, "Unexpected end of file during type."});
        return type;
    }
    else
    {
        type.is_const = true ? tokens[token_index].type == keyword_const : false;
        if (tokens[token_index].type == keyword_var or type.is_const) 
        {
            token_index++;
        }   
        if (tokens[token_index].type == operator_pointer)
        {
            token_index++;
            type.base_or_pointee_type = make_unique<ValueType>(MakeValueType());
        } 
        else if (tokens[token_index].type == identifier) 
        {
            type.base_or_pointee_type = get<string>(tokens[token_index].contents);
        }
        else
        {
            IO::AddError({filename, tokens[token_index].file_position, "Expected type name here."});
        }
    }
    
    return type;
}

FunctionType Builder::MakeFunctionType(optional<vector<string>*> parameter_name_list)
{
    FunctionType type;
    type.return_type = MakeValueType();
    if (tokens[token_index].type != open_parentheses) 
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected type name here."});
    }
    else 
        token_index++;
    while (tokens[token_index].type != close_parentheses) 
    {
        if (token_index >= tokens.size())
        {
            IO::AddError({filename, tokens[token_index].file_position, "Unexpected end of file during parameter type list."});
            return type;
        }
        type.parameters.push_back(MakeValueType());
        if (parameter_name_list.has_value()) 
        {
            if (tokens[token_index].type != identifier) 
            {
                IO::AddError({filename, tokens[token_index].file_position, "Expected parameter name."});
            }
            else
            {
                parameter_name_list.value()->push_back(get<string>(tokens[token_index].contents));
                token_index++;
            }
        }

        if (tokens[token_index].type == seperator)
            token_index++;
        else if (tokens[token_index].type != close_parentheses)
            IO::AddError({filename, tokens[token_index].file_position, "Expected ',' to seperate parameters."});
    }
    token_index++;
    return type;
};

unique_ptr<FunctionCall> Builder::MakeFunctionCallExpression()
{
    unique_ptr<FunctionCall> expr = make_unique<FunctionCall>();
    expr->start_pos = tokens[token_index].file_position;

    if (tokens[token_index].type == identifier)
    {
        expr->name = get<string>(tokens[token_index].contents);
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
        expr->arguments.push_back(MakeExpression());
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

unique_ptr<BinaryOperation> Builder::MakeBinaryExpression()
{
    unique_ptr<BinaryOperation> expr;

    if (tokens[token_index].type == open_parentheses)
    {
        token_index++;
        expr->left_operand = MakeExpression();
        if (token_index >= tokens.size() or tokens[token_index].type != close_parentheses)
        {
            IO::AddError({filename, tokens[token_index].file_position, "Expected \')\' to close expression."});
            return expr;
        }
        token_index++;
    } 
    else 
    {
        expr->left_operand = MakeSimpleExpression();
    }
    expr->operation = BINARY_OPERATORS.at(tokens[token_index].type);
    token_index++;
    if (tokens[token_index].type == open_parentheses)
    {
        token_index++;
        expr->right_operand = MakeExpression();
        if (token_index >= tokens.size() or tokens[token_index].type != close_parentheses)
        {
            IO::AddError({filename, tokens[token_index].file_position, "Expected \')\' to close expression."});
            return expr;
        }
        token_index++;
    } 
    else 
    {
        expr->right_operand = MakeSimpleExpression();
    }
    return expr;
}
unique_ptr<UnaryOperation> Builder::MakeUnaryExpression()
{
    unique_ptr<UnaryOperation> expr = make_unique<UnaryOperation>(); 
    expr->operation = UNARY_OPERATORS.at(tokens[token_index].type);
    token_index++;
    if (tokens[token_index].type == open_parentheses)
    {
        token_index++;
        expr->operand = MakeExpression();
        if (token_index >= tokens.size() or tokens[token_index].type != close_parentheses)
        {
            IO::AddError({filename, tokens[token_index].file_position, "Expected \')\' to close expression."});
            return expr;
        }
        token_index++;
    } 
    else 
    {
        expr->operand = MakeSimpleExpression();
    }
    return expr;
}

unique_ptr<Expression> Builder::MakeSimpleExpression()
{
    if (tokens[token_index].type == identifier)
    {
        unique_ptr<SymbolExpression> expr = make_unique<SymbolExpression>();
        expr->name = get<string>(tokens[token_index].contents);
        token_index++;
        return expr;
    } 
    else if (tokens[token_index].type == literal_value)
    {
        unique_ptr<LiteralExpression> expr = make_unique<LiteralExpression>();
        switch (tokens[token_index].contents.index()) 
        {
            case 1:
                expr->value = get<bool>(tokens[token_index].contents);
            break;
            case 2:
                expr->value = get<int64_t>(tokens[token_index].contents);
            break;
            case 3:
                expr->value = get<double>(tokens[token_index].contents);
            break;
            case 4:
                expr->value = get<char>(tokens[token_index].contents);
            break;
            case 5:
                expr->value = get<string>(tokens[token_index].contents);
            break;
        }
        token_index++;
        return expr;
    }
    else
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected simple expression (literal or variable name)."});
        token_index++;
        return {};
    }
}

unique_ptr<Expression> Builder::MakeExpression()
{
    unique_ptr<Expression> expr;

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
}

unique_ptr<BlockStatement> Builder::MakeBlockStatement()
{
    unique_ptr<BlockStatement> block = make_unique<BlockStatement>();
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
                block->statements.push_back(MakeVariableDefinition());
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
                       block->statements.push_back(MakeFunctionCallExpression());
                    } 
                    else if (tokens[token_index + 1].type == operator_assignment)
                    {
                        block->statements.push_back(MakeAssignmentStatement());
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
                block->statements.push_back(MakeIfStatement());
            break;
            case keyword_while:
                block->statements.push_back(MakeWhileLoop());
            break;
            case keyword_return:
                block->statements.push_back(MakeReturnStatement());
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

unique_ptr<ReturnStatement> Builder::MakeReturnStatement()
{
    unique_ptr<ReturnStatement> statement = make_unique<ReturnStatement>();
    token_index++;
    statement->value = MakeExpression();
    return statement;
}

unique_ptr<WhileLoop> Builder::MakeWhileLoop()
{
    unique_ptr<WhileLoop> loop = make_unique<WhileLoop>();
    token_index++;
    loop->condition = MakeExpression();
    loop->content = MakeBlockStatement();
    return loop;
}


unique_ptr<IfStatement> Builder::MakeIfStatement()
{
    unique_ptr<IfStatement> statement = make_unique<IfStatement>();
    token_index++;
    statement->condition = MakeExpression();
    statement->content = MakeBlockStatement();
    return statement;
}

unique_ptr<AssignmentStatement> Builder::MakeAssignmentStatement()
{
    unique_ptr<AssignmentStatement> statement = make_unique<AssignmentStatement>();

    statement->symbol = get<string>(tokens[token_index].contents);
    //if we're in this function, we can be sure there was an =, so we dont need to test for it
    token_index += 2;
    if (token_index >= tokens.size())
    {
        IO::AddError({filename, tokens.back().file_position, "Unexpected end of file in assignment."});
    }
    statement->value = MakeExpression();
    return statement;
}


unique_ptr<VariableDefinition> Builder::MakeVariableDefinition()
{
    unique_ptr<VariableDefinition> definition = make_unique<VariableDefinition>();

    definition->type = MakeValueType();
    if (token_index >= tokens.size() or tokens[token_index].type != identifier)
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected name after variable type."});
    } 
    else 
    {
        definition->name = get<string>(tokens[token_index].contents);
    }
    
    if (token_index >= tokens.size() or tokens[token_index].type != operator_assignment)
    {
        IO::AddError({filename, tokens[token_index].file_position, "Expected assignment after variable name."});
    }
    token_index++;
    if (token_index >= tokens.size())
    {
        IO::AddError({filename, tokens.back().file_position, "Unexpected end of file inside variable definition."});
        return definition;
    }
    definition->value = MakeExpression();
    return definition;
}
//what do about parameter name
unique_ptr<FunctionDefinition> Builder::MakeFunctionDefinition()
{
    unique_ptr<FunctionDefinition> definition = make_unique<FunctionDefinition>();
    
    definition->type = MakeFunctionType(&definition->parameter_names);

    if (token_index >= tokens.size())
    {
        IO::AddError({filename, tokens.back().file_position, "Unexpected end of file instead of function body."});
        return definition;
    }
    if (tokens[token_index].type == identifier) 
    {
        definition->name = get<string>(tokens[token_index].contents);
        token_index++;
    }
    else
    {
        IO::AddError({filename, tokens.back().file_position, "Expected function name after function type."});
    }
    definition->body = MakeBlockStatement();

    return definition;
}

TranslationUnit Builder::BuildFile(const std::vector<Token>& token_source, const std::string& source_filename)
{
    TranslationUnit unit;
    token_index = 0;
    tokens = token_source;
    filename = source_filename;
    while (token_index < tokens.size() and tokens[token_index].type != error_token)
    {
        switch (tokens[token_index].type) 
        {
            case keyword_var:                                                                                             
            case keyword_const:
                unit.statements.push_back(MakeVariableDefinition());
            break;
            case keyword_function:
                unit.statements.push_back(MakeFunctionDefinition());
            break;
            default:
            IO::AddError({filename, tokens[token_index].file_position, "Expected symbol declaration (nothing else allowed outside a function)"});
            token_index++;
            break;
        }
    }
    return unit;
}