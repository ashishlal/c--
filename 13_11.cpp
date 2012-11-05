#include <iostream>
#include <vector>
#include <memory>

using namespace std;
 template<typename A, typename B>
    struct Pair {
        A key;
        B val;
        Pair(A &k, B &u): key(k), val(u) {}
        Pair(const A &k, const B &u): key(k), val(u) {}
        bool operator != (Pair X) { bool equal = ((X.key == key) && (X.val == val)); if(equal) return false; return true; }
    };

template <typename T, typename U>
class Assoc {
#if 0
    template<typename A, typename B>
    struct Pair {
        A key;
        B val;
        Pair(A &k, B &u): key(k), val(u) {}
        Pair(const A &k, const B &u): key(k), val(u) {}
    };
#endif
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
        void topologicalsort() 
        {
            vector<Pair<T, U> > vec1 ;
            vec1.clear();
            for(typename vector<Pair<T, U> >::iterator p=vec.begin(); 
                p != vec.end(); p++)  {
                auto pred = p;
                
                for(typename vector<Pair<T, U> >::iterator p1=vec.begin();
                    p1 != vec.end(); p1++)  {
                    if((!(p1->key.c)) && (pred->key.compare(p1->key) > 0)) {
                        pred = p1;
                    }
                }
                pred->key.c = true;
                vec1.push_back(*pred);
            }
            vec = std::move(vec1);
        }
        void print_all() 
        {
            cout << "=============================== " << endl;
            for(typename vector<Pair<T, U> >::iterator p=vec.begin(); 
                p != vec.end(); p++)
            {
                cout << p->key.s << ": " << p->val.val << endl;
            }
            cout << "=============================== " << endl;
        }
    };

template<typename X, typename Y>
class Map {
    Assoc<X, Y> vec;
public:
    Map() {};
    Map(const Assoc<X, Y> &vec1):vec(vec1) {};
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
    void topologicalsort()
    {
        return vec.topologicalsort();
    }
    void print_all()
    {
        return vec.print_all();
    }
};

class A {
public:
    string s;
    bool c;
    A() {c=false;};
    A(string &str): s(str), c(false) {};
    A(const char *str): s(string(str)) {};
    operator string() { return s; }
    bool operator== (A &a) { if(a.s == s)  return true; return false;}
    int compare(string &s1) { return s.compare(s1); }
    int compare(A &a) { return s.compare(a.s); }
};

class B {
public:
    double val;
    B() {};
    B(double &v): val(v) {};
    B(double v): val(v) {};
    operator double() { return val; };
    bool operator== (B &b) { if(b.val == val)  return true; return false;}
    int compare(double v) { return (int)(val-v); }
};

int main()
{
    A a("hello");
    B b(100.001);
    Map<A, B> m;
    m.insert(a, b);
    cout << m.find(a) << endl;
    //m.erase(a);
    A a1("hello world");
    m.insert(a1, b);
    m.insert("world", b);
    m.insert("a", 1.0);
    m.insert("b", 20);
    m.insert("dd", 50);
    m.insert("cc", 30);
    m.insert("ee", 60);
    m.insert("d", 40);
    cout << m.find(a1) << endl;
    cout << m.find("world") << endl;
    m.print_all();
    m.topologicalsort(); //comment topological sort does not work too well
    m.print_all();
}
