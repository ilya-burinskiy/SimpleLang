#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <memory>
#include <iostream>
#include <string>

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


struct Word: public Token {
    std::string lexeme;

    Word(Symbol tag, const std::string& str);

    Word(const Word&) = default;
    Word(Word&&) = default;
    Word& operator=(const Word&) = default;
    Word& operator=(Word&&) = default;
    virtual ~Word() = default;
};

struct Num: public Token {
    short val;
    Num(Symbol tag, short val);

    Num(const Num&) = default;
    Num(Num&&) = default;

    Num& operator=(const Num&) = default;
    Num& operator=(Num&&) = default;
    
    virtual ~Num() = default;
};

struct Relop: public Token {
    typedef enum {
        LT, LE, EQ, NE, GT, GE,     // compare operations
    } RelopType;

    RelopType type;

    Relop(Symbol tag, RelopType operation);
    Relop(const Relop&) = default;
    Relop(Relop&&) = default;

    Relop& operator=(const Relop&) = default;
    Relop& operator=(Relop&&) = default;
    virtual ~Relop() = default;
};
#endif