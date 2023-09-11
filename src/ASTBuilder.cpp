#include "ASTBuilder.hpp"
#include "IO.hpp"
#include <iostream>
using namespace AST;
using namespace std;


Expression ASTBuilder::MakeExpression()
{
    Expression expr;
    if (tokens[token_index].type == literal_value)
    {

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