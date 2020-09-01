#include "ast.hpp"
#include "symbols.hpp"
#include "token.hpp"

#include <list>
#include <iostream>

using std::list;
using std::move;
using std::cout;
using std::flush;

ASTNode::ASTNode(Symbol s): symbol(s),
                            parent(nullptr),
                            children(),
                            curr_child(children.begin())
{
}

ASTNode::ASTNode(const ASTNode& other)
{
    symbol = other.symbol;
}

ASTNode& ASTNode::operator=(const ASTNode& other)
{
    symbol = other.symbol;
    return *this;
}

Leaf::Leaf(Symbol s, Token* t): ASTNode(s), tok(t)
{
}

Leaf::Leaf(const Leaf& other): ASTNode(other)
{
    tok = new Token(*(other.tok));
}

Leaf& Leaf::operator=(const Leaf& other)
{
    if (this == &other)
        return *this;

    ASTNode::operator=(other);
    delete tok;
    tok = new Token(*(other.tok));
    return *this;
}

Leaf::~Leaf()
{
    if (tok != nullptr)
        delete tok;
}

AST::AST(): root_{nullptr},
            curr_node_{nullptr}
{
}


AST::AST(AST&& other)
{
    curr_node_ = root_ = other.get_root();
    other.drop_ptrs();
}


AST& AST::operator=(AST&& other)
{
    if (this == & other)
        return *this;
    
    root_ = curr_node_ = other.get_root();
    other.drop_ptrs();
    return *this; 
}

void AST::recursive_delete(ASTNode* subtree_root)
{
    for (auto child: subtree_root->children)
        recursive_delete(child);
    delete subtree_root;
}

AST::~AST()
{
    if (root_ != nullptr)
        recursive_delete(root_);
}

bool AST::is_empty() const 
{
    return root_ == nullptr;
}

ASTNode* AST::get_curr_node()
{
    return curr_node_;
}

const ASTNode* AST::get_root() const
{
    return root_;
}

ASTNode* AST::get_root()
{
    return root_;
}

void AST::drop_ptrs()
{
    root_ = curr_node_ = nullptr;
}

void AST::push_up_curr_node()
{

    while (curr_node_->curr_child == curr_node_->children.end()
                                      && curr_node_ != root_)
        curr_node_ = curr_node_->parent;
    
    if (curr_node_->curr_child != curr_node_->children.end()) {
        auto prev_child = *(curr_node_->curr_child);
        ++(curr_node_->curr_child);
        curr_node_ = prev_child;
    }
}

void AST::insert_token(Token* tok)
{
    auto leaf = static_cast<Leaf*>(curr_node_);
    leaf->tok = tok;
    push_up_curr_node();
}

void AST::insert_root(ASTNode* root) 
{
    if (root_ == nullptr)
        root_ = curr_node_ = root;
}

void AST::hang_to_curr_node(const list<ASTNode*>& children)
{
    for (auto child: children) {
        curr_node_->children.push_back(child);
        child->parent = curr_node_;
    }
    curr_node_->curr_child = curr_node_->children.begin();

    auto prev_child = *(curr_node_->curr_child);
    ++(curr_node_->curr_child);
    curr_node_ = prev_child;
}

