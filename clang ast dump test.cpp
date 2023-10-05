#include <map>
#include <iostream>
#include <cstdarg>
#include <stdarg.h>
int main()
{
    std::map<int, std::string> v{{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}};
    std::map<int, std::string>::iterator itr = v.begin();
    std::cout << sizeof *itr;
    return 0;
}