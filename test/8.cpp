#include <iostream>

class B;
class A {
public:
    A() {b = new B;};
    ~A() {};
    B *b;
    void setB(B *b1) {b = b1; };
    unsigned x;
};

class B {
public:
    B() {};
    ~B() {};
    A *a;
    void setA(A *a1) {a = a1; };
    void f() { std::cout << a->x << std::endl; };
};

int main()
{
    A a1;
    a1.x = 10;
    a1.b->f();
}
