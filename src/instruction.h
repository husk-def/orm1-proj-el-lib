
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <regex.h>
#include "header.h"
#include "user.h"
#include "colors.h"

typedef enum 
{
    NO_INSTR = 10,
    LOGIN = 0,
    LOGOUT,
    SCHALL,
    CHKST,
    DOWNL,
    SEARCH,
    SEARCH_I,
    SEARCH_A,
    SEARCH_T,
    SEARCH_Y,
} instr_t;

union Info {
    User usr;
    Header hdr;
};

typedef struct {
    instr_t instr;
    union Info inf;
} Instruction;

void init_instruction(Instruction *i);

void print_instr(const Instruction *i);

/* fetch arguments function
 * this function goes through str string and gets arguments
 * according to type of instruction
 */
void fetch_args(const char *str, Instruction *i);

/* instruction parser function
 * Arguments:
 *  str - the pointer to instruction string
 *  i - the pointer to Instruction struct to be filled with parameters
 *  @return - type of fetched instruction
 * Operation:
 *  this function parses a string using a regex. If regex matches, struct i
 *  will be filled with parameters
 */
instr_t parse_instr(const char *str, Instruction *i);

#endif //INSTRUCTION_H
