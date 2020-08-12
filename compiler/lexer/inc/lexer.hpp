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
using hash_table = std::unordered_map<std::string, Word>;

class Lexer {
private:
    std::array<char, BUF_SIZE> buf_;
    std::array<char, BUF_SIZE>::iterator lexeme_begin_;
    std::array<char, BUF_SIZE>::iterator forward_;
    hash_table words_;
    int state_;

    Lexer();
    static Lexer* instance;

    bool isws(char c);
    char next_char();
    void skip_blank();
    token_uptr get_num();
    token_uptr get_id();
    token_uptr get_op();
    token_uptr get_none();

    
public:
    Lexer(const Lexer&) = delete;
    Lexer& operator=(const Lexer&) = delete;

    static Lexer* get_instance();
    void load_buf(const char* fname);
    
    token_uptr get_token();
    void reserve(const Word& word);
    void reserve(Word&& word);

    
};
#endif