#ifndef WORD_HPP_
#define WORD_HPP_

#include "token.hpp"
#include "symbols.hpp"

#include <string>
#include <memory>


struct Word: public Token {
    std::string lexeme;

    Word(Symbol tag, const std::string& str);

    Word(const Word&) = default;
    Word(Word&&) = default;
    Word& operator=(const Word&) = default;
    Word& operator=(Word&&) = default;
};

#endif