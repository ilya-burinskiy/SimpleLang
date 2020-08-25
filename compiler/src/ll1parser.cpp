#include "ll1parser.hpp"
#include "lexer.hpp"
#include "symbols.hpp"

#include <list>
#include <typeinfo>

#include <iostream>

using std::cout;
using std::endl;
using std::list;

LL1Parser* LL1Parser::instance = nullptr;

LL1Parser* LL1Parser::get_instance() 
{
    if (instance == nullptr)
        instance = new LL1Parser();
    return instance;
}


LL1Parser::LL1Parser(): ip_{nullptr}, tree_()
{
    stack_.push(Symbol::END);
    stack_.push(Symbol::P);
    curr_symbol_ = stack_.top();

    lex_ = Lexer::get_instance();
    lex_->load_buf("test.txt");
    ip_ = lex_->get_token();
    a_ = ip_->term;
}

void LL1Parser::error() 
{

}

AST LL1Parser::get_tree()
{
    return tree_;
}

LL1Parser::ParserState LL1Parser::M_() 
{

    switch (curr_symbol_) {

        case P:
        case L_:
            switch (a_) {
                case LET:   return DERIVE;    
                case ID:    return DERIVE;
                case IF:    return DERIVE;
                case GOTO:  return DERIVE;
                case PRINT: return DERIVE;
                case INPUT: return DERIVE;
                case END:   return DERIVE;
                default:    return ERROR;
            }
        break;

        case L:
            switch (a_) {
                case LET:   return DERIVE;    
                case ID:    return DERIVE;
                case IF:    return DERIVE;
                case GOTO:  return DERIVE;
                case PRINT: return DERIVE;
                case INPUT: return DERIVE;
                default:    return ERROR;
            }
        break;

    case D:
        switch (a_) {
            case LET: return DERIVE;
            default:  return ERROR;
        }
    break;

    case S:
        switch (a_) {
            case ID:    return DERIVE;
            case IF:    return DERIVE;
            case GOTO:  return DERIVE;
            case INPUT: return DERIVE;
            case PRINT: return DERIVE;
            default:    return ERROR;
        }
    break;

    case LAS:
        switch (a_) {
            case ID:    return DERIVE;
            default:    return ERROR;
        }
    break;

    case LA:
        switch (a_) {
            case ASSIGMENT: return DERIVE;
            case COLON:     return DERIVE;
            default:        return ERROR;
        }
    break;

    case IFS:
        switch (a_) {
            case IF:    return DERIVE;
            default:    return ERROR;
        }
    break;

    case GOTOS:
        switch (a_) {
            case GOTO:  return DERIVE;
            default:    return ERROR;
        }
    break;

    case PRS:
        switch (a_) {
            case PRINT:  return DERIVE;
            default:     return ERROR;
        }
    break;

    case INS:
        switch (a_) {
            case INPUT:  return DERIVE;
            default:     return ERROR;
        }
    break;

    case E:
        switch (a_) {
            case ID:    return DERIVE;
            case NUM:   return DERIVE;
            case MINUS: return DERIVE;
            case LB:    return DERIVE;
            default:    return ERROR;
        }
    break;

    case E_:
        switch (a_) {
            case PLUS:  return DERIVE;
            case MINUS: return DERIVE;
            case RELOP: return DERIVE; // e
            case RB:    return DERIVE; // e
            case GOTO:  return DERIVE; // e
            case DELIM: return DERIVE; // e
            default:    return ERROR;
        }
    break;

    case T:
        switch (a_) {
            case ID:    return DERIVE;
            case NUM:   return DERIVE;
            case LB:    return DERIVE;
            case MINUS: return DERIVE;
            default:    return ERROR;
        }
    break;

    case T_:
        switch (a_) {
        
            case MUL:   return DERIVE;
            case DIV:   return DERIVE;
            case PLUS:  return DERIVE; // e
            case MINUS: return DERIVE; // e
            case RELOP: return DERIVE; // e
            case RB:    return DERIVE; // e
            case GOTO:  return DERIVE; // e
            case DELIM: return DERIVE; // e
            default:    return ERROR;
        }
    break;

    case F:
        switch (a_) {
            case ID:    return DERIVE;
            case NUM:   return DERIVE;
            case MINUS: return DERIVE;
            case LB:    return DERIVE;
            default:    return ERROR;
        }
    break;
    }
}

