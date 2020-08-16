#include "ll1parser.hpp"
#include "lexer.hpp"
#include <list>
#include <vector>

using std::vector;

LL1Parser* LL1Parser::instance = nullptr;

LL1Parser* LL1Parser::get_instance() {
    if (instance == nullptr)
        instance = new LL1Parser();
    return instance;
}


LL1Parser::LL1Parser(): ip_{nullptr}
{
    stack_.push(Symbol::END);
    stack_.push(Symbol::P);
    curr_symbol_ = stack_.top();

    lex_ = Lexer::get_instance();
    lex_->load_buf("test.txt");
    ip_ = lex_->get_token();
    a_ = ip_->term;
}

void LL1Parser::error() {

}

bool LL1Parser::isterminal_(Symbol s) {
    return ((int) s >= 0) && ((int) s < 15);
}

LL1Parser::ParserState LL1Parser::M_(Symbol nonterm, Symbol term) {

    switch (nonterm) {

    case P:
        switch (term) {

        case NUM: return DERIVE;
        case END: return DERIVE;
        default:  return ERROR;
        }
    break;

    case LS:
        switch (term) {

        case NUM: return DERIVE;
        case END: return DERIVE;
        default: return ERROR;
        }
    break;

    case L:
        switch (term) {
        
        case NUM: return DERIVE;
        default: return ERROR;
        }
    break;

    case LBODY:
        switch (term) {

        case ID: return DERIVE;
        case LET: return DERIVE;
        case IF: return DERIVE;
        case GOTO: return DERIVE;
        case INPUT: return DERIVE;
        case PRINT: return DERIVE;
        default: return ERROR;
        }
    break;

    case D:
        switch (term) {
        
        case LET: return DERIVE;
        default: return ERROR;
        }
    break;

    case C:
        switch (term) {
        
        case ID: return DERIVE;
        case NUM: return DERIVE;
        case MINUS: return DERIVE;
        case LB: return DERIVE;
        default: return ERROR;
        }
    break;

    case S:
        switch (term) {

        case ID: return DERIVE;
        case IF: return DERIVE;
        case GOTO: return DERIVE;
        case INPUT: return DERIVE;
        case PRINT: return DERIVE;
        default: return ERROR;
        }
    break;

    case E:
        switch (term) {

        case ID: return DERIVE;
        case NUM: return DERIVE;
        case MINUS: return DERIVE;
        case LB: return DERIVE;
        default: return ERROR;
        }
    break;

    case E_:
        switch (term) {

        case PLUS: return DERIVE;
        case MINUS: return DERIVE;
        case RELOP: return DERIVE;
        case RB: return DERIVE; // e
        case GOTO: return DERIVE; // e
        case DELIM: return DERIVE; /// e
        default: return ERROR;
        }
    break;

    case T:
        switch (term) {
        
        case ID: return DERIVE;
        case NUM: return DERIVE;
        case LB: return DERIVE;
        case MINUS: return DERIVE;
        default: return ERROR;
        }
    break;

    case T_:
        switch (term) {
        
        case PLUS: return DERIVE; //  e
        case MINUS: return DERIVE; // e
        case MUL: return DERIVE;
        case DIV: return DERIVE;
        case RELOP: return DERIVE; // e
        case RB: return DERIVE;    // e
        case GOTO: return DERIVE;  // e
        case DELIM: return DERIVE; // e
        default: return ERROR;
        }
    break;

    case F:
        switch (term) {
        
        case ID: return DERIVE;
        case NUM: return DERIVE;
        case MINUS: return DERIVE;
        case LB: return DERIVE;
        default: return ERROR;
        }
    break;

    }
}

void LL1Parser::derive(Symbol nonterm, Symbol term) {
    switch (nonterm) {

    case P:
        switch (term) {
        
        // prog -> lines
        case NUM:
            stack_.push(LS);
        break;

        // prog -> e
        case END:
        // do nothing
        break;
        }
    break;

    case LS:
        switch (term) {
        
        // lines -> line lines
        case NUM:
            stack_.push(LS);
            stack_.push(L);
        break;

        case END:
        // do nothing
        break;
        }
    break;

    case L:
        switch (term) {
            // line -> num linebody
            case NUM:
            stack_.push(DELIM);
            stack_.push(LBODY);
            stack_.push(NUM);
            break;
        }
    break;

    case LBODY:
        switch (term) {
        
        case LET:
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
        switch (term) {
        
        // stmt -> id = expr
        case ID:
            stack_.push(E);
            stack_.push(ASSIGMENT);
            stack_.push(ID);
        break;

        // stmt -> if cmpexpr goto num
        case IF:
            stack_.push(NUM);
            stack_.push(GOTO);
            stack_.push(C);
            stack_.push(IF);
        break;

        // stmt -> goto num
        case GOTO:
            stack_.push(NUM);
            stack_.push(GOTO);
        break;

        // stmt -> input id
        case INPUT:
            stack_.push(ID);
            stack_.push(INPUT);
        break;

        // stmt -> print id
        case PRINT:
            stack_.push(ID);
            stack_.push(PRINT);
        break;
        }
    break;

    case D:
        switch (term) {
        
        case LET:
            stack_.push(E);
            stack_.push(ASSIGMENT);
            stack_.push(ID);
            stack_.push(LET);
        break;

        }
    break;

    case C:
        switch (term) {

        // cmpexpr -> expr relop expr
        case ID:
        case NUM:
        case MINUS:
        case LB:
            stack_.push(E);
            stack_.push(RELOP);
            stack_.push(E);
        break;
        }
    break;

    case E:
        switch (term) {

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
        switch (term) {
        
        // expr_ -> term expr_
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
        // do nothing
        break;
        }
    break;

    case T:
        switch (term) {
        
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
        switch (term) {
        
        // term_ -> factor term_
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
        switch (term) {
        
        // factor -> id | num | (expr) | negfactor
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

void LL1Parser::parse() {
    while (curr_symbol_ != Symbol::END) {
        a_ = ip_->term;

        if (curr_symbol_ == a_) {
            stack_.pop();
            scanned_tokens_.push_back(ip_);
            ip_ = lex_->get_token();
        }
        // error case
        else if (isterminal_(curr_symbol_)) {
            error();
        }

        // error case 
        else if (ERROR == M_(curr_symbol_, a_)) {
            error();
        } 

        else if (SYNCH == M_(curr_symbol_, a_)) {

        }

        else if (DERIVE == M_(curr_symbol_, a_)) {
            stack_.pop();
            derive(curr_symbol_, a_);
        }

        curr_symbol_ = stack_.top();
    }
}