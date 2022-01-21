
#include "user.h"
#include <string.h>
#include <stdio.h>

void init_user(User *u)
{
    u->id[0] = 0;
    u->pass[0] = 0;
}

void init_users(User *us, int size)
{
    while (--size >= 0) init_user(&us[size]);
}


/*
 * 0 - match
 * 1 - no match 
 */
int user_match(const User *u1, const User *u2)
{
    if (u1->id[0] == 0 || u2->id[0] == 0 || u1->pass[0] == 0 || u2->pass[0] == 0) {
        return 1;
    } else if ((strcmp(u1->id, u2->id) != 0) || (strcmp(u1->pass, u2->pass) != 0)) {
        return 1;
    } else {
        return 0;
    }
}

char *utos(const User *usr, char *str)
{
    sprintf(str, "%s|%s|", usr->id, usr->pass);
    return str;
}

void print_user(const User *u)
{
    printf("user: \"%s\", password: \"%s\"\n", u->id, u->pass);
}

void print_users(const User *us, int size)
{
    int i;
    for (i = 0; i < size; ++i) {
        if ((us[i].id[0] != 0) && (us[i].pass[0] != 0)) {
            print_user(&us[i]);
        }
    }
}

int unique_user(const User *us, const User *u, int size)
{
    while (--size >= 0) {
        if (user_match(&us[size], u) == 0) return 1;
    }
    return 0;
}

int add_user(User *us, const User *u, int size)
{
    int i;
    if (unique_user(us, u, size) == 1) return -1;
    for (i = 0; i < size; ++i) {
        if (us[i].id[0] == 0 && us[i].pass[0] == 0) {
            /* add user on i-th place */
            strcpy(us[i].id, u->id);
            strcpy(us[i].pass, u->pass);
            return i;
        }
    }
    return -1;
}

void remove_user(User *us, int code)
{
    us[code].id[0] = 0;
    us[code].pass[0] = 0;
}
