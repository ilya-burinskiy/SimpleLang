#include "lexer.hpp"
#include "token.hpp"
#include "operation.hpp"
#include "brace.hpp"

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
*/
using Tok = Token::TokenNames;
using Br  = Brace::BraceNames;
using Op  = Operation::OpNames;

Lexer* Lexer::instance = nullptr;

Lexer::Lexer(): buf_{},
                lexeme_begin_{buf_.end()},
                forward_{buf_.end()},
                words_{},
                state_{0}
{
    reserve(Word((int) Tok::INPUT, "input"));
    reserve(Word((int) Tok::LET, "let"));
    reserve(Word((int) Tok::PRINT, "print"));
    reserve(Word((int) Tok::GOTO, "goto"));
    reserve(Word((int) Tok::IF, "if"));
    reserve(Word((int) Tok::END, "end"));
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
    string lexeme = word.get_lexeme();
    words_.insert(std::make_pair(lexeme, word));
}

void Lexer::reserve(Word&& word) {
    using std::string;
    string lexeme = word.get_lexeme();
    words_.insert(std::make_pair(lexeme, std::move(word)));
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

    auto isbrace = [](char c) {
        return c == '(' || c == ')';
    };
    
    char c;
    Token* res;
    while (true) {

        switch (state_) {
        case 0:
            c = next_char();
            if (isws(c))
                state_ = 1;
            else if (isdigit(c)) {
                state_ = 2;
                lexeme_begin_ = forward_;
            } else if (c == 'r') {
                state_ = 9;
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
                    return token_uptr(new Brace(
                        (int) Tok::BRACE,
                        (int) Br::LB)
                    );
                else
                    return token_uptr(new Brace(
                        (int) Tok::BRACE,
                        (int) Br::RB)
                    );
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
            return token_uptr(new Token((int) Tok::NONE));

        // skip comment
        case 9:
            c = next_char();
            if (c == 'e') {
                state_ = 10;
            } else {
                state_ = 3;
            }
        break;

        case 10:
            c = next_char();
            if (c == 'm') {
                state_ = 11;
            } else {
                state_ = 3;
            }
        break;

        case 11:
            c = next_char();
            if (c != '\n')
                state_ = 11;
            else {
                state_ = 0;
                --forward_;
            }
        break;
        }
    }
}

token_uptr Lexer::get_num() {
    char c;
    Token *res;
    while (true) {
        if (state_ == 2) {
            c = next_char();
            if (isdigit(c))
                state_ = 2;
            else {
                std::string number(lexeme_begin_, forward_);
                state_ = 0;
                --forward_;

                res = new Num((int) Tok::NUM, std::stoi(number));
                return token_uptr(res);
            }
        }
    }
}

token_uptr Lexer::get_id() {
    char c;
    Token *res;
    while (true) {
        if (state_ == 3) {
            c = next_char();
            if (isalnum(c) || c == '_')
                state_ = 3;
            else {
                std::string s = std::string(lexeme_begin_, forward_);
                state_ = 0;
                --forward_;

                hash_table::iterator w = words_.find(s);
                if (w != words_.end()) {
                    res = new Word(std::move(w->second));
                    return token_uptr(res);
                } else {
                    Word w = Word((int) Tok::ID, s);
                    reserve(w);
                    res = new Word(std::move(w));
                    return token_uptr(res);
                }
            }
        }
    }
}

token_uptr Lexer::get_op() {
    Token* res;
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
                return token_uptr(new Operation(
                    (int) Tok::OP,
                    (int) Op::PLUS)
                );
            } else if (c == '-') {
                state_ = 0;
                return token_uptr(new Operation(
                    (int) Tok::OP,
                    (int) Op::MINUS)
                );
            } else if (c == '*') {
                state_ = 0;
                return token_uptr(new Operation(
                    (int) Tok::OP,
                    (int) Op::MUL)
                );
            } else if (c == '/') {
                state_ = 0;
                return token_uptr(new Operation(
                    (int) Tok::OP,
                    (int) Op::DIV)
                );
            }

        break;

        case 6:
            c = next_char();
            state_ = 0;
            // <> op
            if (c == '>') {
                return token_uptr(new Operation(
                    (int) Tok::OP,
                    (int) Op::NE)
                );
            // <= op
            } else if (c == '=') {
                return token_uptr(new Operation(
                    (int) Tok::OP,
                    (int) Op::LE)
                );
            // < op 
            } else {
                --forward_;
                return token_uptr(new Operation(
                    (int) Tok::OP,
                    (int) Op::LT)
                );
            }
        break;

        case 7:
            c = next_char(); 
            state_ = 0;
            // == op
            if (c == '=')
                return token_uptr(new Operation(
                    (int) Tok::OP,
                    (int) Op::EQ)
                );
            // = op
            else {
                --forward_;
                return token_uptr(new Operation(
                    (int) Tok::OP,
                    (int) Op::ASSIGMENT)
                );
            }
        break;

        case 8:
            c = next_char();
            state_ = 0;
            // >= op
            if (c == '=')
                return token_uptr(new Operation(
                    (int) Tok::OP,
                    (int) Op::GE)
                );
            // > op
            else
                --forward_;
                return token_uptr(new Operation(
                    (int) Tok::OP,
                    (int) Op::GT)
                );
        }
    }
}