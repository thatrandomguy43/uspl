#include "ASTBuilder.hpp"
#include "IO.hpp"
using namespace AST;
using namespace std;



AST::TranslationUnit& ASTBuilder::BuildFile(const std::vector<Token>& token_source, std::string source_filename)
{
    tokens = token_source;
    filename = source_filename;
    while (token_index != tokens.size())
    {
        switch (tokens[token_index].type) 
        {
            case keyword_var:
                root.statements.push_back({});
                root.statements.back().type = make_unique<variant<BlockStatement, AssignmentStatement, VariableDefinition, FunctionDefinition>>();
            break;
            case keyword_const:
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