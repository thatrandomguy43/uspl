#include "TypeAnalyzer.hpp"
#include "IO.hpp"

using namespace std;

const multimap<AST::Node, AST::Node> NUMERIC_CONVERSIONS
{
    {AST::Node::BuildBasicType("int8"), AST::Node::BuildBasicType("int16")},
    {AST::Node::BuildBasicType("int8"), AST::Node::BuildBasicType("int32")},
    {AST::Node::BuildBasicType("int8"), AST::Node::BuildBasicType("int64")},
    {AST::Node::BuildBasicType("int16"), AST::Node::BuildBasicType("int8")},
    {AST::Node::BuildBasicType("int16"), AST::Node::BuildBasicType("int32")},
    {AST::Node::BuildBasicType("int16"), AST::Node::BuildBasicType("int64")},
    {AST::Node::BuildBasicType("int32"), AST::Node::BuildBasicType("int8")},
    {AST::Node::BuildBasicType("int32"), AST::Node::BuildBasicType("int16")},
    {AST::Node::BuildBasicType("int32"), AST::Node::BuildBasicType("int64")},
    {AST::Node::BuildBasicType("int64"), AST::Node::BuildBasicType("int8")},
    {AST::Node::BuildBasicType("int64"), AST::Node::BuildBasicType("int16")},
    {AST::Node::BuildBasicType("int64"), AST::Node::BuildBasicType("int32")},

    {AST::Node::BuildBasicType("float32"), AST::Node::BuildBasicType("int8")},
    {AST::Node::BuildBasicType("float32"), AST::Node::BuildBasicType("int16")},
    {AST::Node::BuildBasicType("float32"), AST::Node::BuildBasicType("int32")},
    {AST::Node::BuildBasicType("float32"), AST::Node::BuildBasicType("int64")},
    {AST::Node::BuildBasicType("float64"), AST::Node::BuildBasicType("int8")},
    {AST::Node::BuildBasicType("float64"), AST::Node::BuildBasicType("int16")},
    {AST::Node::BuildBasicType("float64"), AST::Node::BuildBasicType("int32")},
    {AST::Node::BuildBasicType("float64"), AST::Node::BuildBasicType("int64")},
};



//this whole solution is rather bad, i will probably need some more practical way to define conversions
bool TypeAnalyzer::IsTypeConvertable(const AST::Node& to, const AST::Node& from) const
{
    return false;
}
/*
optional<AST::Node> TypeAnalyzer::FindTypeOfSymbol(const string& symbol) const
{
    optional<AST::Node> type;
    const AST::Node* block = global_scope;
    for (int block_idx = 0; block_idx < scope.size(); block_idx++)
    {
        for (int statement_idx = 0; statement_idx < scope[block_idx]; statement_idx++)
        {
            if (block->properties[statement_idx]->index() == 6) 
            {
                const AST::VariableDefinition& definition = get<AST::VariableDefinition>(*block->statements[statement_idx]);
                if (definition.declaration.name == symbol)
                    return definition.declaration.type;
            }
            else if (block->statements[statement_idx]->index() == 7)
            {
                const AST::VariableDefinition& definition = get<AST::VariableDefinition>(*block->statements[statement_idx]);
                if (definition.declaration.name == symbol)
                    return definition.declaration.type;
            }
        }
    }

    return type;
}
*/
void TypeAnalyzer::AnalyzeSymbolNameExpression(AST::Node& expr)
{

}

void TypeAnalyzer::AnalyzeLiteralExpression(AST::Node& literal)
{
    AST::Node& type = get<AST::Node>(literal.properties["value_type"]);
    type.id = "Type";
    type.properties["category"] = "basic";
    type.properties["is_const"] = false;
    if (get<string>(literal.properties["operation_type"]) == "literal_bool")
    {
        type.properties["base"] = "bool";
    }
    else if (get<string>(literal.properties["operation_type"]) == "literal_integer")
    {
        type.properties["base"] = "int64";
    }
    else if (get<string>(literal.properties["operation_type"]) == "literal_float")
    {
        type.properties["base"] = "float64";
    }
    else if (get<string>(literal.properties["operation_type"]) == "literal_char")
    {
        type.properties["base"] = "char";
    }
    else if (get<string>(literal.properties["operation_type"]) == "literal_string")
    {
        type.properties["category"] = "pointer";
        AST::Node const_char = AST::Node::BuildBasicType("char");
        const_char.properties["is_const"] = true;
        type.properties["pointed_to_type"] = const_char;
    }
    
}

