#include "sml.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

Simpletron* Simpletron::instance = nullptr;

bool Simpletron::write_word_(ushort addr, const u_char* bytes) 
{
    if (addr + WORD_SIZE < MEM_SIZE) {
        memory_[addr] = bytes[1];
        memory_[addr + 1] = bytes[0];
        return true;
    }
    return false;
}

ushort Simpletron::read_word_(ushort addr)
{
    ushort word = 0xFFFF;
    word = word & (memory_[addr]);
    word = word << 8;
    word |= memory_[addr + 1];

    return word;
}

void Simpletron::read_program(const char* fname) {
    string str;
    int word;
    u_char word_bytes[WORD_SIZE];
    u_char mask = 0xFF; 
    ushort addr = 0;
    ifstream prog_file(fname);

    if (prog_file.is_open()) {
        while (getline(prog_file, str)) {
            word = (ushort) stoi(str, 0, 16);
            word_bytes[1] = (word >> 8) & mask;
            word_bytes[0] = word & mask;
            if (write_word_(addr, word_bytes))
                addr += WORD_SIZE;
            else {
                prog_file.close();
                throw runtime_error("Not enought memory to read the programm");
            }
        }
    } else {
        prog_file.close();
        throw runtime_error("Could not open the file");
    }
    prog_file.close();
}

void Simpletron::fetch() {
    instruction_register_ = read_word_(instruction_counter_);
}


void Simpletron::decode() {
    ushort op_mask = 0xF << 12;
    ushort opd_mask = 0xFFFF >> 4;

    operation_code_ = (instruction_register_ & op_mask) >> 12;
    operand_ = (instruction_register_ & opd_mask);
}

void Simpletron::run() {
    u_char bytes[WORD_SIZE];
    ushort val;
    do {
        fetch();
        decode();
        switch (operation_code_)
        {
        case READ:
            cout << ">>> ";
            cin >> val;
            bytes[1] = (val >> 8) & 0xFF;
            bytes[0] = val & 0xFF;
            write_word_(operand_, bytes); 
            break;

        case WRITE:
            cout << read_word_(operand_) << endl;
            break;

        case LOAD:
            accumulator_ = read_word_(operand_);
            break;

        case STORE:
            bytes[1] = (accumulator_ >> 8) & 0xFF;
            bytes[0] = accumulator_ & 0xFF;
            write_word_(operand_, bytes);
            break;

        case ADD:
            accumulator_ += read_word_(operand_);
            break;

        case SUB:
            accumulator_ -= read_word_(operand_);
            break;

        case DIV:
            accumulator_ /= read_word_(operand_);
            break;

        case MUL:
            accumulator_ *= read_word_(operand_);
            break;

        case BRANCH:
            instruction_counter_ = operand_;
            continue;

        case BRANCHNEG:
            if ((short) accumulator_ < 0)
                instruction_counter_ = operand_;
            continue;

        case BRANCHZERO:
            if (accumulator_ == 0)
                instruction_counter_ = operand_;
            continue;

        default:
            break;
        }

        instruction_counter_ += WORD_SIZE;
    } while (operation_code_ != HALT);
}