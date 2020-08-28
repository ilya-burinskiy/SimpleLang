#include "compiler.hpp"
#include "ll1parser.hpp"

#include <iostream>
#include <string>

Compiler* Compiler::instance = nullptr;

Compiler* Compiler::get_instance(const char* fname)
{
    if (instance == nullptr)
        instance = new Compiler(fname);
    return instance;
}

Compiler::Compiler(const char* fname)
{
    parser_ = LL1Parser::get_instance(fname);
}


void Compiler::compile()
{
    using std::cout;
    using std::endl;
    using std::string;

    try {
        parser_->parse();
        AST tree = parser_->get_tree();
        ASTNode* start = tree.get_root();
        postorder_traverse(start);
        cout << endl;
    } catch (const string& err) {
        cout << err << endl;
    }

}