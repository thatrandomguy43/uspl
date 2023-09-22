#include "ASTBuilder.hpp"

class TypeAnalyzer
{
    int16_t current_scope_depth;
    public:
    void TypeAnalyzeBlock(AST::BlockStatement);
};
