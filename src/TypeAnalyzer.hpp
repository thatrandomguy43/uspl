#pragma once
#include "ASTBuilder.hpp"
#include <unordered_map>



class TypeAnalyzer
{
    std::string filename;
    std::vector<int16_t> scope;
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
