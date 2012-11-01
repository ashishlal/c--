#include <iostream>

using namespace std;

template<class T> class List {
    typedef struct Link {
        Link *pre;
        Link *suc;
        T val;
        Link(Link *p, Link *s, const T& v): pre(p), suc(s), val(v) {}
    } Link;
    Link *head;
    Link *curr;
public:
    List(): head(NULL), curr(NULL) {}
    ~List() { for(Link *p=head; p; p=p->suc) delete p; }
    List(const T & t): head(new Link(NULL, NULL, t)), curr(head) {}
    void add(T t) { Link *l = new Link(NULL, NULL, t); 
                      if(curr==NULL) curr = l; 
                      if(head == NULL) head = l;
                      l->pre = curr; l->suc=NULL;
                      curr->suc=l;
                      curr = l; }
    Link *begin() { return head; }
    Link *end() { return curr; }
    void erase(Link *l) { 
        for(Link *p=head; p; p=p->suc) {
            if((unsigned int)p == (unsigned int)l) {
                p->suc->pre = p->pre;
                p->pre->suc = p->suc;
                p->pre = NULL; p->suc = NULL;
                delete p;
                return ;
            }
        }
    }
    void remove(T t) { 
        for(Link *p=head; p; p=p->suc) {
            if(p->val == t) {
                print_ptrs();
                p->suc->pre = p->pre;
                p->pre->suc = p->suc;
                if((unsigned int)p == (unsigned int)head) {
                    head = p->suc;
                }
                if((unsigned int)p == (unsigned int)curr) {
                    curr = p->pre;
                }
                p->pre = NULL; p->suc = NULL;
                delete p;
                return;
            }
        }
    }
    size_t size() {
        size_t i=0;
        for(Link *p=head; p; p=p->suc) i++;
        return i;
    }
    void print_all() {
        for(Link *p=head; p; p=p->suc) std::cout << p->val << std::endl;
    }
    void print_ptrs() {
        for(Link *p=head; p; p=p->suc) std::cout << p << " " << p->pre << " " << p->suc << std::endl;
    }
};

int main()
{
    List<int> l;
    l.add(1);
    l.add(1);
    l.add(2);
    l.add(3);
    l.print_all();
    l.remove(1);
    l.print_all();
}
