#include <iostream>
#include <vector>

using namespace std;
template <typename T, typename U>
class Assoc {
    template<typename A, typename B>
    struct Pair {
        A key;
        B val;
        Pair(A &k, B &u): key(k), val(u) {}
        Pair(const A &k, const B &u): key(k), val(u) {}
    };
    vector<Pair<T, U> > vec;
    Assoc(const Assoc &) {} ;  // copy constructor
    Assoc & operator=(const Assoc &) {}; // assignment operator
    public:
        Assoc() {}
        const U& operator[] (const T &t)
        {
            for(typename vector<Pair<T, U> >::iterator p=vec.begin(); p != vec.end(); p++)
            {
                if(t == p->key) return p->val;
                return vec.back().val;
            }
        }
        void add(const T &t, const U & val)
        {
            for(typename vector<Pair<T, U> >::iterator p=vec.begin(); p != vec.end(); p++)
            {
                if(const_cast<T &>(t) == p->key) return;
            }
            vec.push_back(Pair<T, U> (t, val));
        }
        void erase(const T &t)
        {
            for(typename vector<Pair<T, U> >::iterator p=vec.begin(); p != vec.end(); p++)
            {
                if(const_cast<T &>(t) == p->key) {
                    vec.erase(p);
                    return;
                }
            }
        }
        U & find(const T & key) 
        {
            for(typename vector<Pair<T, U> >::iterator p=vec.begin(); p != vec.end(); p++)
            {
                if(const_cast<T &>(key) == p->key) {
                    return p->val;
                }
            }
            return vec.back().val;
        }
        void print_all() const
        {
            for(typename vector<Pair<T, U> >::iterator p=vec.begin(); p != vec.end(); p++)
            {
                //std::cout << p->name << ": " << p->val << std::endl;
            }
        }
    };

template<typename X, typename Y>
class Map {
    Assoc<X, Y> vec;
public:
    Map() {};
    void insert(const X &t, const Y &u)
    {
        vec.add(t, u);
    }
    void erase(const X & key)
    {
        vec.erase(key);
    }
    Y & find(const X & t) 
    {
        return vec.find(t);
    }
};

class A {
public:
    string s;
    A() {};
    A(string &str): s(str) {};
    A(const char *str): s(string(str)) {};
    operator string() { return s; }
    bool operator== (A &a) { if(a.s == s)  return true; return false;}
};

class B {
public:
    double val;
    B() {};
    B(double &v): val(v) {};
    B(double v): val(v) {};
    operator double() { return val; };
    bool operator== (B &b) { if(b.val == val)  return true; return false;}
};

int main()
{
    A a("hello");
    B b(100.001);
    Map<A, B> m;
    m.insert(a, b);
    cout << m.find(a) << endl;
    m.erase(a);
    A a1("hello world");
    m.insert(a1, b);
    m.insert("world", b);
    cout << m.find(a1) << endl;
    cout << m.find("world") << endl;
}
