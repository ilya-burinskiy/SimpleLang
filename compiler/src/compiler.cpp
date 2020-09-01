#include "compiler.hpp"
#include "ll1parser.hpp"

#include <iostream>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;


Compiler* Compiler::instance = nullptr;
Compiler* Compiler::get_instance(const char* fname)
{
    if (instance == nullptr)
        instance = new Compiler(fname);
    return instance;
}


Compiler::Compiler(const char* fname): bias_(0), high_adr_(0xFFF),
                                       sym_table_()
{
    parser_ = LL1Parser::get_instance(fname);
}


void Compiler::compile()
{
    try {
        parser_->parse();
        AST tree = std::move(parser_->get_tree());
        ASTNode* start = tree.get_root();
        apply_semantic_rules(start);
        cout << endl;
    } catch (const string& err) {
        cout << err << endl;
    }

}


void Compiler::apply_semantic_rules(ASTNode* node)
{
    for (auto child: node->children) {
        apply_semantic_rules(child);
    }

    switch (node->symbol)
    {
        case D:
        {
            // variable name is second child of D node
            auto var_node = *(++(node->children.begin()));
            auto var_leaf = static_cast<Leaf*>(var_node);
            auto var = static_cast<Word*>(var_leaf->tok);

            auto var_addr = high_adr_ - bias_;
            auto expr_addr = high_adr_ - bias_ + 2;

            sym_table_.insert({var->lexeme, var_addr});
            cout << "mov " << expr_addr << ", " << var_addr << endl;

            bias_ += 2;
        } 
        break;


        case LA:
        {
            auto las = node->parent;
            auto id = static_cast<Leaf*>(las->children.front());
            auto id_name = static_cast<Word*>(id->tok)->lexeme;

            if (node->children.front()->symbol == COLON) {
                cout << id_name << ':' << endl; 
            } else {
                auto tabel_entry = sym_table_.find(id_name);
                if (tabel_entry == sym_table_.end()) {
                    undefined_symbol(id_name);
                }

                auto var_addr = tabel_entry->second;
                auto expr_addr = high_adr_ - bias_ + 2;
                cout << "mov " << expr_addr << ", " << var_addr << endl;
                
                bias_ += 2;
            }
        }
        break;

        case IFS:
        {
            auto e1_addr = high_adr_ - bias_ + 4;
            auto e2_addr = high_adr_ - bias_ + 2;

            auto ifstmt_begin = node->children.begin();
            for (int i = 0; i < 2; ++i)
                ++ifstmt_begin;
            auto relop_leaf = static_cast<Leaf*>(*ifstmt_begin);
            auto relop = static_cast<Relop*>(relop_leaf->tok);
            
            auto ifstmt_end = node->children.end();
            for (int i = 0; i < 2; ++i)
                --ifstmt_end;
            auto label_leaf = static_cast<Leaf*>(*ifstmt_end);
            auto label = static_cast<Word*>(label_leaf->tok);

            cout << "mov " << e1_addr << ", \%ax" << endl;
            cout << "sub " << e2_addr << endl;

            switch (relop->type) {
                case Relop::EQ:
                    cout << "jz ";
                break;

                case Relop::GE:
                    cout << "jnl ";
                break;

                case Relop::GT:
                    cout << "jnle ";
                break;

                case Relop::LE:
                    cout << "jng ";
                break;

                case Relop::LT:
                    cout << "jnge ";
                break;

                case Relop::NE:
                    cout << "jnz ";
                break;
            }
            cout << label->lexeme << endl;
        }
        break;

        case GOTOS:
        {
            auto label_node = *(node->children.begin());
            auto label_leaf = static_cast<Leaf*>(label_node);
            auto label = static_cast<Word*>(label_leaf->tok);

            cout << "jmp " << label->lexeme << ':' << endl;
        }
        break;

        case PRS:
        {
            auto var_node_iter = ++(node->children.begin());
            auto var_node = static_cast<Leaf*>(*var_node_iter);
            auto var = static_cast<Word*>(var_node->tok);

            auto table_entry = sym_table_.find(var->lexeme);
            if (table_entry == sym_table_.end()) {
                undefined_symbol(var->lexeme);
            }

            cout << "print " << table_entry->second  << endl;
        }
        break;

        case INS:
        {
            auto var_node_iter = ++(node->children.begin());
            auto var_leaf = static_cast<Leaf*>(*var_node_iter);
            auto var = static_cast<Word*>(var_leaf->tok);

            auto table_entry = sym_table_.find(var->lexeme);
            if (table_entry == sym_table_.end()) {
                undefined_symbol(var->lexeme);
            }

            cout << "input " << table_entry->second << endl;
        }
        break;

        case E_:
        {   
            if (node->children.empty())
                return;

            auto opd1_addr = high_adr_ - bias_ + 4;
            auto opd2_addr = high_adr_ - bias_ + 2;

            auto op_node = *(node->children.begin());
            auto op_leaf = static_cast<Leaf*>(op_node);
            auto op = op_leaf->tok;

            cout << "move " << opd1_addr << ", \%ax" << endl;
            switch (op->term)
            {
                case PLUS:
                    cout << "add " << opd2_addr << endl;
                break;

                case MINUS:
                    cout << "sub " << opd2_addr << endl;
                break;
            }
            cout << "move \%ax, " << opd1_addr << endl;
            bias_ -= 2;                 // overwritting opd2
        }
        break;

        case T_:
        {
            if (node->children.empty())
                return;

            auto opd1_addr = high_adr_ - bias_ + 4;
            auto opd2_addr = high_adr_ - bias_ + 2;

            auto op_node = *(node->children.begin());
            auto op_leaf = static_cast<Leaf*>(op_node);
            auto op = op_leaf->tok;

            cout << "move " << opd1_addr << ", \%ax" << endl;
            switch (op->term)
            {
                case MUL:
                    cout << "mul " << opd2_addr << endl;
                break;

                case DIV:
                    cout << "div " << opd2_addr << endl;
                break;
            }
            cout << "move \%ax, " << opd1_addr << endl;
            bias_ -= 2;                 // overwritting opd2
        }
        break;

        case F:
        {
            auto first_child = node->children.front();
            
            switch (first_child->symbol)
            {
                case ID:
                {
                    auto var_leaf = static_cast<Leaf*>(first_child);
                    auto var = static_cast<Word*>(var_leaf->tok);

                    auto table_entry = sym_table_.find(var->lexeme);
                    if (table_entry == sym_table_.end()) {
                        undefined_symbol(var->lexeme);
                    }
                    auto var_addr = table_entry->second;

                    cout << "mov " << var_addr << ", " << high_adr_ - bias_ << endl;
                    bias_ += 2;
                }
                break;

                case NUM:
                {
                    auto num_leaf = static_cast<Leaf*>(first_child);
                    auto num = static_cast<Num*>(num_leaf->tok);

                    auto num_addr = high_adr_ - bias_;
                    cout << "mov $" << num->val << ", " << num_addr << endl;

                    bias_ += 2;
                }
                break;

                case UMINUS:
                {
                    auto opd_addr = high_adr_ - bias_ + 2;
                    cout << "mov " << opd_addr << ", \%ax" << endl;
                    cout << "mul $-1" << endl;
                    cout << "mov \%ax, " << opd_addr << endl;
                }
                break;
            }

        }
        break;
    }
}


void Compiler::undefined_symbol(const string& str)
{
    stringstream ss;
    ss << "Undefined reference to " << str << endl;
    ss << "Compilation terminated";
    throw string(ss.str());
}