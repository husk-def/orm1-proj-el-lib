
#include "instruction.h"
#include "header.h"
#include <stdio.h>

void print_instr(const Instruction *i)
{
    char str[200];
    char tmp[200];
    if (strncmp(i->instrname, "login", 6) == 0) {
        sprintf(str, "login %s %s", i->id, i->pass);
    } else if (strncmp(i->instrname, "logout", 7) == 0) {
        sprintf(str, "logout");
    } else if (strncmp(i->instrname, "schall", 7) == 0) {
        sprintf(str, "schall");
    } else if (strncmp(i->instrname, "search", 7) == 0) {
        sprintf(str, "search");
        strcat(str, htos(i->hdr, tmp));
    } else if (strncmp(i->instrname, "chkst", 6) == 0) {
        sprintf(str, "chkst");
    } else if (strncmp(i->instrname, "downl", 6) == 0) {
        sprintf(str, "downl");
    } else {
        sprintf(str, "invalid instruction");
    }
    printf("%s", str);
}
