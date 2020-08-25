#ifndef NUM_HPP_
#define NUM_HPP_

#include "symbols.hpp"
#include "token.hpp"

struct Num: public Token {
    short val;
    Num(Symbol tag, short val);

    Num(const Num&) = default;
    Num(Num&&) = default;

    Num& operator=(const Num&) = default;
    Num& operator=(Num&&) = default;
    
};
#endif