#include "token.hpp"
#include "symbols.hpp"
#include <iostream>

using std::ostream;

Token::Token(Symbol t): term{t} 
{
}

ostream& operator<<(ostream& os, const Token& tok) {
    switch (tok.term) {
    case Symbol::ID:
        os << "ID";
    break;
    case Symbol::NUM:
        os << "NUM";
    break;
    case Symbol::LET:
        os << "LET";
    break;
    case Symbol::IF:
        os << "IF";
    break;
    case Symbol::GOTO:
        os << "GOTO";
    break;
    case Symbol::PRINT:
        os << "PRINT";
    break;
    case Symbol::INPUT:
        os << "INPUT";
    break;
    case Symbol::PLUS:
        os << "PLUS";
    break;
    case Symbol::MINUS:
        os << "MINUS";
    break;
    case Symbol::MUL:
        os << "MUL";
    break;
    case Symbol::DIV:
        os << "DIV";
    break;
    case Symbol::RELOP:
        os << "RELOP";
    break;
    case Symbol::LB:
        os << "LB";
    break;
    case Symbol::RB:
        os << "RB";
    break;
    case Symbol::END:
        os << "END";
    break;
    }
}