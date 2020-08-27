#ifndef LL1PARSER_HPP_
#define LL1PARSER_HPP_

#include "symbols.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include "astnode.hpp"
#include <list>
#include <stack>


class LL1Parser 
{
private:

    typedef enum {
        ERROR,
        DERIVE,
        SYNCH   // for synchronisation if error happened
    } ParserState;

    LL1Parser(const char* fname);
    Lexer*             lex_;
    std::stack<Symbol> stack_; 
    Token*             ip_;
    Symbol             curr_symbol_;
    Symbol             a_;
    AST                tree_;
    static LL1Parser*  instance;

    ParserState M_(); // simulates a predictive parsing table
    void error();
    void synch();
    void derive();
    void insert_in_tree();

public:
    static LL1Parser* get_instance(const char* fname);

    void set_curr_tok(Token*);
    void parse();
    AST get_tree();

    LL1Parser(const LL1Parser&) = delete;
    LL1Parser& operator=(const LL1Parser&) = delete;

    ~LL1Parser();
};

#endif