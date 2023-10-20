#include <unordered_map>
#include <typeinfo>
class xyz{
    std::unordered_map<int, xyz> abc;
};
int main(int argc, char**)
{
    xyz AAAAA;
    return 0;
}