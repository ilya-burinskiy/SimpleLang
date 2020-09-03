#ifndef AST_HPP_
#define AST_HPP_

#include "symbols.hpp"
#include "token.hpp"
#include <list>
#include <iostream>

struct ASTNode {
    Symbol                        symbol;
    ASTNode*                      parent;
    std::list<ASTNode*>           children;
    std::list<ASTNode*>::iterator curr_child;

    ASTNode(Symbol s = EPS);
    ASTNode(const ASTNode&) = delete;
    ASTNode& operator=(const ASTNode&) = delete;

    virtual ~ASTNode() = default;
};

struct Leaf: ASTNode {
    Token* tok;

    Leaf(Symbol s, Token* t = nullptr);

    Leaf(const Leaf& other) = delete;
    Leaf& operator=(const Leaf& other) = delete;
    virtual ~Leaf();
};


class AST 
{
private:
    ASTNode* root_;
    ASTNode* curr_node_;

    void recursive_copy(const ASTNode* other);
    void recursive_delete(ASTNode* subtree_root);

public:
    AST();

    AST(const AST& other) = delete;
    AST(AST&& other);

    AST& operator=(const AST& other) = delete;
    AST& operator=(AST&& other);

    void insert_token(Token* tok);
    void insert_root(ASTNode* node);
    void push_up_curr_node();
    void hang_to_curr_node(const std::list<ASTNode*>& children);

    bool is_empty() const;
    ASTNode* get_curr_node();
    const ASTNode* get_root() const;
    ASTNode* get_root();

    void drop_ptrs();
    ~AST();
};

#endif