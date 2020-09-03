#include "ll1parser.hpp"
#include "lexer.hpp"
#include "symbols.hpp"

#include <list>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::list;
using std::string;
using std::stringstream;

LL1Parser* LL1Parser::instance = nullptr;

LL1Parser* LL1Parser::get_instance(const char* fname) 
{
    if (instance == nullptr)
        instance = new LL1Parser(fname);
    return instance;
}


LL1Parser::LL1Parser(const char* fname): ip_{nullptr}, tree_()
{
    stack_.push(Symbol::END);
    stack_.push(Symbol::P);
    curr_symbol_ = stack_.top();

    lex_ = Lexer::get_instance(fname);
    ip_ = lex_->get_token();
    a_ = ip_->term;
}

void LL1Parser::error()
{
    stringstream ss;
    auto line = lex_->get_line_num();
    ss << "Error in line " << line;

    // this token woldn't be inserted is ast so it deleted
    delete ip_; 
    tree_.~AST();

    throw string(ss.str());
}

AST& LL1Parser::get_tree()
{
    return tree_;
}

/**
 * Parse table
 * 
 * Algorithm for constructin parse table:
 * For each grammar production A -> alpha do
 *      For each terminal a from FIRST(alpha) add A -> alpha to M[A, a]
 *      If e in FIRST(alpha), then for each terminal b from FOLLOW(A) add
 *      A -> alpha to M[A, b]. If e in FIRST(alpha) and $ in FOLLOW(A) then
 *      add  A -> alpha to M[A, $]
 */
LL1Parser::ParserState LL1Parser::M_() 
{

    switch (curr_symbol_) {

        case P:
        case I_:
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

        case I:
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
            case ID:     return DERIVE;
            case NUM:    return DERIVE;
            case UMINUS: return DERIVE;
            case LB:     return DERIVE;
            default:     return ERROR;
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
            case ID:     return DERIVE;
            case NUM:    return DERIVE;
            case LB:     return DERIVE;
            case UMINUS: return DERIVE;
            default:     return ERROR;
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
            case ID:     return DERIVE;
            case NUM:    return DERIVE;
            case UMINUS: return DERIVE;
            case LB:     return DERIVE;
            default:     return ERROR;
        }
    break;
    }
}

/**
 *   Derives production for the current symbol
 *   and insterts its children to the AST
 */
