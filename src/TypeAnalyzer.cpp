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
                expr.type.base = {"int8"};
            }
            else if (get<uint64_t>(literal) < INT16_MAX)
            {
                expr.type.base = {"int16"};
            }
            else if (get<uint64_t>(literal) < INT32_MAX)
            {
                expr.type.base = {"int32"};
            }
            else 
            {
                expr.type.base = {"int64"};
            }
        break;
        case 3:
            expr.type.base = {"float64"};
        break;
        case 4:
            expr.type.base = {"char"};
        break;
        case 5:
            expr.type.base = {"char"};
            expr.type.level_of_indirection = 1;
            expr.type.is_const = true;
        break;
    }
}



void TypeAnalyzer::TypeAnalyzeBlock(AST::BlockStatement& block)
{
    for (auto& statement : block.statements)
    {
        switch (statement->index()) {
            case 0:
                TypeAnalyzeBlock(get<AST::BlockStatement>(*statement));
            break;
            case 1:
                TypeAnalyzeExpression(get<AST::ReturnStatement>(*statement).value);
            break;
            case 2:
                TypeAnalyzeExpression(get<AST::IfStatement>(*statement).condition);
                TypeAnalyzeBlock(get<AST::IfStatement>(*statement).content);
            break;
            case 3:
                TypeAnalyzeExpression(get<AST::WhileLoop>(*statement).condition);
                TypeAnalyzeBlock(get<AST::WhileLoop>(*statement).content);
            break;
            case 4:
                TypeAnalyzeExpression(get<AST::AssignmentStatement>(*statement).value);
                //TODO check if assigned value type matches variable type
            break;
            case 5:
                TypeAnalyzeFunctionCall(get<AST::FunctionCallExpression>(*statement));
            break;
            case 6:
                TypeAnalyzeExpression(get<AST::VariableDefinition>(*statement).value);
                //TODO check if assigned value type matches variable type
            break;
            case 7:
                ;
            break;
        }
    }
}