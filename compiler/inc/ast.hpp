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
    ASTNode(const ASTNode& other);
    ASTNode& operator=(const ASTNode& other);
    virtual ~ASTNode() = default;
};

struct Leaf: ASTNode {
    Token* tok;

    Leaf(Symbol s, Token* t = nullptr);
    ~Leaf();

    Leaf(const Leaf& other);
    Leaf& operator=(const Leaf& other);
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

    AST(const AST& other);
    AST(AST&& other);

    AST& operator=(const AST& other);
    AST& operator=(AST&& other);

    ~AST();

    void insert_token(Token* tok);
    void insert_root(ASTNode* node);
    void push_up_curr_node();
    void hang_to_curr_node(std::list<ASTNode*>&& children);

    bool is_empty() const;
    ASTNode* get_curr_node();
    const ASTNode* get_root() const;
    ASTNode* get_root();

    void drop_ptrs();
};

void postorder_traverse(const ASTNode* start);
#endif