void LL1Parser::insert_in_tree()
{
    switch (curr_symbol_) {
        case E:
            if (tree_.is_empty()) {
                ASTNode* expr  = new ASTNode(E);
                ASTNode* term  = new ASTNode(T);
                ASTNode* expr_ = new ASTNode(E_);

                tree_.insert_root(expr, {term, expr_});
            } else {
                ASTNode* term  = new ASTNode(T);
                ASTNode* expr_ = new ASTNode(E_);

                tree_.hang_to_curr_node({term, expr_});
            }
        break;

        case E_:
            switch (a_) {
                case PLUS: 
                {
                    ASTNode* plus  = new ASTNode(PLUS);
                    ASTNode* term  = new ASTNode(T);
                    ASTNode* expr_ = new ASTNode(E_);

                    tree_.hang_to_curr_node({plus, term, expr_});
                    tree_.push_up_curr_node();
                }
                break;

                case MINUS:
                {
                    ASTNode* minus = new ASTNode(MINUS);
                    ASTNode* term  = new ASTNode(T);
                    ASTNode* expr_ = new ASTNode(E_);

                    tree_.hang_to_curr_node({minus, term, expr_});
                    tree_.push_up_curr_node();
                }
                break;

                case RELOP:
                case RB:
                case GOTO:
                case DELIM:
                    tree_.push_up_curr_node();
                break;
            }
        break;

        case T:
        {
            ASTNode* factor = new ASTNode(F);
            ASTNode* term_  = new ASTNode(T_);

            tree_.hang_to_curr_node({factor, term_});
        }
        break;

        case T_:
            switch (a_) {
                case MUL:
                {
                    ASTNode* mul    = new ASTNode(MUL);
                    ASTNode* factor = new ASTNode(F);
                    ASTNode* term_  = new ASTNode(T_);

                    tree_.hang_to_curr_node({mul, factor, term_});
                    tree_.push_up_curr_node();
                }
                break;

                case DIV:
                {
                    ASTNode* div    = new ASTNode(DIV);
                    ASTNode* factor = new ASTNode(F);
                    ASTNode* term_  = new ASTNode(T_);

                    tree_.hang_to_curr_node({div, factor, term_});
                    tree_.push_up_curr_node();
                }
                break;

                case RELOP:
                case RB:
                case GOTO:
                case DELIM:
                case PLUS:
                case MINUS:
                    tree_.push_up_curr_node();
                break;
            }
        break;

        case F:
            switch (a_) {
                case NUM:
                case ID:
                {
                    Leaf* l = new Leaf(ip_);
                    
                    tree_.hang_to_curr_node({l});
                    tree_.push_up_curr_node();
                }
                break;

                case MINUS:
                {
                    ASTNode* minus = new ASTNode(MINUS);
                    ASTNode* factor = new ASTNode(F);

                    tree_.hang_to_curr_node({minus, factor});
                    tree_.push_up_curr_node();
                }
                break;

                case LB:
                {
                    ASTNode* lbrace = new ASTNode(LB);
                    ASTNode* expr = new ASTNode(E);
                    ASTNode* rbrace = new ASTNode(RB);

                    tree_.hang_to_curr_node({lbrace, expr, rbrace});
                    tree_.push_up_curr_node();
                }
                break;
            }
        break;
    }
}

