#include "lexer.hpp"

int main(int argc, char** argv) {
    Lexer* lex = Lexer::get_instance();
    lex->load_buf("test.txt");
    lex->get_token();

    return 0;
}