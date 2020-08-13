#ifndef SYMBOLS_HPP_
#define SYMBOLS_HPP_

namespace Symbols {

    enum Terminal {
        LET, INPUT, PRINT, GOTO, IF, END, // reserved terminals
        ID, NUM,                          
        OP, RELOP,                        // ariphmetic operations
                                          // and relation operations

        LB, RB,                           // left brace, right brace
        NONE,                             // end of token stream
    };

    enum Nonterminal {
        PROG, DEFS, DEF, STMTS, STMT
        
    };
}
#endif