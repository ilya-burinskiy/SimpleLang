#include "symbols.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include "ll1parser.hpp"
#include "astnode.hpp"

#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    LL1Parser* parser = LL1Parser::get_instance();
    parser->parse();

    AST tree = parser->get_tree();
    ASTNode* start = tree.get_root();
    postorder_traverse(start);
    cout << endl;
}