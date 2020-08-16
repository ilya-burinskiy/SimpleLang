#ifndef LL1PARSER_HPP_
#define LL1PARSER_HPP_

#include "symbols.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include <list>
#include <stack>


class LL1Parser {
private:
    typedef enum {
        // EMPTY,  // empty table cell
        ERROR,
        DERIVE,
        SYNCH   // for synchronisation if error happened
    } ParserState;

    LL1Parser();
    Lexer*             lex_;
    std::stack<Symbol> stack_; 
    std::list<Token*>  scanned_tokens_;
    Token*             ip_;
    Symbol             curr_symbol_;
    Symbol             a_;

    static LL1Parser*  instance;

    inline bool isterminal_(Symbol s);
    ParserState M_(Symbol nonterm, Symbol term); // simulates a predictive parsing table
    void error();
    void synch();
    void derive(Symbol nonterm, Symbol term);

public:
    // friend void SCC::set_curr_tok(LL1Parser& p);

    static LL1Parser* get_instance();
    void set_curr_tok(Token*);
    void parse();

    LL1Parser(const LL1Parser&) = delete;
    LL1Parser& operator=(const LL1Parser&) = delete;
};

#endif