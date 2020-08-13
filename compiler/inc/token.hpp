#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <memory>
#include "symbols.hpp"


struct Token {
    Symbols::Terminal tag;

    explicit Token(Symbols::Terminal t);

    Token(const Token&) = default;
    Token(Token &&) = default;

    Token& operator=(const Token&) = default;
    Token& operator=(Token&&) = default;
    virtual ~Token() = default;
};
#endif