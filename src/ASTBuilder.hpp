#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "Tokenizer.hpp"

namespace AST
{


class Node;


//this solution is also 100x easier to serialize, since its gonna be structured a lot like json
class Node
{
    public:
    std::string id;
    std::unordered_map<std::variant<int64_t, std::string>, std::variant<bool,int64_t, double, std::string, Node>> properties;

};

class Builder    
{
std::string filename;
std::vector<Token> tokens;
size_t token_index;

Node MakeType();
Node MakeFunctionCallExpression();
Node MakeBinaryExpression();
Node MakeUnaryExpression();
Node MakeSimpleExpression();
Node MakeExpression();
Node MakeBlockStatement();
Node MakeReturnStatement();
Node MakeIfStatement();
Node MakeWhileLoop();
Node MakeAssignmentStatement();
Node MakeDeclaration();
Node MakeVariableDefinition();
Node MakeFunctionDefinition();
public:


void BuildFile(Node&, const std::vector<Token>&, const std::string&);                                                                     
};
}