#include "word.hpp"
#include <string>

const std::string& Word::get_lexeme() const {
    return lexeme_;
}

void Word::set_lexeme(const std::string& new_lex) {
    lexeme_ = new_lex;
}