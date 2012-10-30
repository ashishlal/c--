#include <iostream>
#include <string>

using namespace std;
void *f() {return NULL;}
int main()
{
    char *str ;
    int j = 99;
    int *i = &j;
    str = new char(5);
    cout << "s = " << &str << " i = " << i << " f = " << &f << endl;
}
