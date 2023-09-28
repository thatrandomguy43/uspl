#include "ASTBuilder.hpp"
#include <unordered_map>

class X 
{
    std::vector<X> a;
};

class TypeAnalyzer
{
    uint8_t current_scope_depth = 0;
    std::vector<std::unordered_map<std::string, std::variant<AST::VariableType, AST::FunctionType>>> symbols;
    bool IsTypeConvertable(const AST::VariableType&, const AST::VariableType&);
    std::variant<std::nullopt_t, AST::VariableType, AST::FunctionType> FindTypeOfSymbol(const std::string&);
    void AnalyzeSymbolNameExpression(AST::SymbolNameExpression&);
    void AnalyzeLiteralExpression(AST::LiteralExpression&);
    void AnalyzeUnaryExpression(AST::UnaryExpression&);
    void AnalyzeBinaryExpression(AST::BinaryExpression&);
    void AnalyzeFunctionCall(AST::FunctionCallExpression&);
    void AnalyzeExpression(AST::Expression&);
    void CheckAssignment(AST::AssignmentStatement&);
    void CheckVariableDefinition(AST::VariableDefinition&);
    void AnalyzeStatement(AST::Statement&);
    void AnalyzeFunctionDefinition(AST::FunctionDefinition&);
    public:
    void AnalyzeBlock(AST::BlockStatement&);
};
