#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "Tokenizer.hpp"

namespace AST
{

enum UnaryOpType
{
    bitwise_not,
    logical_not,
    dereference,
    addressof,
    negation,
};

enum BinaryOpType
{
    addition,
    subtraction,
    multiplication,
    division,
    modulo,
    bitwise_and,
    bitwise_or,
    bitwise_xor,
    bitshift_left,
    bitshift_right,
    logical_and,
    logical_or,
    equals,
    notequals,
    lessthan,
    greaterthan,
    lessthan_equals,
    greaterthan_equals,
};

const std::unordered_map<TokenType, UnaryOpType> UNARY_OPERATORS
{
   {operator_bitwise_not, bitwise_not},
   {operator_logical_not, logical_not},
   {operator_pointer, dereference},
   {operator_address, addressof},
   {operator_subtraction, negation}
};
const std::unordered_map<TokenType, BinaryOpType> BINARY_OPERATORS
{
    {operator_addition, addition},
    {operator_subtraction, subtraction},
    {operator_multiplication, multiplication},
    {operator_division, division},
    {operator_modulo, modulo},
    {operator_bitwise_and, bitwise_and},
    {operator_bitwise_or, bitwise_or},
    {operator_bitwise_xor, bitwise_xor},
    {operator_shift_left, bitshift_left},
    {operator_shift_right, bitshift_right},
    {operator_logical_and, logical_and},
    {operator_logical_or, logical_or},
    {operator_equality, equals},
    {operator_inequality, notequals},
    {operator_lessthan, lessthan},
    {operator_greaterthan, greaterthan},
    {operator_less_or_equal, lessthan_equals},
    {operator_greater_or_equal, greaterthan_equals}
};

class ValueType 
{
    public:
    bool is_const;
    std::variant<std::string, std::unique_ptr<ValueType>> base_or_pointee_type;
};

class FunctionType
{
    public:
    std::unique_ptr<ValueType> return_type;
    std::vector<std::unique_ptr<ValueType>> parameters;
};

class Node
{
    public:
    size_t start_pos, end_pos;
    virtual std::string Serialize() const = 0;
    virtual ~Node() = 0;
};

class Expression
{
    protected:
    Expression(){};
    public:
    std::unique_ptr<ValueType> type;
};

class SymbolExpression : public Expression, public Node
{
    public:
    std::string name;
    std::string Serialize() const {return "Placeholder node serialization output.";}
    ~SymbolExpression(){};
};

class LiteralExpression : public Expression, public Node
{
    public:
    std::string name;
    std::variant<bool, int64_t, double, char, std::string> value;
    std::string Serialize() const {return "Placeholder node serialization output.";}
    ~LiteralExpression(){};
};

class UnaryOperation : public Expression, public Node
{
    public:
    UnaryOpType operation;
    std::unique_ptr<Expression> operand;
    std::string Serialize() const {return "Placeholder node serialization output.";}
    ~UnaryOperation(){};
};

class BinaryOperation : public Expression, public Node
{
    public:
    BinaryOpType operation;
    std::unique_ptr<Expression> left_operand, right_operand;
    std::string Serialize() const {return "Placeholder node serialization output.";}
    ~BinaryOperation(){};
};

class LocalStatement
{
    protected:
    LocalStatement(){};
    public:
};

class GlobalStatement
{
    protected:
    GlobalStatement(){};
    public:
};


class FunctionCall : public Expression, public LocalStatement, public Node
{
    public:
    std::string name;
    std::vector<std::unique_ptr<Expression>> arguments;
    std::string Serialize() const {return "Placeholder node serialization output.";}
    ~FunctionCall(){};
};

class ReturnStatement : public LocalStatement, public Node
{
    public:
    std::unique_ptr<Expression> value;
    std::string Serialize() const {return "Placeholder node serialization output.";}
    ~ReturnStatement(){};
};

class BlockStatement : public LocalStatement, public Node
{
    public:
    std::vector<std::unique_ptr<LocalStatement>> statements;
    std::string Serialize() const {return "Placeholder node serialization output.";}
    ~BlockStatement(){};
};

class IfStatement : public LocalStatement, public Node
{
    public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> content;
    std::string Serialize() const {return "Placeholder node serialization output.";}
    ~IfStatement(){};
};

class WhileLoop : public LocalStatement, public Node
{
    public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> content;
    std::string Serialize() const {return "Placeholder node serialization output.";}
    ~WhileLoop(){};
};

class AssignmentStatement : public LocalStatement, public Node
{
    public:
    std::string symbol;
    std::unique_ptr<Expression> value;
    std::string Serialize() const {return "Placeholder node serialization output.";}
    ~AssignmentStatement(){};
};

class VariableDefinition : public LocalStatement, public GlobalStatement, public Node
{
    public:
    std::string name;
    ValueType type;
    std::unique_ptr<Expression> value;
    std::string Serialize() const {return "Placeholder node serialization output.";}
    ~VariableDefinition(){};
};

class FunctionDefinition : public GlobalStatement, public Node
{
    public:
    std::string name;
    FunctionType type;
    std::unique_ptr<BlockStatement> body;
    std::string Serialize() const {return "Placeholder node serialization output.";}
    ~FunctionDefinition(){};
};

class TranslationUnit
{
    public:
    std::vector<std::unique_ptr<GlobalStatement>> statements;
};


class Builder    
{
std::string filename;
std::vector<Token> tokens;
size_t token_index;

ValueType MakeValueType();
FunctionType MakeFunctionType();
std::unique_ptr<FunctionCall> MakeFunctionCallExpression();
std::unique_ptr<BinaryOperation> MakeBinaryExpression();
std::unique_ptr<UnaryOperation> MakeUnaryExpression();
std::unique_ptr<Expression> MakeSimpleExpression();
std::unique_ptr<Expression> MakeExpression();
std::unique_ptr<BlockStatement> MakeBlockStatement();
std::unique_ptr<ReturnStatement> MakeReturnStatement();
std::unique_ptr<IfStatement> MakeIfStatement();
std::unique_ptr<WhileLoop> MakeWhileLoop();
std::unique_ptr<AssignmentStatement> MakeAssignmentStatement();
std::unique_ptr<VariableDefinition> MakeVariableDefinition();
std::unique_ptr<FunctionDefinition> MakeFunctionDefinition();

size_t CalcEndPos();
public:


TranslationUnit BuildFile(const std::vector<Token>&, const std::string&);                                                                     
};
}