#include <iostream>
#include <exception>
#include <stdexcept>

template <typename T>
class Ptr_to_T {
    T *p;
    T *array;
    int size;
public:
    // bind to array v of size s, initial val p
    Ptr_to_T(T *p1, T* v, int s): p(p1), array(v), size(s) {};
    Ptr_to_T(T *p1) {};

    Ptr_to_T & operator++ () throw() {
        if(((char *)p - (char *)array) >= size) {
            throw std::out_of_range("++ takes pointer out of range");
            return *this;
        }
        p++;
            return *this;
    }
    Ptr_to_T & operator++ (int) throw() { // postfix
        if(((char *)p - (char *)array) >= size) {
            throw std::out_of_range("++ takes pointer out of range");
            return *this;
        }
        p++;
            return *this;
    }

    Ptr_to_T & operator-- () throw() { // prefix
        if(((char *)p - (char *)array) <= 0) {
            try {
                throw std::out_of_range("-- takes pointer out of range");
                return *this;
            }
            catch (const std::exception &e) {
                std::cout << "caught exception.." << e.what() << "\n";
            }
            return *this;
        }
        p--;
            return *this;
    }
    Ptr_to_T & operator-- (int) throw() {  // postfix
        if(((char *)p - (char *)array) <= 0) {
            try {
                throw std::out_of_range("-- takes pointer out of range");
                return *this;
            }
            catch (const std::exception &e) {
                std::cout << "caught exception.." << e.what() << "\n";
            }
        }
        p--;
            return *this;
    }

    T& operator* () { return *p; } ; // prefix
};

void term()
{
    std::cout << "Inside term.." << std::endl;
    try {
        ;
    }
    catch (const std::exception &e) {
        std::cout << "caught exception.." << e.what() << "\n";
    }
    catch (...) {
        std::cout << "all handled here!"<< std::endl;
    }
}

int main()
{
    int i[200];
    for(int k = 0; k < 200; k++) {
        i[k] = k;
    }
    std::set_terminate(term);
    try {
        Ptr_to_T<int> j(&i[0], i, 200);
        j++;
        std::cout << *j << std::endl;
        j--;
        std::cout << *j << std::endl;
        j--;
    }
    catch (const std::exception &e) {
        std::cout << "caught exception.." << e.what() << "\n";
    }
    catch (...) {
        std::cout << "all handled here!"<< std::endl;
    }
}
