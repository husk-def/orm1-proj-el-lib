
#include "user.h"
#include <stdio.h>

char *utos(const User *usr, char *str)
{
    sprintf(str, "%s|%s|", usr->id, usr->pass);
    return str;
}
