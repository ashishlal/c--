#include <iostream>
#include <vector>

using namespace std;
template <class T, class U>
class Assoc {
    struct Pair {
        T key;
        U val;
        Pair(T &k, U &u): key(k), val(u) {}
    };
    vector<Pair> vec;
    Assoc(const Assoc &) {} ;  // copy constructor
    Assoc & operator=(const Assoc &) {}; // assignment operator
    public:
        Assoc() {}
        const U& operator[] (const T &t)
        {
            for(vector<Pair>::iterator p=vec.begin(); p != vec.end(); p++)
            {
                if(t == p->key) return p->val;
                return vec.back().val;
            }
        }
        void add(T &t, U & val)
        {
            for(vector<Pair>::iterator p=vec.begin(); p != vec.end(); p++)
            {
                if(t == p->key) return;
                vec.push_back(Pair(t, val));
            }
        }
        void erase(T &t)
        {
            for(vector<Pair>::iterator p=vec.begin(); p != vec.end(); p++)
            {
                if(s == p->key) {
                    vec.erase(p);
                    return;
                }
            }
        }
        U & find(T & key)
        {
            for(vector<Pair>::iterator p=vec.begin(); p != vec.end(); p++)
            {
                if(s == p->key) {
                    return p->val;
                }
            }
            return vec.back().val;
        }
        void print_all() const
        {
            for(vector<Pair>::iterator p=vec.begin(); p != vec.end(); p++)
            {
                //std::cout << p->name << ": " << p->val << std::endl;
            }
        }
    };

template<typename T, typename U>
class Map {
    Assoc<T, U> vec;
public:
    Map(): {};
    void insert(const T &t, const U &u)
    {
        vec.add(t, u);
    }
    void erase(const T & key)
    {
        vec.erase(key);
    }
    const U & find(const T & t)
    {
    }
};

class A {
public:
    string s;
    A() {};
    A(string &str): s(str) {};
    string & operator() { return s; }
    bool operator== (const A &a) { if(a.s == s)  return true; return false;}
};

class B {
public:
    double val;
    B() {};
    B(double &v): v(val) {};
    double & operator() { return val; };
    bool operator== (const B &b) { if(b.val == val)  return true; return false;}
}

int main()
{
    A a("hello");
    B b(100.001);
    Map<A, B> m;
    m.insert(a, b);
    cout << m.find(a) << endl;
}
