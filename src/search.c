
#include "search.h"
#include "header.h"
#include <string.h>

short compare(Header a, Header b) {
    /* id check */
    if (b.id == -1) {
        /* skip */
    } else {
        if (b.id != a.id) return -1;
    }
    /* year check */
    if (b.year == -1) {
        /* skip */
    } else {
        if (b.year != a.year) return -1;
    }
    /* name check */
    if (b.name[0] == '\0') {
        /* skip */
    } else {
        if (strcmp(a.name, b.name) != 0) return -1;
    }
    /* author check */
    if (b.author[0] == '\0') {
        /* skip */
    } else {
        if (strcmp(a.author, b.author) != 0) return -1;
    }
    if (b.trunc_name[0] == '\0') {
        /* skip */
    } else {
        if (strcmp(a.trunc_name, b.trunc_name) != 0) return -1;
    }
    return 1;
}

int search_h(Header *hs, Header criteria, Header *ret, int size)
{
    int i;
    int ret_cnt = 0;
    for (i = 0; i < size; ++i) {
        if (compare(hs[i], criteria) == 1) {
            /* match */
            seth(&(ret[ret_cnt++]), &(hs[i]));
        } else {
            /* skip, not matched */
        }
    }
    return ret_cnt;
}
