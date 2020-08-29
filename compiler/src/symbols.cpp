#include "symbols.hpp"

#include <iostream>

using std::ostream;
using std::cout;
using std::endl;

bool is_terminal(Symbol s)
{
    return ((int) s >= 0 && (int) s < 18);
}

bool is_bin_ariph_op(Symbol s)
{
    return s == PLUS || s == MINUS ||
           s == MUL  || s == DIV;
}

ostream& operator<<(ostream& os, Symbol s) {
    
    switch (s) {
        case ID:    os << "ID"; break;
        case NUM:   os << "NUM"; break;
        case LET:   os << "LET"; break;
        case IF:    os << "IF"; break;
        case GOTO:  os << "GOTO"; break;
        case PRINT: os << "PRINT"; break;
        case INPUT: os << "INPUT"; break;
        case PLUS:  os << '+'; break;
        case ASSIGMENT: os << '='; break;
        case COLON: os << ':' << endl; break;
        case DELIM: os << ';' << endl; break;

        case Symbol::MINUS:
        case Symbol::UMINUS:
            os << '-';
        break;

        case MUL:   os << '*'; break;
        case DIV:   os << '/'; break;
        case RELOP: os << "RELOP"; break;
        case LB:    os << '('; break;
        case RB:    os << ')'; break;
    }

    return os;
}