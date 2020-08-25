#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <memory>
#include <iostream>
#include "symbols.hpp"


struct Token {
    Symbol term;

    explicit Token(Symbol t);
    Token(const Token&) = default;
    Token(Token &&) = default;

    Token& operator=(const Token&) = default;
    Token& operator=(Token&&) = default;
    virtual ~Token() = default;
};
#endif