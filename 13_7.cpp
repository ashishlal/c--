#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::istringstream;
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
    B(double &&v): val(std::forward<double> (v)) {};
    // B(double const & v): val(v) {};
    operator double() { return val; };
    bool operator== (B &b) { if(b.val == val)  return true; return false;}
    int compare(double v) { return (int)(val-v); }
};

int main()
{
    Map<A, B> m;
    string str;
    cout << "Enter a (key, value) pair seperated by a comma" << endl;
    cout << "Enter a string for key and a number for value" << endl;
    getline(cin, str);
    istringstream ss(str);
    string word;
    vector<string> words;
    while( getline(ss, word, ',') ) {
        cout << word << "\n";
        words.push_back(word);
    }
    double val = atof(words[1].c_str());
    m.insert(words[0], val);
    m.insert("aa", 42);
    m.print_all();
}
