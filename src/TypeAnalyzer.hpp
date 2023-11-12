#include "ASTBuilder.hpp"
class TypeAnalyzer
{
const std::string* filename;
std::unordered_map<std::string, std::pair<std::variant<const AST::VariableDefinition*, const AST::FunctionDefinition*>, uint16_t>> known_symbols;
uint16_t current_scope_depth = 0;

bool IsValidValueType(const AST::ValueType&);
void Analyze(AST::Expression&);
void Analyze(AST::VariableNameExpression&);
void Analyze(AST::LiteralExpression&);
void Analyze(AST::UnaryOperation&);
void Analyze(AST::BinaryOperation&);
void Analyze(AST::LocalStatement&);
void Analyze(AST::GlobalStatement&);
void Analyze(AST::FunctionCall&);
void Analyze(AST::ReturnStatement&);
void Analyze(AST::BlockStatement&);
void Analyze(AST::IfStatement&);
void Analyze(AST::WhileLoop&);
void Analyze(AST::AssignmentStatement&);
void Analyze(AST::VariableDefinition&);
void Analyze(AST::FunctionDefinition&);

public:
void Analyze(AST::TranslationUnit&, const std::string*);
};

