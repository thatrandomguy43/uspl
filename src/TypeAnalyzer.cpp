#include "TypeAnalyzer.hpp"
#include "IO.hpp"
using namespace std;

bool TypeAnalyzer::IsTypeConvertable(const AST::VariableType& to, const AST::VariableType& from)
{
    const set<AST::UnqualifiedType> INTEGER_TYPES = {
        {"int8"},
        {"int16"},
        {"int32"},
        {"int64"},
    };

    const set<AST::UnqualifiedType> FLOAT_TYPES = {
        {"float32"},
        {"float64"},
    };

    if (to.level_of_indirection == 0 and from.level_of_indirection == 0)
    {
        if (INTEGER_TYPES.contains(from.base))
        {
            if (INTEGER_TYPES.contains(to.base)) 
            {
                return true;
            }
            else if (FLOAT_TYPES.contains(to.base))
            {
                return true;
            }
        }
    }
    //i dont really want default conversions to bool, people can be tripped up by truthy/falsy easily
    return false;
}

void TypeAnalyzer::AnalyzeSymbolNameExpression(AST::SymbolNameExpression& expr)
{
    variant<nullopt_t, AST::VariableType, AST::FunctionType> type = FindTypeOfSymbol(expr.name);
    if (type.index() == 1) 
    {
        expr.type = get<AST::VariableType>(type);
    }
    else if (type.index() == 2)
    {
        IO::AddError({"Placeholder filename", 0, "'" + expr.name + "' is a function, and cannot be used as an expression on its own. Functions must be called to be an expression. (as it is in this currently function pointer-less language)"});
    }
    else 
    {
         IO::AddError({"Placeholder filename", 0, "Use of undeclared identifier '" + expr.name + "'"});
    }
}

void TypeAnalyzer::AnalyzeLiteralExpression(AST::LiteralExpression& literal)
{
    switch (literal.value.index()) 
    {
        case 1:
            literal.type.base = {"bool"};
        break;
        case 2:
            literal.type.base = {"int64"};
        break;
        case 3:
            literal.type.base = {"float64"};
        break;
        case 4:
            literal.type.base = {"char"};
        break;
        case 5:
            literal.type.base = {"char"};
            literal.type.level_of_indirection = 1;
            literal.type.is_const = true;
        break;
    }
}

void TypeAnalyzer::AnalyzeUnaryExpression(AST::UnaryExpression& expr)
{
    AnalyzeExpression(expr.operand);
    switch (expr.operation) {
        case AST::negation:
            if (IsTypeConvertable({"int64"}, expr.operand.GetType())) 
            {
                expr.type = {"int64"};
            }
            else if (IsTypeConvertable({"float64"}, expr.operand.GetType()))
            {
                expr.type = {"float64"};
            }
            else 
            {
                IO::AddError({"Placeholder file", 0, "Cannot perform negation on non-numerical type " + expr.operand.GetType().base.identifier + "."} );
            }
        break;
        case AST::bit_not:
            if (IsTypeConvertable({"int64"}, expr.operand.GetType())) 
            {
                expr.type = {"int64"};
            }
            else 
            {
                IO::AddError({"Placeholder file", 0, "Cannot perform bitwise NOT on non-integer type " + expr.operand.GetType().base.identifier + "."} );
            }
        break;
        case AST::logic_not:
            if (IsTypeConvertable({"bool"}, expr.operand.GetType())) 
            {
                expr.type = {"bool"};
            }
            else 
            {
                IO::AddError({"Placeholder file", 0, "Cannot perform logical NOT on non-boolean type " + expr.operand.GetType().base.identifier + "."} );
            }      
        break;
        case AST::address:
            if (expr.operand.value->index() == 0)
            {
                expr.type = expr.operand.GetType();
                expr.type.level_of_indirection++;
            }
            else 
            {
                IO::AddError({"Placeholder file", 0, "Can only get address of variables (lvalues), and not other types of expression (rvalues)."} );
            }
        break;
        case AST::dereference:
            if (expr.operand.value->index() > 0)
            {
                expr.type = expr.operand.GetType();
                expr.type.level_of_indirection--;
            }
            else 
            {
                IO::AddError({"Placeholder file", 0, "Cannot dereference non-pointer type " + expr.operand.GetType().base.identifier + "."} );
            }
        break;
    }
}