void TypeAnalyzer::AnalyzeUnaryExpression(AST::Node& expr)
{
    AnalyzeExpression(get<AST::Node>(expr.properties["operand"]));
    const AST::Node& operand_type = get<AST::Node>(get<AST::Node>(expr.properties["operand"]).properties["type"]);
    if (get<string>(expr.properties["operation"]) == "negation")
    {
        if (IsTypeConvertable(AST::Node::BuildBasicType("int64"), operand_type))
        {
            expr.properties["type"] = AST::Node::BuildBasicType("int64");
        }
        else if (IsTypeConvertable(AST::Node::BuildBasicType("float64"), operand_type))
        {
            expr.properties["type"] = AST::Node::BuildBasicType("float64");
        }
        else 
        {
            IO::AddError({ filename, 0, "Cannot perform negation on non-numerical type " + operand_type.StringifyType() + "."} );
        }
    }
    else if (get<string>(expr.properties["operation"]) == "bitwise_not")
    {
        if (IsTypeConvertable({{"int64"}}, expr.operand.GetType())) 
        {
            expr.properties["type"] = AST::Node::BuildBasicType("int64");
        }
        else 
        {
            IO::AddError({ filename, 0, "Cannot perform bitwise operations on non-integer type " + expr.operand.GetType().base + "."} );
        }
    }
    else if (get<string>(expr.properties["operation"]) == "logical_not")
    {
        if (IsTypeConvertable({{"bool"}}, expr.operand.GetType())) 
        {
            expr.properties["type"] = AST::Node::BuildBasicType("bool");
        }
        else 
        {
            IO::AddError({ filename, 0, "Cannot perform logical operations on non-boolean type " + expr.operand.GetType().base + "."} );
        }
    }
    else if (get<string>(expr.properties["operation"]) == "addressof")
    {
        if (expr.operand.value->index() == 0)
        {
            expr.type = expr.operand.GetType();
            expr.type.level_of_indirection++;
        }
        else 
        {
            IO::AddError({ filename, 0, "Can only get address of variables (lvalues), and not other types of expression (rvalues)."} );
        }
    }
    else if (get<string>(expr.properties["operation"]) == AST::dereference)
    {
        if (expr.operand.value->index() > 0)
        {
            expr.type = expr.operand.GetType();
            expr.type.level_of_indirection--;
        }
        else 
        {
            IO::AddError({ filename, 0, "Cannot dereference non-pointer type " + expr.operand.GetType().base + "."} );
        }
    }   
}

void TypeAnalyzer::AnalyzeBinaryExpression(AST::Node& expr)
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
        case AST::bitwise_and:
        case AST::bitwise_or:
        case AST::bitwise_xor:
        case AST::bitshift_left:
        case AST::bitshift_right:
        break;
        case AST::logical_and:
        case AST::logical_or:
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
void TypeAnalyzer::AnalyzeFunctionCall(AST::Node& expr)
{

}


void TypeAnalyzer::AnalyzeExpression(AST::Node& expr)
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

void TypeAnalyzer::CheckAssignment(AST::Node& assignment)
{
    AnalyzeExpression(assignment.value);

}
void TypeAnalyzer::CheckVariableDefinition(AST::Node& definition)
{
    AnalyzeExpression(definition.value);


}

void TypeAnalyzer::AnalyzeStatement(AST::Node& statement)
{
    switch (statement->index()) {
            case 0:
                AnalyzeBlock(get<AST::BlockStatement>(*statement));
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

void TypeAnalyzer::AnalyzeFunctionDefinition(AST::Node& definition)
{
    AnalyzeBlock(definition.body);
}

void TypeAnalyzer::AnalyzeBlock(AST::Node& block)
{
    scope.push_back(0);
    for (; scope.back() < block.statements.size(); scope.back()++)
    {
        AnalyzeStatement(block.statements[scope.back()]);
    }
    scope.pop_back();
}

void TypeAnalyzer::AnalyzeTranslationUnit(AST::Node& unit)
{
    global_scope = &unit.global_scope;
    AnalyzeBlock(unit.global_scope);
}