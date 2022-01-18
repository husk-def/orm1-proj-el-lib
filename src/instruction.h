
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "header.h"

typedef struct {
    char instrname[7];
    char id[6];
    char pass[10];
    Header hdr;
} Instruction;

void print_instr(const Instruction *i);

#endif //INSTRUCTION_H