void LL1Parser::derive() 
{
    switch (curr_symbol_) {

        case P:
            switch (a_) {
                case LET:       
                case ID:    
                case IF:    
                case GOTO:  
                case PRINT:
                case INPUT:
                    // prog -> line_
                    stack_.push(L_);
                break;
                case END:  
                    // prog -> e
                break;
            }
        break;

        case L_:
            switch (a_) {
                case LET:       
                case ID:    
                case IF:    
                case GOTO:  
                case PRINT: 
                case INPUT:
                    // line_ -> line line_
                    stack_.push(L_);
                    stack_.push(L);
                break;
                case END:   
                    // line_ -> e
                break;
            }
        break;

        case L:
            switch (a_) {
                case LET:
                    stack_.push(DELIM);
                    stack_.push(D);
                break;

                case ID:
                case IF:
                case GOTO:
                case INPUT:
                case PRINT:
                    stack_.push(S);
                break;
            }
        break;

    case S:
        switch (a_) {

            // stmt -> las
            case ID:
                stack_.push(LAS); 
            break;

            // stmt -> ifstm
            case IF:
                stack_.push(DELIM);
                stack_.push(IFS);
            break;

            // stmt -> gotostmt;
            case GOTO:
                stack_.push(DELIM);
                stack_.push(GOTOS);
            break;

            // stmt -> inputstmt;
            case INPUT:
                stack_.push(DELIM);
                stack_.push(INS);
            break;

            // stmt -> printstmt;
            case PRINT:
                stack_.push(DELIM);
                stack_.push(PRS);
            break;
        }
    break;

    case LAS:
        stack_.push(LA);
        stack_.push(ID);
    break;

    case LA:
        switch (a_) {
            case COLON:
                stack_.push(COLON);
            break;

            case ASSIGMENT:
                stack_.push(DELIM);
                stack_.push(E);
                stack_.push(ASSIGMENT);
            break;
        }
    break;

    case IFS:
        stack_.push(ID);
        stack_.push(GOTO);
        stack_.push(E);
        stack_.push(RELOP);
        stack_.push(E);
        stack_.push(IF);
    break;

    case GOTOS:
        stack_.push(ID);
        stack_.push(GOTO);
    break;

    case PRS:
        stack_.push(ID);
        stack_.push(PRINT);
    break;

    case INS:
        stack_.push(ID);
        stack_.push(INPUT);
    break;

    case D:
        switch (a_) {
        
        case LET:
            stack_.push(E);
            stack_.push(ASSIGMENT);
            stack_.push(ID);
            stack_.push(LET);
        break;

        }
    break;

    case E:
        switch (a_) {

        // expr -> term expr_
        case ID:
        case NUM:
        case MINUS:
        case LB:
            stack_.push(E_);
            stack_.push(T);
        break;
        }
    break;

    case E_:
        switch (a_) {
        
            case PLUS:
                stack_.push(E_);
                stack_.push(T);
                stack_.push(PLUS);
            break;
            case MINUS:
                stack_.push(E_);
                stack_.push(T);
                stack_.push(MINUS);
            break;

            // expr_ -> e
            case RELOP:
            case RB:
            case GOTO:
            case DELIM:
            break;
        }
    break;

    case T:
        switch (a_) {
        
        // term -> factor term_
        case ID:
        case NUM:
        case MINUS:
        case LB:
            stack_.push(T_);
            stack_.push(F);
        break;
        }
    break;

    case T_:
        switch (a_) {
        
            case MUL:
                stack_.push(T_);
                stack_.push(F);
                stack_.push(MUL);
            break;

            case DIV:
                stack_.push(T_);
                stack_.push(F);
                stack_.push(DIV);
            break;

            // term_ -> e
            case PLUS:
            case MINUS:
            case RELOP:
            case RB:
            case GOTO:
            case DELIM:
            // do nothing
            break;
        }
    break;

    case F:
        switch (a_) {
        
        // factor -> id | num | (expr) | -factor 
            case ID:
                stack_.push(ID);
            break;
            case NUM:
                stack_.push(NUM);
            break;
            case LB:
                stack_.push(RB);
                stack_.push(E);
                stack_.push(LB);
            break;
            case MINUS:
                stack_.push(F);
                stack_.push(MINUS);
            break;
        
        }
    break;

    }
}

void LL1Parser::parse() 
{
    while (curr_symbol_ != Symbol::END) {
        a_ = ip_->term;
        if (curr_symbol_ == a_) {
            stack_.pop();
            ip_ = lex_->get_token();
        }
        // error case
        else if (is_terminal(curr_symbol_)) {
            error();
        }

        // error case 
        else if (ERROR == M_()) {
            error();
        } 

        else if (SYNCH == M_()) {

        }

        else if (DERIVE == M_()) {
            insert_in_tree();
            stack_.pop();
            derive();
        }

        curr_symbol_ = stack_.top();
    }
}