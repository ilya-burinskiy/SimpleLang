#ifndef COMPILER_HPP_
#define COMPILER_HPP_

#include "lexer.hpp"
#include "ll1parser.hpp"
#include "astnode.hpp"

class Compiler {
private:
    static Compiler* instance;
    LL1Parser* parser_;

    Compiler(const char* fname);

    void semantic_rule_();
public:
    static Compiler* get_instance(const char* fname);
    void compile();



    Compiler(const Compiler&) = delete;
    Compiler& operator=(const Compiler&) = delete;
};

#endif