void LL1Parser::derive() 
{
    switch (curr_symbol_) {

        case P:
        {
            switch (a_) {
                case LET:       
                case ID:    
                case IF:    
                case GOTO:  
                case PRINT:
                case INPUT:
                {
                    // root of AST
                    auto prog = new ASTNode(P);
                    tree_.insert_root(prog);

                    // prog -> instr_
                    auto i_ = new ASTNode(I_);
                    tree_.hang_to_curr_node({i_});

                    stack_.push(I_);
                }
                break;
            }
        }
        break;

        case I_:
        {
            switch (a_) {
                case LET:       
                case ID:    
                case IF:    
                case GOTO:  
                case PRINT: 
                case INPUT:
                {
                    // instr_ -> instr intstr_
                    auto i = new ASTNode(I);
                    auto i_ = new ASTNode(I_);
                    tree_.hang_to_curr_node({i, i_});

                    stack_.push(I_);
                    stack_.push(I);
                }
                break;

                case END:
                    tree_.push_up_curr_node();
                break;
            }
        }
        break;

        case I:
        {
            switch (a_) {
                case LET:
                {
                    auto d = new ASTNode(D);
                    tree_.hang_to_curr_node({d});

                    stack_.push(D);
                }
                break;

                case ID:
                case IF:
                case GOTO:
                case INPUT:
                case PRINT:
                {
                    auto s = new ASTNode(S);
                    tree_.hang_to_curr_node({s});

                    stack_.push(S);
                }
                break;
            }
        }
        break;

        case S:
        {
            switch (a_) {

                // stmt -> las
                case ID:
                {
                    auto las = new ASTNode(LAS);
                    tree_.hang_to_curr_node({las});
                    stack_.push(LAS); 
                }
                break;

                // stmt -> ifstm
                case IF:
                {    
                    auto ifs = new ASTNode(IFS);
                    tree_.hang_to_curr_node({ifs});

                    stack_.push(IFS);
                }
                break;

                // stmt -> gotostmt;
                case GOTO:
                {
                    auto gotos = new ASTNode(GOTOS);
                    tree_.hang_to_curr_node({gotos});

                    stack_.push(GOTOS);
                }
                break;

                // stmt -> inputstmt;
                case INPUT:
                {
                    auto inputs = new ASTNode(INS);
                    tree_.hang_to_curr_node({inputs});

                    stack_.push(INS);
                }
                break;

                // stmt -> printstmt;
                case PRINT:
                {
                    auto prints = new ASTNode(PRS);
                    tree_.hang_to_curr_node({prints});

                    stack_.push(PRS);
                }
                break;
            }
        }
        break;

        case LAS:
        {
            auto id = new Leaf(ID);
            auto la = new ASTNode(LA);
            tree_.hang_to_curr_node({id, la});

            stack_.push(LA);
            stack_.push(ID);
        }
        break;

        case LA:
        {
            switch (a_) {
                case COLON:
                {
                    auto colon = new Leaf(COLON);
                    tree_.hang_to_curr_node({colon});

                    stack_.push(COLON);
                }
                break;

                case ASSIGMENT:
                {
                    auto assgmt = new Leaf(ASSIGMENT);
                    auto e = new ASTNode(E);
                    auto delim = new Leaf(DELIM);
                    tree_.hang_to_curr_node({assgmt, e, delim});

                    stack_.push(DELIM);
                    stack_.push(E);
                    stack_.push(ASSIGMENT);
                }
                break;
            }
        }
        break;

        case IFS:
        {
            auto if_ = new Leaf(IF);
            auto e1 = new ASTNode(E);
            auto relop = new Leaf(RELOP);
            auto e2 = new ASTNode(E);
            auto goto_ = new Leaf(GOTO);
            auto id = new Leaf(ID);
            auto delim = new Leaf(DELIM);
            tree_.hang_to_curr_node({if_, e1, relop, e2, goto_, id, delim});

            stack_.push(DELIM);
            stack_.push(ID);
            stack_.push(GOTO);
            stack_.push(E);
            stack_.push(RELOP);
            stack_.push(E);
            stack_.push(IF);
        }
        break;

        case GOTOS:
        {
            auto goto_leaf = new Leaf(GOTO);
            auto id = new Leaf(ID); 
            auto delim = new Leaf(DELIM);
            tree_.hang_to_curr_node({goto_leaf, id, delim});

            stack_.push(DELIM);
            stack_.push(ID);
            stack_.push(GOTO);
        }
        break;

        case PRS:
        {
            auto print = new Leaf(PRINT);
            auto id = new Leaf(ID);
            auto delim = new Leaf(DELIM);
            tree_.hang_to_curr_node({print, id, delim});

            stack_.push(DELIM);
            stack_.push(ID);
            stack_.push(PRINT);
        }
        break;

        case INS:
        {
            auto input = new Leaf(INPUT);
            auto id = new Leaf(ID);
            auto delim = new Leaf(DELIM);
            tree_.hang_to_curr_node({input, id, delim});

            stack_.push(DELIM);
            stack_.push(ID);
            stack_.push(INPUT);
        }
        break;

        case D:
        {
            auto let = new Leaf(LET);
            auto id = new Leaf(ID);
            auto assgmt = new Leaf(ASSIGMENT);
            auto e = new ASTNode(E);
            auto delim = new Leaf(DELIM);
            tree_.hang_to_curr_node({let, id, assgmt, e, delim});

            stack_.push(DELIM);
            stack_.push(E);
            stack_.push(ASSIGMENT);
            stack_.push(ID);
            stack_.push(LET);

        }
        break;

        case E:
        {
            switch (a_) {

                // expr -> term expr_
                case ID:
                case NUM:
                case UMINUS:
                case LB:
                {
                    auto t = new ASTNode(T);
                    auto e_ = new ASTNode(E_);
                    tree_.hang_to_curr_node({t, e_});

                    stack_.push(E_);
                    stack_.push(T);
                }
                break;
            }
        }
        break;

        case E_:
        {
            switch (a_) {
                case PLUS:
                {
                    auto plus = new Leaf(PLUS);
                    auto t = new ASTNode(T);
                    auto e_ = new ASTNode(E_);

                    tree_.hang_to_curr_node({plus, t, e_});

                    stack_.push(E_);
                    stack_.push(T);
                    stack_.push(PLUS);
                }
                break;

                case MINUS:
                {
                    auto minus = new Leaf(MINUS);
                    auto t = new ASTNode(T);
                    auto e_ = new ASTNode(E_);
                    tree_.hang_to_curr_node({minus, t, e_});

                    stack_.push(E_);
                    stack_.push(T);
                    stack_.push(MINUS);
                }
                break;

                case RB:
                case RELOP:
                case GOTO:
                case DELIM:
                    tree_.push_up_curr_node();
                break;
            }
        }
        break;

        case T:
        {
            switch (a_) {
            
                // term -> factor term_
                case ID:
                case NUM:
                case UMINUS:
                case LB:
                {
                    auto f = new ASTNode(F);
                    auto t_ = new ASTNode(T_);
                    tree_.hang_to_curr_node({f, t_});

                    stack_.push(T_);
                    stack_.push(F);
                }
                break;
            }
        }
        break;

        case T_:
        {
            switch (a_) {
                case MUL:
                {
                    auto mul = new Leaf(MUL);
                    auto f = new ASTNode(F);
                    auto t_ = new ASTNode(T_);
                    tree_.hang_to_curr_node({mul, f, t_});

                    stack_.push(T_);
                    stack_.push(F);
                    stack_.push(MUL);
                }
                break;

                case DIV:
                {
                    auto div = new Leaf(DIV);
                    auto f = new ASTNode(F);
                    auto t_ = new ASTNode(T_);
                    tree_.hang_to_curr_node({div, f, t_});

                    stack_.push(T_);
                    stack_.push(F);
                    stack_.push(DIV);
                }
                break;

                case PLUS:
                case MINUS:
                case RB:
                case RELOP:
                case GOTO:
                case DELIM:
                    tree_.push_up_curr_node();
                break;
            }
        }
        break;

        case F:
        {
            switch (a_) {
            
                // factor -> id | num | (expr) | -factor 
                case ID:
                {
                    auto id = new Leaf(ID);
                    tree_.hang_to_curr_node({id});

                    stack_.push(ID);
                }
                break;

                case NUM:
                {
                    auto num = new Leaf(NUM);
                    tree_.hang_to_curr_node({num});

                    stack_.push(NUM);
                }
                break;

                case LB:
                {
                    auto lb = new Leaf(LB);
                    auto e = new ASTNode(E);
                    auto rb = new Leaf(RB);
                    tree_.hang_to_curr_node({lb, e, rb});

                    stack_.push(RB);
                    stack_.push(E);
                    stack_.push(LB);
                }
                break;

                case UMINUS:
                { 
                    auto uminus = new Leaf(UMINUS);
                    auto f = new ASTNode(F);
                    tree_.hang_to_curr_node({uminus, f});

                    stack_.push(F);
                    stack_.push(UMINUS);
                }
                break;
            }
        }
        break;

    }
}

void LL1Parser::parse() 
{

    while (curr_symbol_ != END) {
        a_ = ip_->term;
        if (curr_symbol_ == a_) {
            tree_.insert_token(ip_);
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
            stack_.pop();
            derive();
        }

        curr_symbol_ = stack_.top();
    }
}