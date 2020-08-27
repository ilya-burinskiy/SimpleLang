#include <iostream>
#include "compiler.hpp"

using namespace std;

int main(int argc, char** argv)
{
    Compiler* scc = Compiler::get_instance(argv[1]);
    scc->compile();
}