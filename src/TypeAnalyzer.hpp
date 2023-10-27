#pragma once
#include "ASTBuilder.hpp"
#include <utility>

class TypeAnalyzer
{
    std::string filename;
    const AST::Node* global_scope;
    /*a cursor for what scope we're currently looking at, so we can figure out what symbols are in scope there
    each number is a scope within a certain block, so as an example:
    int32 x = 0
    if true do
    int32 a = 0
    while a == 0 do
    int32 b = 0
    a = 1
    >> int32 c = 0 <<
    end
    end
    the highligted statement is in scope [1, 1, 2]
    second statement in global scope, second statement within that block, third statement in that block
    */
    std::vector<uint16_t> scope;
    bool IsTypeConvertable(const AST::Node&, const AST::Node&) const;
    std::optional<AST::Node> FindTypeOfSymbol(const std::string&) const;
    void AnalyzeSymbolNameExpression(AST::Node&);
    void AnalyzeLiteralExpression(AST::Node&);
    void AnalyzeUnaryExpression(AST::Node&);
    void AnalyzeBinaryExpression(AST::Node&);
    void AnalyzeFunctionCall(AST::Node&);
    void AnalyzeExpression(AST::Node&);
    void CheckAssignment(AST::Node&);
    void CheckVariableDefinition(AST::Node&);
    void AnalyzeStatement(AST::Node&);
    void AnalyzeFunctionDefinition(AST::Node&);
    void AnalyzeBlock(AST::Node&);
    public:
    void AnalyzeTranslationUnit(AST::Node&);
};