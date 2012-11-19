#include <iostream>

class B;
class A {
public:
    A() {};
    ~A() {};
    static int f();
    static B *b;
    static void setB(B *b1);
};

B* A::b;
class B: private A {
public:
    B();
    ~B() {};
    unsigned x;
};

int A::f() { return A::b->x; };
void A::setB(B *b1) { b = b1; };
B::B() { A::b = this; };

int main()
{
    B b2;
    b2.x=100;
    std::cout << A::f() << std::endl;
    return 0;
}
