#ifndef BRACE_HPP_
#define BRACE_HPP_
#include "token.hpp"

class Brace: public Token {
private:
    int type_;
public:
    Brace(int tag, int type): Token(tag), type_(type)
    {
    }

    enum BraceNames {
        LB, RB
    };
};
#endif