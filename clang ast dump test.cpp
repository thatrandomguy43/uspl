#include <iostream>
#include <memory>
using namespace std;

class B 
{
    public:
virtual void Print()
{
print(cout, "base class");
}
};
class D : public B
{
    public:
    void Print()
    {
        print(cout, "derived class");
    }
};

unique_ptr<D> MakeD()
{
    return make_unique<D>(D{});
}

int main(int argc, char** argv)
{
    unique_ptr<B> pb = MakeD();
    pb->Print();
    return 0;
}