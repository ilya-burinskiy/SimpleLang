#ifndef SYMBOLS_HPP_
#define SYMBOLS_HPP_

typedef enum {
    // Terminals:
    LET, INPUT, PRINT, GOTO, IF,      // reserved terminals
    ID, NUM,                          
    OP, RELOP,                        // ariphmetic and relation operations

    LB, RB,                           // left brace, right brace
    NONE,                             // end of token stream ($)

    /*
        Nonterminals:
        prog, lines, lines_, line, linebody, def, stmt
        cmpexpr, rval, expr, expr_, term, term_, factor, negfactor
    */
    P, LS, LS_, L, LBODY, D, S, C, R, 
    E, E_, T, T_, F, NF
        
} Symbol;
#endif