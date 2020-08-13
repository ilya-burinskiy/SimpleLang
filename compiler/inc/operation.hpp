#ifndef OPERATION_HPP_
#define OPERATION_HPP_

#include "symbols.hpp"
#include "token.hpp"

struct Operation: public Token {
    enum Op {
        PLUS, MINUS, DIV, MUL,      // ariphmetic operations
        LT, LE, EQ, NE, GT, GE,     // compare operations
        ASSIGMENT
    };

    Op optype;

    Operation(Symbols::Terminal tag, Op operation);
    Operation(const Operation&) = default;
    Operation(Operation&&) = default;

    Operation& operator=(const Operation&) = default;
    Operation& operator=(Operation&&) = default;
};
#endif