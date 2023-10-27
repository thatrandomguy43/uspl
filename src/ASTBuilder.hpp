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

    static Node BuildBasicType(const std::string& type_name)
    {
        Node node;
        node.id = "Type";
        node.properties["is_const"] = false;
        node.properties["category"] = "basic";
        node.properties["base"] = type_name;
        return node;
    }
    std::string StringifyType() const 
    {
        return "WHOOPS MISSING NAME LOUL";
    }
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
Node MakeSymbolDeclaration();
Node MakeVariableDefinition();
Node MakeFunctionDefinition();
public:


void BuildFile(Node&, const std::vector<Token>&, const std::string&);                                                                     
};
}