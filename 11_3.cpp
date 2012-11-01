#include <iostream>

using namespace std;
class INT {
public:
    INT() {};
    INT(int j) {i=j;};
    INT(const INT &j) {i=j.i;};
    int operator+ (int j) { return i + j; };
    int operator+ (INT k) { return i + k.i; };
    int operator++ () { return ++i; };
    int operator+= (int j) { return i+=j; };
    INT operator= (int j) { i=j; return *this; };
    INT operator= (INT j) { i=j.i; return *this; };
    operator int() { return i; };
private:
    int i;
};
int main()
{
    INT a = 10;
    cout << a << endl;
    cout << a+1 << endl;
    a+=1;
    cout << a << endl;
}
