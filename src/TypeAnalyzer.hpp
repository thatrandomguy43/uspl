#include "ASTBuilder.hpp"

class TypeAnalyzer
{
    int16_t current_scope_depth;
    public:
    std::variant<AST::VariableType, AST::FunctionType> FindTypeOfSymbol(const std::string&);
    void TypeAnalyzeFunctionCall(AST::FunctionCallExpression&);
    void TypeAnalyzeExpression(AST::Expression&);
    void TypeAnalyzeBlock(AST::BlockStatement&);
    void TypeAnalyzeFunctionDefinition(AST::FunctionDefinition&);
};
