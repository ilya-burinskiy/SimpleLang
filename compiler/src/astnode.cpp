#include "astnode.hpp"
#include "symbols.hpp"
#include "token.hpp"
#include "num.hpp"
#include "word.hpp"

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

ASTNode::~ASTNode()
{
}

Leaf::Leaf(Token* t): ASTNode(t->term), tok(t)
{
}

Leaf::~Leaf()
{
    // delete tok;
}

AST::AST(): root_{nullptr},
            curr_node_{nullptr}
{
}

void AST::delete_sub_tree(ASTNode* subtree_root)
{
    // for (auto child: subtree_root->children)
    //     delete_sub_tree(child);
    // delete subtree_root;
}

AST::~AST()
{
    // delete_sub_tree(root_);
}

bool AST::is_empty() const {
    return root_ == nullptr;
}

ASTNode* AST::get_curr_node()
{
    return curr_node_;
}

ASTNode* AST::get_root(){
    return root_;
}
void AST::push_up_curr_node()
{
    bool is_node_found = false;
    while (!is_node_found) {

        // find parent with curr_child not pointing to children.end()
        while (curr_node_->curr_child == curr_node_->children.end()
                                      && curr_node_ != root_)
        {
            curr_node_ = curr_node_->parent;
        }
        

        // find first nonterminal child
        // if node has children we must find node to wich we can hang another nodes
        while (curr_node_->curr_child != curr_node_->children.end()
               && is_terminal( (*(curr_node_->curr_child))->symbol) )
        {
            ++(curr_node_->curr_child);
        }
        
        // such node is founded
        if (curr_node_->curr_child != curr_node_->children.end()) {
            ASTNode* prev_child = *(curr_node_->curr_child);
            ++(curr_node_->curr_child);
            curr_node_ = prev_child;
            is_node_found = true;

        // search in parent
        } else if (curr_node_ != root_) {
            is_node_found = false;
        } else 
            return;

    }
}

void AST::insert_root(ASTNode* root, list<ASTNode*>&& children) 
{
    if (root_ == nullptr)
        root_ = curr_node_ = root;
    hang_to_curr_node(move(children));
}

void AST::hang_to_curr_node(list<ASTNode*>&& children)
{
    curr_node_->children = move(children);
    curr_node_->curr_child = curr_node_->children.begin();
    for (auto child: curr_node_->children)
        child->parent = curr_node_;

    ASTNode* prev_child = *(curr_node_->curr_child);
    ++(curr_node_->curr_child);
    curr_node_ = prev_child;
}


void postorder_traverse(ASTNode* start_node)
{
    for (auto child: start_node->children) {
        postorder_traverse(child);
    }

    if (is_terminal(start_node->symbol))
        cout << start_node->symbol << ' ' << flush;
}
