#include <iostream>
#include <memory>
using namespace std;

int nogood()
{
    int a = 4;
    cout << a;
    if(a == 5)
    {
        return 1;
    }
}

int main(int argc, char** argv)
{
    int x = nogood();
    return 0;
}