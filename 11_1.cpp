#include <iostream>
using namespace std;

struct Y;
typedef struct X {
    int i;
    X(int j) { i= j; };
    int operator+ (int j) { return i+j; };
    int operator* (X a) { return i*a.i; };
} X;

typedef struct Y {
    int i;
    Y(X x) { i = x.i; };
    Y operator+ (X x) {i += x.i; return *this;};
    int operator+ (Y y) {return i + y.i; };
    int operator+ (int x) {return i + x; };
    int operator* (int x) {return i * x; };
    int operator* (Y b) {return i * b.i; };
} Y;

extern X operator* (X, Y);
extern int f(X);
X x =1;
Y y = x;
int i = 2;

X operator* (X x, Y y)
{
    x.i = x.i * y.i;
    return x;
}

int operator* (int x, Y y)
{
    return x * y.i;
}

int operator+ (int x, Y b)
{
    return x + b.i;
}

int f(int a)
{
    return a;
}


int f(Y y)
{
    return y.i;
}

int main()
{
    cout << i+10 << endl;
    cout << y+10 << endl;
    cout << y+10*y << endl;
    cout << x+(y+i) << endl;
    cout << x*x+i << endl;
    cout << f(7) << endl;
    cout << f(y) << endl;
    cout << y+y << endl;
    cout << 106+y << endl;
}
