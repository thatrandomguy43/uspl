#include <functional>
#include <string>
#include <vector>
#include <variant>
#include <memory>
#include "Tokenizer.hpp"


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
class BlockStatement;
class AssignmentStatement
{
    std::string target_name;
    std::unique_ptr<Expression> value; 
};
class Statement
{
    std::unique_ptr<std::variant<BlockStatement, AssignmentStatement>> w;
};
class BlockStatement
{
    std::vector<Statement> statements;
};

class VariableDeclaration
{
    public:
    std::string name;
    VariableType type;
};
class VariableDefinition
{
    public:
    VariableDeclaration declation;
    Expression value;
};
class FunctionType
{
    public:
    UnqualifiedType return_type;
    std::vector<UnqualifiedType> parameter_types;
};
class FunctionDeclaration
{
    public:
    std::string name;
    FunctionType type;
};
class FunctionDefinition
{
    public:
    FunctionDeclaration declation;
    BlockStatement body;
};
class TranslationUnitAST
{
    public:
    TranslationUnitAST(const std::vector<Token>&);
    std::vector<Statement> statements;
};