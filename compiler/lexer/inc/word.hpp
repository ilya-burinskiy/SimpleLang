#ifndef WORD_HPP_
#define WORD_HPP_
#include "token.hpp"
#include <string>
#include <memory>

class Word;
using word_uptr = std::unique_ptr<Word>;


class Word: public Token {
private:
    std::string lexeme_;
public:
    Word(int tag, const std::string& str): Token(tag), lexeme_(str)
    {
    }

    Word(const Word&) = default;
    Word(Word &&) = default;
    Word& operator=(const Word&) = default;
    
    const std::string& get_lexeme() const;
    void set_lexeme(const std::string& new_lex);
};

#endif