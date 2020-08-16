#include "lexer.hpp"
#include "token.hpp"
#include "relop.hpp"
#include "symbols.hpp"

#include <memory>
#include <utility>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

/*
                    TO DO
    - Make two buffers instead of one and reload them.
    - Handle errors.
    - Skip comments
*/

Lexer* Lexer::instance = nullptr;

Lexer::Lexer(): buf_{},
                lexeme_begin_{buf_.end()},
                forward_{buf_.end()},
                words_{},
                state_{0}
{
    reserve(Word(Symbol::INPUT, "input"));
    reserve(Word(Symbol::LET,   "let"));
    reserve(Word(Symbol::PRINT, "print"));
    reserve(Word(Symbol::GOTO,  "goto"));
    reserve(Word(Symbol::IF,    "if"));
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


void Lexer::reserve(const Word& word) {
    using std::string;
    using std::make_pair;
    words_.insert(make_pair(word.lexeme, word));
}

void Lexer::reserve(Word&& word) {
    using std::string;
    using std::make_pair;
    using std::move;
    words_.insert(make_pair(word.lexeme, move(word)));
}

char Lexer::next_char() {
    if (forward_ == buf_.end()) {
        forward_ = buf_.begin();
        return *forward_;
    }
    return *(++forward_);
}

bool Lexer::isws(char c) {
    return c == ' ' || c == '\n' ||
           c == '\t';
}

void Lexer::skip_blank() {
    char c;
    while (true) {
        if (state_ == 1) {
            c = next_char();
            if (isws(c)) 
                state_ = 1;
            else {
                state_ = 0;
                --forward_;
                return;
            }
        }
    }
}

Token* Lexer::get_token() {

    auto isop = [](char c) {
        return c == '+' ||
               c == '-' ||
               c == '*' ||
               c == '/' ||
               c == '=' ||
               c == '>' ||
               c == '<';
    };

    auto isbrace = [](char c) {
        return c == '(' || c == ')';
    };
    
    char c;
    while (true) {

        switch (state_) {
        case 0:
            c = next_char();
            if (isws(c))
                state_ = 1;
            else if (isdigit(c)) {
                state_ = 2;
                lexeme_begin_ = forward_;
            } else if (isalpha(c) || c == '_') {
                state_ = 3;
                lexeme_begin_ = forward_;
            } else if (isop(c)) {
                state_ = 4;
                lexeme_begin_ = forward_;
                --forward_;
            } else if (isbrace(c)) {
                state_ = 0;
                if (c == '(')
                    return new Token(Symbol::LB);
                else
                    return new Token(Symbol::RB);
            } else if (c == ';') {
                return new Token(Symbol::DELIM);
            } else if (EOF == c)
                state_ = 5;
        break;

        // blank case
        case 1:
            skip_blank();
        break;  

        // number case
        case 2:
            return get_num();

        // id case
        case 3:
            return get_id();
        
        // op case
        case 4:
            return get_op();
        
        // EOF case:
        case 5:
            // Buffers might be reloaded here if eof at
            // the end of one of them and if is not
            // return that there is no tokens
            return new Token(Symbol::END);
        }
    }
}

Token* Lexer::get_num() {
    using std::string;
    using std::stoi;

    char c;
    while (true) {
        if (state_ == 2) {
            c = next_char();
            if (isdigit(c))
                state_ = 2;
            else {
                state_ = 0;
                string s(lexeme_begin_, forward_);
                --forward_;
                int val = stoi(s);
                return new Num(Symbol::NUM, val);
            }
        }
    }
}

Token* Lexer::get_id() {
    using std::string;
    using std::unordered_map;

    char c;
    while (true) {
        if (state_ == 3) {
            c = next_char();
            if (isalnum(c) || c == '_')
                state_ = 3;
            else {
                state_ = 0;
                string s = string(lexeme_begin_, forward_);
                --forward_;
                unordered_map<string, Word>::iterator w = words_.find(s);
                if (w != words_.end()) {
                    return  new Word((w->second));
                } else {
                    Word w = Word(Symbol::ID, s);
                    reserve(w);
                    return new Word(Symbol::ID, s);
                }
            }
        }
    }
}

Token* Lexer::get_op() {
    char c;
    while (true) {
        switch (state_) {
        case 4:
            c = next_char();
            if (c == '<')
                state_ = 6;
            else if (c == '=')
                state_ = 7;
            else if (c == '>')
                state_ = 8;
            else if (c == '+') {
                state_ = 0;
                return new Token(Symbol::PLUS);
            } else if (c == '-') {
                state_ = 0;
                return new Token(Symbol::MINUS);
            } else if (c == '*') {
                state_ = 0;
                return new Token(Symbol::MUL);
            } else if (c == '/') {
                state_ = 0;
                return new Token(Symbol::DIV);
            }

        break;

        case 6:
            c = next_char();
            state_ = 0;
            // <> op
            if (c == '>') {
                return new Relop(Symbol::RELOP, Relop::NE);
            // <= op
            } else if (c == '=') {
                return new Relop(Symbol::RELOP, Relop::LE);
            // < op 
            } else {
                --forward_;
                return new Relop(Symbol::RELOP, Relop::LT);
            }
        break;

        case 7:
            c = next_char(); 
            state_ = 0;
            // == op
            if (c == '=')
                return new Relop(Symbol::RELOP, Relop::EQ);
            // = op
            else {
                --forward_;
                return new Token(Symbol::ASSIGMENT);
            }
        break;

        case 8:
            c = next_char();
            state_ = 0;
            // >= op
            if (c == '=')
                return new Relop(Symbol::RELOP, Relop::GE);
            // > op
            else
                --forward_;
                return new Relop(Symbol::RELOP, Relop::GT);
        }
    }
}