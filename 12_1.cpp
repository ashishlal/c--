#include <iostream>

using namespace std;
class Base {
    public:
    Base() {};
        virtual void iam() { cout << "Base\n"; }
        void iam2() { cout << "Base-iam2\n"; }
};
class Derived1: public Base {
    public:
    Derived1() {};
        virtual void iam() { cout << "Derived1\n"; }
        void iam2() { cout << "Derived1-iam2\n"; }
};

class Derived2: public Base {
    public:
    Derived2() {};
        virtual void iam() { cout << "Derived2\n"; }
};

int main()
{
    Derived1 *d1 = new Derived1;
    Derived2 *d2; 
    Base *b = d1;
    b->iam() ;
    d1->iam() ;
    b->iam2() ;
    d2 = static_cast <Derived2 *>(b);
    if(d2) d2->iam() ;
    b->iam() ;
}
