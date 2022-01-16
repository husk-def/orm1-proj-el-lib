
#include "header.h"
#include <string.h>

Header init_criteria()
{
    Header h;
    h.author[0] = 0;
    h.id = -1;
    h.trunc_name[0] = 0;
    h.name[0] = 0;
    h.year = -1;
    return h;
}

char * htos(Header h, char *str)
{
    char buf[100];
    sprintf(buf, "%s|%d|%s|%s|%d", h.name, h.id, h.author, h.trunc_name, h.year);
    strcpy(str, buf);
    return str;
}

void seth(Header *dst, const Header *src)
{
    dst->year = src->year;
    dst->id = src->id;
    strcpy(dst->author, src->author);
    strcpy(dst->trunc_name, src->trunc_name);
    strcpy(dst->name, src->name);
}

void printh(const Header *h)
{
    char buf[100];
    //printf("%s\n", htos(*h, buf));
    htos(*h, buf);
    printf("%s\n", buf);
}

void printh_arr(const Header *harr, int size)
{
    int i;
    for (i = 0; i < size; ++i) {
        printh(&harr[i]);
    }
}
