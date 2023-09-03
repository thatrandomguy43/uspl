#include <string>
#include <vector>
#include <variant>
#include <memory>

//this is so i can replace it with something  more appropriate if i later find something
using type_id = std::string;

class Variable
{
    std::string name;
    type_id type_name;
};
