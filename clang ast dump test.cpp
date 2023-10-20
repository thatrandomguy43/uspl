
#include <typeinfo>
int main(int argc, char**)
{
    const std::type_info& x = typeid([](double x){return x*x;});
    return 0;
}