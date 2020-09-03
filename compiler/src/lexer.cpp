#include "lexer.hpp"
#include "token.hpp"
#include "symbols.hpp"

#include <memory>
#include <utility>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

Lexer* Lexer::instance = nullptr;

Lexer::Lexer(const char* fname): buf_{},
                                 line_{1},
                                 lexeme_begin_{buf_.end()},
                                 forward_{buf_.end()},
                                 words_{},
                                 state_{0},
                                 prev_term_(EPS)
{
    reserve(Word(INPUT, "input"));
    reserve(Word(LET,   "let"));
    reserve(Word(PRINT, "print"));
    reserve(Word(GOTO,  "goto"));
    reserve(Word(IF,    "if"));

    load_buf(fname);
}


Lexer* Lexer::get_instance(const char* fname) {
    if (!instance)
        return new Lexer(fname);
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

ushort Lexer::get_line_num()
{
    return line_;
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

void Lexer::skip_blank() 
{
    char c;
    while (true) 
    {
        if (state_ == 1) {
            c = next_char();
            if (isws(c)) 
                state_ = 1;
                if (c == '\n') ++line_;
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
            if (isws(c)) {
                if (c == '\n') ++line_;
                state_ = 1;
            } else if (isdigit(c)) {
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
                if (c == '(') {
                    prev_term_ = LB;
                    return new Token(LB);
                } else {
                    prev_term_ = RB;
                    return new Token(RB);
                }
            } else if (c == ';') {
                prev_term_ = DELIM;
                state_ = 0;
                return new Token(DELIM);
            } else if (c == ':') {
                prev_term_ = COLON;
                state_ = 0;
                return new Token(COLON);
            } else if (EOF == c) {
                return new Token(END);
            }
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
        }
    }
}

Token* Lexer::get_num() 
{
    using std::string;
    using std::stoi;

    char c;
    while (true) 
    {
        if (state_ == 2) 
        {
            c = next_char();
            if (isdigit(c))
                state_ = 2;
            else {
                prev_term_ = NUM;
                state_ = 0;
                auto s = string(lexeme_begin_, forward_);
                --forward_;
                int val = stoi(s);
                return new Num(NUM, val);
            }
        }
    }
}

Token* Lexer::get_id() {
    using std::string;
    using std::unordered_map;

    char c;
    while (true) 
    {
        if (state_ == 3) 
        {
            c = next_char();
            if (isalnum(c) || c == '_')
                state_ = 3;
            else 
            {
                prev_term_ = ID;
                state_ = 0;
                auto s = string(lexeme_begin_, forward_);
                --forward_;
                auto w = words_.find(s);
                if (w != words_.end()) {
                    return  new Word(w->second);
                } else {
                    auto w = Word(ID, s);
                    reserve(w);
                    return new Word(ID, s);
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
            if (c == '<') {
                prev_term_ = RELOP;
                state_ = 6;
            } else if (c == '=') {
                state_ = 7;
            } else if (c == '>') {
                prev_term_ = RELOP;
                state_ = 8;
            } else if (c == '+') {
                prev_term_ = PLUS;
                state_ = 0;
                return new Token(PLUS);
            } else if (c == '-') {
                // check for unary minus
                state_ = 0;
                if (is_bin_ariph_op(prev_term_) || prev_term_ == ASSIGMENT
                                                || prev_term_ == LB
                                                || prev_term_ == RELOP 
                                                || prev_term_ == EPS)
                {
                    prev_term_ = UMINUS;
                    return new Token(UMINUS);
                }
                prev_term_ = MINUS;
                return new Token(MINUS);
            } else if (c == '*') {
                prev_term_ = MUL;
                state_ = 0;
                return new Token(MUL);
            } else if (c == '/') {
                prev_term_ = DIV;
                state_ = 0;
                return new Token(DIV);
            }

        break;

        case 6:
            c = next_char();
            state_ = 0;
            // <> op
            if (c == '>') {
                return new Relop(RELOP, Relop::NE);
            // <= op
            } else if (c == '=') {
                return new Relop(RELOP, Relop::LE);
            // < op 
            } else {
                --forward_;
                return new Relop(RELOP, Relop::LT);
            }
        break;

        case 7:
            c = next_char(); 
            state_ = 0;
            // == op
            if (c == '=') {
                prev_term_ = RELOP;
                return new Relop(RELOP, Relop::EQ);
            }
            // = op
            else {
                prev_term_ = ASSIGMENT;
                --forward_;
                return new Token(ASSIGMENT);
            }
        break;

        case 8:
            c = next_char();
            state_ = 0;
            // >= op
            if (c == '=')
                return new Relop(RELOP, Relop::GE);
            // > op
            else
                --forward_;
                return new Relop(RELOP, Relop::GT);
        }
    }
}