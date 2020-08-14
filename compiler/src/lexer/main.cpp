#include "lexer.hpp"
#include "token.hpp"
#include "operation.hpp"
#include "symbols.hpp"

#include <memory>
#include <utility>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    Lexer* lex = Lexer::get_instance();
#ifndef NDEBUG
    lex->load_buf("test.txt");
#else
    lex->load_buf("../test.txt");
#endif
    Token* tok;
    Symbol term;
    int tag;
    do {
        tok = lex->get_token();
        term = tok->term;
        cout << *tok << endl;
        delete tok;
    } while (term != Symbol::NONE);

    return 0;
}