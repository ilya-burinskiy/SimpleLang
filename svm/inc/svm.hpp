#ifndef SML_H
#define SML_H

// input/output
#define READ       0x1
#define WRITE      0x2

#define LOAD       0x3
#define STORE      0x4

// arithmetic operations
#define ADD        0x5
#define SUB        0x6
#define DIV        0x7
#define MUL        0x8

// control operations
#define BRANCH     0x9
#define BRANCHNEG  0xa
#define BRANCHZERO 0xb
#define HALT       0x0

#define MEM_SIZE   4096u
#define WORD_SIZE  2

#include <array>
#include <string>
#include <algorithm>


class Simpletron {

private:
    std::array<u_char, MEM_SIZE> memory_;
    ushort                       accumulator_;
    ushort                       instruction_register_;
    ushort                       instruction_counter_;
    u_char                       operation_code_;
    ushort                       operand_;

    static Simpletron* instance;
    Simpletron(): accumulator_{0}, instruction_counter_{0},
                  instruction_register_{0}, operand_{0}, operation_code_{0}
    {
        std::fill(memory_.begin(), memory_.end(), 0u);
    }

    Simpletron(const Simpletron& );
    Simpletron& operator=(Simpletron& );

    bool write_word_(ushort addr, const u_char* bytes);
    ushort read_word_(ushort addr);
    void fetch();
    void decode();

public:
    static Simpletron* get_instance() {
        if (!instance)
            instance = new Simpletron();
        return instance;
    }

    void read_program(const char* fname);
    void run();
};

#endif