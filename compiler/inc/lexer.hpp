#ifndef LEXER_HPP_
#define LEXER_HPP_

#include "token.hpp"
#include "num.hpp"
#include "word.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <array>
#include <fstream>

#define BUF_SIZE 4096

class Lexer {
private:
    std::array<char, BUF_SIZE> buf_;
    std::array<char, BUF_SIZE>::iterator lexeme_begin_;
    std::array<char, BUF_SIZE>::iterator forward_;
    std::unordered_map<std::string, Word> words_;
    int state_;

    Lexer();
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

    static Lexer* get_instance();
    void load_buf(const char* fname);
    
    Token* get_token();
    void reserve(const Word& word);
    void reserve(Word&& word);
};
#endif