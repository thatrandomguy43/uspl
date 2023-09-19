#include "TypeAnalyzer.hpp"
#include "IO.hpp"
using namespace std;

void LiteralExpressionType(AST::Expression& expr)
{
    AST::LiteralExpression literal = get<AST::LiteralExpression>(*expr.value); 
    switch (get<AST::LiteralExpression>(*expr.value).index()) 
    {
        case 1:
            expr.type.base.identifier = "bool";
        break;
        case 2:
            if (get<uint64_t>(literal) < INT8_MAX)
            {
                expr.type.base.identifier = "int8";
            }
            else if (get<uint64_t>(literal) < INT16_MAX)
            {
                expr.type.base.identifier = "int16";
            }
            else if (get<uint64_t>(literal) < INT32_MAX)
            {
                expr.type.base.identifier = "int32";
            }
            else 
            {
                expr.type.base.identifier = "int64";
            }
        break;
        case 3:
            expr.type.base.identifier = "float64";
        break;
        case 4:
            expr.type.base.identifier = "char";
        break;
        case 5:
            expr.type.base.identifier = "char";
            expr.type.array_dimensions = 1;
            expr.type.is_const = true;
        break;
    }
}

void TypeAnalyzer::TypeAnalyzeBlock(AST::BlockStatement block)
{
    vector<map<string, AST::VariableType>> variable_declarations;
    vector<map<string, AST::FunctionType>> function_declarations;
    int16_t current_scope_depth = 0;


}