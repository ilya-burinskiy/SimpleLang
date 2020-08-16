#ifndef SYMBOLS_HPP_
#define SYMBOLS_HPP_

// Don't change symbols positions because
// isterminal function in ll1parser class will not work
typedef enum {
    // Terminals:
    LET, INPUT, PRINT, GOTO, IF, 
    ID, NUM, PLUS, MINUS, MUL, DIV, ASSIGMENT,
    RELOP, LB, RB, DELIM,

    // Nonterminals:
    // prog, lines, lines_, line, linebody, def, stmt
    // cmpexpr, rval, expr, expr_, term, term_, factor, negfactor
    P, LS, L, LBODY, D, S, C,
    E, E_, T, T_, F,

    // special grammar symbpol $
    END,
} Symbol;
#endif