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
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    Lexer* lex = Lexer::get_instance();
// #ifndef NDEBUG
    // lex->load_buf("test.txt");
// #else
    lex->load_buf("../test.txt");
// #endif
    token_uptr tok;
    token_uptr tmp;
    Token* ptok;
    int tag;
    do {
        tok = lex->get_token();
        ptok = tok.get();
        tag = ptok->get_tag();
        Word* pw;
        Num* pnum;
        Operation* op;
        switch (tag) {

        case Token::TokenNames::NUM:
            pnum = dynamic_cast<Num*>(ptok);
            cout << pnum->get_tag() << ", " << pnum->get_value() << endl;
        break;

        case Token::TokenNames::ID:
            pw = dynamic_cast<Word*>(ptok);
            cout << pw->get_tag() << ", " << pw->get_lexeme() << endl;
        break;

        case Token::TokenNames::OP:
            op = dynamic_cast<Operation*>(ptok);
            cout << op->get_tag() << ", " << op->get_opname() << endl;
        break;

        // case Token::TokenNames::BRACE:
        // break;
        }
        tmp = std::move(tok);
        tok.release();
    } while (tmp->get_tag() != (int) Token::TokenNames::NONE);
    return 0;
}