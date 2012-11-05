#include <functional>
#include <map>
#include <iostream>

typedef std::function<unsigned int ()> MyCB;
class myB {
public:
    myB() {};
    ~myB() {};
    void add_stat(unsigned int n, MyCB cb)
    {
        m_stat[n] = cb;
    };
    template<class T>
    void add_stat(unsigned int n, T& obj, unsigned int (T::*cb)())
    {
        add_stat(n, std::bind(cb, &obj));
    }
    virtual MyCB get_stat(unsigned int n)
    {
        return m_stat[n];
    }
private:
    std::map<unsigned int, MyCB> m_stat;
};

class myA {
public:
    myA(myB *bp):b(bp) 
    {
        b->add_stat<myA>(0, *this, &myA::my_stat);
    }
    ~myA() {};
    unsigned int my_stat() { return 2; };
private:
    myB *b;
};

int main()
{
    myB b;
    myA a(&b);
    std::cout << (b.get_stat(0))() << std::endl;
}
