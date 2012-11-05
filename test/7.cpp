#include <iostream>
using namespace std;
int i=0;
int foobar() 
{
    return i;
}
#if 1
int &foo(int &a)
{
    i = a;
    return i;
}
#endif
int &foo(int &&a)
{
    i = a;
    return i;
}

int main()
{
    int j = foobar();
    int *r = &foo(i);
    cout << j << endl;
    cout << *r << endl;
    cout << foo(42) << endl;
}