void TypeAnalyzer::AnalyzeBinaryExpression(AST::BinaryExpression& expr)
{
    AnalyzeExpression(expr.left_operand);
    AnalyzeExpression(expr.right_operand);
    switch (expr.operation) {
        case AST::addition:
        case AST::subtraction:
        case AST::multiplication:
        case AST::division:
        case AST::modulo:
        break;
        case AST::bit_and:
        case AST::bit_or:
        case AST::bit_xor:
        case AST::bitshift_left:
        case AST::bitshift_right:
        break;
        case AST::logic_and:
        case AST::logic_or:
        break;
        case AST::equals:
        case AST::notequals:
        case AST::lessthan:
        case AST::greaterthan:
        case AST::lessthan_equals:
        case AST::greaterthan_equals:
        break;
    }
}

void TypeAnalyzer::AnalyzeExpression(AST::Expression& expr)
{
    switch (expr.value->index()) {
            case 0:
                AnalyzeSymbolNameExpression(get<AST::SymbolNameExpression>(*expr.value));
            break;
            case 1:
                AnalyzeLiteralExpression(get<AST::LiteralExpression>(*expr.value));
            break;
            case 2:
                AnalyzeUnaryExpression(get<AST::UnaryExpression>(*expr.value));
            break;
            case 3:
                AnalyzeBinaryExpression(get<AST::BinaryExpression>(*expr.value));
            break;
            case 4:
                AnalyzeFunctionCall(get<AST::FunctionCallExpression>(*expr.value));
            break;
        }
}

void TypeAnalyzer::CheckAssignment(AST::AssignmentStatement& assignment)
{
    AnalyzeExpression(assignment.value);
    variant<nullopt_t, AST::VariableType, AST::FunctionType> target_type = FindTypeOfSymbol(assignment.target_name);
    if (target_type.index() == 1 and IsTypeConvertable(get<AST::VariableType>(target_type), assignment.value.GetType()))
    {
        
    }
    else 
    {
    
    }
}
void TypeAnalyzer::CheckVariableDefinition(AST::VariableDefinition& definition)
{
    AnalyzeExpression(definition.value);
    if (IsTypeConvertable(definition.declaration.type, definition.value.GetType()))
    {
    
    }
    else 
    {
    
    }
}

void TypeAnalyzer::AnalyzeStatement(AST::Statement& statement)
{
    switch (statement->index()) {
            case 0:
                current_scope_depth++;
                symbols.push_back({});
                AnalyzeBlock(get<AST::BlockStatement>(*statement));
                symbols.pop_back();
                current_scope_depth--;
            break;
            case 1:
                AnalyzeExpression(get<AST::ReturnStatement>(*statement).value);
            break;
            case 2:
                AnalyzeExpression(get<AST::IfStatement>(*statement).condition);
                AnalyzeBlock(get<AST::IfStatement>(*statement).content);
            break;
            case 3:
                AnalyzeExpression(get<AST::WhileLoop>(*statement).condition);
                AnalyzeBlock(get<AST::WhileLoop>(*statement).content);
            break;
            case 4:
                CheckAssignment(get<AST::AssignmentStatement>(*statement));
            break;
            case 5:
                AnalyzeFunctionCall(get<AST::FunctionCallExpression>(*statement));
            break;
            case 6:
                CheckVariableDefinition(get<AST::VariableDefinition>(*statement));
            break;
            case 7:
                AnalyzeFunctionDefinition(get<AST::FunctionDefinition>(*statement));
            break;
        }
}

void TypeAnalyzer::AnalyzeFunctionDefinition(AST::FunctionDefinition& definition)
{
    current_scope_depth++;
    symbols.push_back({});
    for (uint8_t param_idx = 0; param_idx < definition.declation.type.parameters.size(); param_idx++)
    {
        symbols.at(current_scope_depth).insert({definition.declation.type.parameters.at(param_idx).name, definition.declation.type.parameters.at(param_idx).type});
    }
    AnalyzeBlock(definition.body);
    symbols.pop_back();
    current_scope_depth--;
}

void TypeAnalyzer::AnalyzeBlock(AST::BlockStatement& block)
{
    for (auto& statement : block.statements)
    {
        AnalyzeStatement(statement);
    }
}