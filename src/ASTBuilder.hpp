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

enum UnaryOpType
{
    negation,
    bit_not,
    logic_not,
    address,
    dereference,
};


class BlockStatement;
class AssignmentStatement;
class VariableDefinition;
class FunctionDefinition;
class SymbolNameExpression;
class UnaryExpression;
class BinaryExpression;
class FunctionCallExpression;
//the nullopt is just so i dont get a compile error when assingnning the contents of a literal value token to it
//the compiler dosent know that nullopt does not happen if the token type is a literal
using LiteralExpression = std::variant<std::nullopt_t, bool, uint64_t, double, char, std::string>;
class UnqualifiedType 
{
    public:
    std::string identifier;
};
class VariableType
{
    public:
    UnqualifiedType base;
    bool is_const;
};
class Expression
{
    public:
    std::unique_ptr<std::variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression, FunctionCallExpression>> value;
    VariableType type;
};
class SymbolNameExpression
{
    public:
    std::string name;
    VariableType type;
};

class BinaryExpression
{
    public:
    Expression left_operand;
    Expression right_operand;
    BinaryOpType operation;
    VariableType type;
};
class UnaryExpression
{
    public:
    Expression operand;
    UnaryOpType operation;
    VariableType type;
};
class FunctionCallExpression
{
    public:
    std::string identifier;
    std::vector<Expression> args;
};
using Statement = std::unique_ptr<std::variant<BlockStatement, AssignmentStatement, VariableDefinition, FunctionDefinition>>;
class AssignmentStatement
{
    public:
    std::string target_name;
    Expression value; 
};
class BlockStatement
{
    public:
    std::vector<Statement> statements;
};
class IfStatement
{
    public:
    BlockStatement content;
    Expression condition;
};
class WhileLoop
{
    public:
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
class FunctionDeclaration
{
    public:
    FunctionType type;
    std::string name;
};
class FunctionDefinition
{
    public:
    FunctionDeclaration declation;
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
    size_t token_index;

    AST::FunctionCallExpression MakeFunctionCallExpression();
    AST::BinaryExpression MakeBinaryExpression();
    AST::UnaryExpression MakeUnaryExpression();
    AST::Expression MakeExpression();
    std::variant<AST::SymbolNameExpression, AST::LiteralExpression> MakeSimpleExpression();
    AST::VariableDefinition MakeVariableDefinition();
    public:


    void BuildFile(AST::TranslationUnit&, const std::vector<Token>&, std::string);                                                                     
};