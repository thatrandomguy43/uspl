#include <string>
#include <vector>
#include <variant>
#include <memory>
#include "Tokenizer.hpp"

namespace AST
{

enum BinaryOpType
{
    addition,
    subtraction,
    multiplication,
    division,
    modulo,
    bit_and,
    bit_or,
    bit_xor,
    bitshift_left,
    bitshift_right,
    logic_and,
    logic_or,
    equals,
    notequals,
    lessthan,
    greaterthan,
    lessthan_equals,
    greaterthan_equals,


};

class BlockStatement;
class AssignmentStatement;
class VariableDefinition;
class FunctionDefinition;

class UnqualifiedType 
{
    public:
    std::string id;
};
class VariableType
{
    public:
    UnqualifiedType base;
    bool is_const;
};
class SymbolNameExpression
{
    std::string name;
    VariableType type;
};
using LiteralExpression = std::variant<bool, char, std::string, uint64_t, double>;
class BinaryOperation
{
    std::string name;
    VariableType type;
};
class UnaryOperation
{
    std::string name;
    VariableType type;
};
class Expression
{
    public:
    std::unique_ptr<std::variant<SymbolNameExpression, LiteralExpression>> value;
    VariableType type;
};
using Statement = std::unique_ptr<std::variant<BlockStatement, AssignmentStatement, VariableDefinition, FunctionDefinition>>;
class AssignmentStatement
{
    std::string target_name;
    std::unique_ptr<Expression> value; 
};
class BlockStatement
{
    std::vector<Statement> statements;
};
class WhileLoop
{
    BlockStatement content;
    Expression condition;
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
    VariableType return_type;
    std::vector<VariableType> parameter_types;
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
    AST::Expression MakeExpression();
    AST::VariableDefinition MakeVariableDefinition();
    AST::TranslationUnit& BuildFile(const std::vector<Token>&, std::string);                                                                     
};