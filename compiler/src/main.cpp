#include "symbols.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include "ll1parser.hpp"

int main(int argc, char** argv) {
    LL1Parser* parser = LL1Parser::get_instance();
    parser->parse();
}