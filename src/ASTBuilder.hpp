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



class AssignmentStatement;
class VariableDefinition;
class FunctionDefinition;
class SymbolNameExpression;
class UnaryExpression;
class BinaryExpression;
class FunctionCallExpression;
class BlockStatement;
class ReturnStatement;
class IfStatement;
class WhileLoop;
//the nullopt is just so i dont get a compile error when assingnning the contents of a literal value token to it
//the compiler dosent know that nullopt does not happen if the token type is a literal

class UnqualifiedType 
{
    public:
    std::string identifier;
};
class VariableType
{
    public:
    UnqualifiedType base;
    bool is_const = false;
    int8_t level_of_indirection = 0;
};
class LiteralExpression
{
    public:
    std::variant<std::nullopt_t, bool, uint64_t, double, char, std::string> value;
    VariableType type;
};
class Expression
{
    public:
    std::unique_ptr<std::variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression, FunctionCallExpression>> value;
    VariableType GetType();
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
    VariableType type;
};

using Statement = std::unique_ptr<std::variant<BlockStatement, ReturnStatement, IfStatement, WhileLoop, AssignmentStatement, FunctionCallExpression, VariableDefinition, FunctionDefinition>>;
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
class ReturnStatement
{
    public:
    Expression value;
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
class VariableDeclaration
{
    public:
    std::string name;
    VariableType type;
};
class VariableDefinition
{
    public:
    VariableDeclaration declaration;
    Expression value;
};
class FunctionType
{
    public:
    VariableType return_type;
    std::vector<VariableDeclaration> parameters;
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
    BlockStatement body;
};
class TranslationUnit
{
    public:
    BlockStatement global_scope;
};

}

class ASTBuilder    
{
    std::string filename;
    std::vector<Token> tokens;
    size_t token_index;

    AST::VariableType MakeVariableType();
    AST::FunctionCallExpression MakeFunctionCallExpression();
    AST::BinaryExpression MakeBinaryExpression();
    AST::UnaryExpression MakeUnaryExpression();
    std::variant<AST::SymbolNameExpression, AST::LiteralExpression> MakeSimpleExpression();
    AST::Expression MakeExpression();
    AST::BlockStatement MakeBlockStatement();
    AST::ReturnStatement MakeReturnStatement();
    AST::IfStatement MakeIfStatement();
    AST::WhileLoop MakeWhileLoop();
    AST::AssignmentStatement MakeAssignmentStatement();
    AST::VariableDefinition MakeVariableDefinition();
    AST::FunctionDefinition MakeFunctionDefinition();
    public:


    void BuildFile(AST::TranslationUnit&, const std::vector<Token>&, std::string);                                                                     
};