#ifndef COMPILER_HPP_
#define COMPILER_HPP_

#include "lexer.hpp"
#include "ll1parser.hpp"
#include "ast.hpp"

#include <unordered_map>
#include <string>

class Compiler {
private:
    static Compiler* instance;
    LL1Parser* parser_;

    ushort high_adr_;
    ushort bias_;
    std::unordered_map<std::string, ushort> sym_table_;


    Compiler(const char* fname);
    void apply_semantic_rules(ASTNode* node);
    void undefined_symbol(const std::string& s); 

public:
    static Compiler* get_instance(const char* fname);
    void compile();

    Compiler(const Compiler&) = delete;
    Compiler& operator=(const Compiler&) = delete;
};

#endif