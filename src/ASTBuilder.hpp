#pragma once
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
class Declaration;
class Type;

class Type
{
    public:
    std::string base;
    bool is_const = false;
    bool is_function = false;
    int8_t level_of_indirection = 0;
    std::vector<Declaration> parameters = {};
};


class LiteralExpression
{
    public:
    std::variant<std::nullopt_t, bool, uint64_t, double, char, std::string> value;
    Type type;
};
class Expression
{
    public:
    std::unique_ptr<std::variant<SymbolNameExpression, LiteralExpression, UnaryExpression, BinaryExpression, FunctionCallExpression>> value;
    Type GetType() const;
};
class SymbolNameExpression
{
    public:
    std::string name;
    Type type;

};

class BinaryExpression
{
    public:
    Expression left_operand;
    Expression right_operand;
    BinaryOpType operation;
    Type type;
};
class UnaryExpression
{
    public:
    Expression operand;
    UnaryOpType operation;
    Type type;
};
class FunctionCallExpression
{
    public:
    std::string identifier;
    std::vector<Expression> args;
    Type type;
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
class Declaration
{
    public:
    Type type;
    std::string name;
};
class VariableDefinition
{
    public:
    Declaration declaration;
    Expression value;
};
class FunctionDefinition
{
    public:
    Declaration declation;
    BlockStatement body;
};

}
class TranslationUnit
{
    public:
    AST::BlockStatement global_scope;
};

class ASTBuilder    
{
std::string filename;
std::vector<Token> tokens;
size_t token_index;

AST::Type MakeType();
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
AST::Declaration MakeDeclaration();
AST::VariableDefinition MakeVariableDefinition();
AST::FunctionDefinition MakeFunctionDefinition();
public:


void BuildFile(TranslationUnit&, const std::vector<Token>&, const std::string&);                                                                     
};