
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "header.h"
#include "user.h"

union Info {
    User usr;
    Header hdr;
};

typedef struct {
    char instrname[7];
    union Info inf;
} Instruction;

void print_instr(const Instruction *i);

#endif //INSTRUCTION_H
