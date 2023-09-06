#include <string>
#include <vector>
#include <variant>
#include <memory>
#include "Tokenizer.hpp"

namespace AST
{


class BlockStatement;
class AssignmentStatement;
class VariableDefinition;
class FunctionDefinition;

class UnqualifiedType 
{
    public:
    std::string id;
};
class SymbolNameExpression
{
    std::string name;
};
class LiteralExpression
{
    std::variant<bool, char, std::string, uint64_t, double> value;
};
class VariableType
{
    public:
    UnqualifiedType base;
};
class Expression
{
    public:
    std::unique_ptr<std::variant<SymbolNameExpression, LiteralExpression>> value;
    VariableType type;
};
class Statement
{
    public:
    std::unique_ptr<std::variant<BlockStatement, AssignmentStatement, VariableDefinition, FunctionDefinition>> type;
};
class AssignmentStatement
{
    std::string target_name;
    std::unique_ptr<Expression> value; 
};
class BlockStatement
{
    std::vector<Statement> statements;
};

class VariableDefinition
{
    public:
    std::string name;
    VariableType type;
    Expression value;
};
class FunctionType
{
    public:
    UnqualifiedType return_type;
    std::vector<UnqualifiedType> parameter_types;
};

class FunctionDefinition
{
    public:
    FunctionType declation;
    std::string name;
    std::vector<std::string> param_names;
    BlockStatement body;
};
class TranslationUnit
{
    public:
    std::vector<Statement> statements;
};

}

class ASTBuilder    
{
    std::string filename;
    std::vector<Token> tokens;
    size_t token_index = 0;
    public:
    AST::TranslationUnit root;
    AST::TranslationUnit& BuildFile(const std::vector<Token>&, std::string);                                                                     
};