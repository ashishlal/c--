#include <iostream>
#include <cstdlib>
#include <cstring>

class String {
    struct Srep {
        char *s;
        int sz;    // number of characters
        int n;     // reference count
        Srep(int nsz, const char *p) {
            n = 1;
            sz = nsz;
            s = new char[sz + 1];
            strcpy(s, p);
        }
        ~Srep() { delete [] s; }
        Srep *get_own_copy()
        {
            if(n == 1) return this;
            n--;
            return new Srep(sz, s);
        }
        void assign(int nsz, const char *p)
        {
            if(sz != nsz) {
               delete [] s;
               sz = nsz;
               s = new char[sz + 1];
            }
            strcpy(s,p);
        }
    };
    Srep *rep;
public:
    String() { rep = new Srep(0, ""); };
    String(int a, int b) { rep = new Srep(b-a, ""); };
    String(const String &x) { x.rep->n++; rep = x.rep; };
    ~String() { if(--rep->n == 0) delete rep; };
    String & operator=(const String &x)
    {
         x.rep->n++;
         if(--rep->n == 0) delete rep;
         rep = x.rep;
         return *this;
    };
    String(const char *s) { rep = new Srep(strlen(s), s); }
    String & operator=(const char *s)
    {
        if(rep->n == 1)
            rep->assign(strlen(s),s);
        else {
            rep->n--;
            rep = new Srep(strlen(s), s);
        }    
        return *this;
    }
    void check(int i) const {  
        if (i < 0 || rep->sz <= i) std::cout << "bad" << std::endl;
    }
    char read(int i) const { return rep->s[i]; }
    void write(int i, char c) const { rep->s[i]=c; }
    char operator[](int i) const { return rep->s[i]; }
    int size() const { return rep->sz; }
    String & operator() (int a, int b)
    {
        if(a >= b) return *this;
        if(b >= size()) return *this;
        String *s = new String(a, b);
        for(int i=0; i < (b-a); i++) {
           s->write(i, rep->s[a+i]);
        }
        return *s;
    }
};

class String_iter {
    // refer to string and string element
public:
    String_iter(String &s); // iterator for s
    char &get() { c = str[elemptr]; return c; };   // reference to curr element
    char &next();   // reference to next element
    bool hasNext();   // reference to next element
private:
    unsigned int elemptr; // ptr to current element
    String str;
    char c;
};

String_iter::String_iter(String &s)
{
    elemptr = 0;
    str = s;
}
char & String_iter::next()
{
    elemptr++;
    c = str[elemptr];
    return c;
}

bool String_iter::hasNext()
{
    bool ret = (elemptr < str.size())? true: false;
    return ret;
}

int main()
{
    String s("hello world");
    String_iter itr(s);
    for(char c = itr.get(); itr.hasNext(); c = itr.next()) {
        std::cout << c << std::endl;
    }
    std::cout << "" << std::endl;
    String s1(s(0, 5));
    String_iter itr1(s1);
    for(char c = itr1.get(); itr1.hasNext(); c = itr1.next()) {
        std::cout << c << std::endl;
    }
}
