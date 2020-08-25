#include "symbols.hpp"
#include "astnode.hpp"

#include <ostream>

using std::ostream;
using std::cout;

bool is_terminal(Symbol s)
{
    return ((int) s >= 0 && (int) s < 17);
}

ostream& operator<<(ostream& os, Symbol s) {
    
    switch (s) {
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