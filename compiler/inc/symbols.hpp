#ifndef SYMBOLS_HPP_
#define SYMBOLS_HPP_

#include <ostream>

// Don't change symbols positions because
// isterminal function in ll1parser class will not work
typedef enum {
    // Terminals:
    LET, INPUT, PRINT, GOTO, IF, 
    ID, NUM, PLUS, MINUS, MUL, DIV, ASSIGMENT,
    RELOP, LB, RB, DELIM, COLON,

    // Nonterminals:
    P,      // programm
    L_,     // line_ (left reqursion elimination)
    L,      // line
    D,      // definition
    S,      // statement
    LAS,    // label or assigment statemen
    IFS,    // if statement
    GOTOS,  // goto statememt 
    PRS,    // print stmt
    INS,    // input stmt
    LA,     // label or assigment
    LBL,    // label
    ASSGMT, // assigment
    E,      // expression
    E_,     // (left reqursion elimination)
    T,      // term 
    T_,     // (left reqursion elimination)
    F,      // factor 

    // special grammar symbpol $
    END,
} Symbol;


bool is_terminal(Symbol s);
std::ostream& operator<<(std::ostream& os, Symbol s);
#endif