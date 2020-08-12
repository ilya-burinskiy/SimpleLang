#ifndef NUM_HPP_
#define NUM_HPP_
#include <token.hpp>

class Num: public Token {
private:
    int value_;
public:
    Num(int tag, int val): Token(tag), value_{val}
    {
    }

    Num(const Num&) = default;
    Num& operator=(const Num&) = default;
    
    int get_value() const;
    void set_value(int new_val);
};
#endif