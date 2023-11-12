#include "TypeAnalyzer.hpp"

using namespace AST;
using namespace std;

bool TypeAnalyzer::IsValidValueType(const ValueType& type)
{
    const set<string> BASIC_TYPES{"void", "bool", "char", "int8", "int16", "int32", "int64", "float32", "float64"};
    switch (type.base_or_pointee_type.index()) {
        case 0:
        if (BASIC_TYPES.contains(get<string>(type.base_or_pointee_type)))
            return true;
        else
            return false;
        break;
        case 1:
            IO::AddError({*filename, type.start_pos, "sorry can't pointers yet :("});
            return false;
        break;
    }
    return false;
}



void TypeAnalyzer::Analyze(AST::Expression& node)
{

}

void TypeAnalyzer::Analyze(AST::VariableNameExpression& node)
{

}

void TypeAnalyzer::Analyze(AST::LiteralExpression& node)
{

}

void TypeAnalyzer::Analyze(AST::UnaryOperation& node)
{

}

void TypeAnalyzer::Analyze(AST::BinaryOperation& node)
{

}

void TypeAnalyzer::Analyze(AST::LocalStatement& node)
{

}

void TypeAnalyzer::Analyze(AST::GlobalStatement& node)
{

}

void TypeAnalyzer::Analyze(AST::FunctionCall& node)
{

}

void TypeAnalyzer::Analyze(AST::ReturnStatement& node)
{

}

void TypeAnalyzer::Analyze(AST::BlockStatement& node)
{

}

void TypeAnalyzer::Analyze(AST::IfStatement& node)
{

}

void TypeAnalyzer::Analyze(AST::WhileLoop& node)
{

}

void TypeAnalyzer::Analyze(AST::AssignmentStatement& node)
{

}

void TypeAnalyzer::Analyze(AST::VariableDefinition& node)
{

}

void TypeAnalyzer::Analyze(AST::FunctionDefinition& node)
{
    if (known_symbols.contains(node.name))
    {
        IO::AddError({*filename, node.name_file_pos, "Identifier '" + node.name + "' is already defined."});
        return;
    }
/*
    if (node.type) 
        

        
        known_symbols.insert({node.name, {&node, current_scope_depth}});*/
}

void TypeAnalyzer::Analyze(AST::TranslationUnit& node, const string* fname_ptr)
{
    filename = fname_ptr;
    for (auto& stat : node.statements)
    {
        if (VariableDefinition* subclass_ptr = dynamic_cast<VariableDefinition*>(stat.get()))
            Analyze(*subclass_ptr);
        else if (FunctionDefinition* subclass_ptr = dynamic_cast<FunctionDefinition*>(stat.get()))
            Analyze(*subclass_ptr);
    }
}

