#ifndef OPERATION_HPP_
#define OPERATION_HPP_

#include "symbols.hpp"
#include "token.hpp"

struct Relop: public Token {
    typedef enum {
        LT, LE, EQ, NE, GT, GE,     // compare operations
    } RelopType ;

    RelopType type;

    Relop(Symbol tag, RelopType operation);
    Relop(const Relop&) = default;
    Relop(Relop&&) = default;

    Relop& operator=(const Relop&) = default;
    Relop& operator=(Relop&&) = default;
};
#endif