#include <iostream>

template <typename T>
class STC {
    T old;
public:
    STC(T f) { old = set_unexpected(f); }
    ~STC() { set_unexpected = old; }
};
