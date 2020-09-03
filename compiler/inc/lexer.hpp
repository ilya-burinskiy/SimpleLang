#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <array>
#include <fstream>

#include "token.hpp"

#define BUF_SIZE 4096

class Lexer {
private:
    std::array<char, BUF_SIZE>            buf_;
    ushort                                line_;
    std::array<char, BUF_SIZE>::iterator  lexeme_begin_;
    std::array<char, BUF_SIZE>::iterator  forward_;
    std::unordered_map<std::string, Word> words_;
    int                                   state_;
    Symbol                                prev_term_;
    Lexer(const char* fname);
    static Lexer* instance;

    bool isws(char c);
    char next_char();
    void skip_blank();
    Token* get_num();
    Token* get_id();
    Token* get_op();
    Token* get_none();

    
public:
    Lexer(const Lexer&) = delete;
    Lexer& operator=(const Lexer&) = delete;

    static Lexer* get_instance(const char* fname);
    void load_buf(const char* fname);
    
    Token* get_token();
    ushort get_line_num();
    void reserve(const Word& word);
    void reserve(Word&& word);
};
#endif