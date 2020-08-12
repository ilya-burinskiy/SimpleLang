#ifndef OPERATION_HPP_
#define OPERATION_HPP_

#include "token.hpp"

class Operation: public Token {
private:
    int name_;
public:
    Operation(int tag, int name): Token(tag), name_(name)
    {
    }

    enum OperationNames {
        PLUS, MINUS, DIV, MUL,      // ariphmetic operations
        LT, LE, EQ, NE, GT, GE,     // compare operations
        ASSIGMENT
    };

    int get_opname() const;
};
#endif