#include "lexer.hpp"
#include "token.hpp"

#include <memory>
#include <utility>
#include <array>
#include <fstream>
#include <iostream>

/*
                    TO DO
    - Make two buffers instead of one and reload them
*/

Lexer* Lexer::instance = nullptr;

Lexer::Lexer(): buf_{},
                lexeme_begin_{buf_.begin()},
                forward_{buf_.begin()},
                words_{}
{
    reserve(word_uptr(new Word((int) Token::TokenNames::REM, "rem")));
    reserve(word_uptr(new Word((int) Token::TokenNames::INPUT, "input")));
    reserve(word_uptr(new Word((int) Token::TokenNames::LET, "let")));
    reserve(word_uptr(new Word((int) Token::TokenNames::PRINT, "print")));
    reserve(word_uptr(new Word((int) Token::TokenNames::GOTO, "goto")));
    reserve(word_uptr(new Word((int) Token::TokenNames::IF, "if")));
    reserve(word_uptr(new Word((int) Token::TokenNames::END, "end")));
}


Lexer* Lexer::get_instance() {
    if (!instance)
        return new Lexer();
    return instance; 
}

void Lexer::load_buf(const char* fname) {
    std::ifstream in;
    in.open(fname);
    if (in.is_open()) {
        int i = 0;
        while (!in.eof()) {
            buf_[i] = in.get();
            ++i;
        }
        buf_[i] = EOF;
    }
    in.close();
}


void Lexer::reserve(word_uptr word) {
    using std::string;
    string lexeme = word->get_lexeme();
    words_.insert(std::make_pair(lexeme, std::move(word)));
}

token_uptr Lexer::get_token() {
    auto isop = [](char c) {
        return c == '+' ||
               c == '-' ||
               c == '*' ||
               c == '/' ||
               c == '=' ||
               c == '>' ||
               c == '<';
    };

    while (*forward_ != EOF) {
        if (isalpha(*forward_)) {

        } else if (isalnum(*forward_)) {

        } else if (isop(*forward_)) {

        }
        else if (isblank(*forward_)) {
            ++forward_;
        }
    }
    
}