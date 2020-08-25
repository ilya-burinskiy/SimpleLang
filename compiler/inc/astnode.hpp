#ifndef ASTNODE_HPP_
#define ASTNODE_HPP_

#include "symbols.hpp"
#include "token.hpp"
#include <list>
#include <iostream>

struct ASTNode {
    Symbol                        symbol;
    ASTNode*                      parent;
    std::list<ASTNode*>           children;
    std::list<ASTNode*>::iterator curr_child;

    ASTNode(Symbol s);
    ASTNode(const ASTNode& other) = delete;
    ASTNode& operator=(const ASTNode& other) = delete;
    virtual ~ASTNode();
};

struct Leaf: ASTNode {
    Token* tok;
    Leaf(Token* t);
    ~Leaf();
};


class AST 
{
private:
    ASTNode* root_;
    ASTNode* curr_node_;
public:
    AST();
    ~AST();
    void delete_sub_tree(ASTNode* subtree_root);
    void insert_root(ASTNode* root, std::list<ASTNode*>&& children);
    void hang_to_curr_node(std::list<ASTNode*>&& children);
    void push_up_curr_node();
    void postorder_traverse(ASTNode* start) const;
    bool is_empty() const;
    ASTNode* get_curr_node();
    ASTNode* get_root();
};

void postorder_traverse(ASTNode* start);
#endif