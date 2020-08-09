#include "svm.hpp"
#include <iostream>

using namespace std;
int main(int argc, char** argv) {
    Simpletron* svm = Simpletron::get_instance();
    try {
        svm->read_program(argv[1]);
    } catch (runtime_error& err) {
        err.what();
    }
    svm->run();
}