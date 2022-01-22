
#include "user.h"
#include "colors.h"
#include <bits/types/FILE.h>
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
    sprintf(str, "user: \"%s\", password: \"%s\"\n", usr->id, usr->pass);
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

int login(const User *u)
{
    char path[24] = "users/";
    FILE *fp;
    char password[16];

    strcat(path, u->id);
    //printf("path: %s|\nsent password: %s|\n", path, i->inf.usr.pass);
    /* a file must exist for this to work */
    fp = fopen(path, "r+");
    if (fp == NULL) {
        //printf("user doesnt exist, creating a new one\n");
        /* create a new file */
        fp = fopen(path, "w");
        /* put a password in it */
        fprintf(fp, "%s\n", u->pass);
    } else {
        /* fetch a password */
        fscanf(fp, "%s", password);
        //printf("fetched password:%s|\n", password);
        if (strcmp(u->pass, password) == 0) {
            //printf("correct password.\n");
            fclose(fp);
            return 1;
        } else {
            //printf("incorrect password.\n");
            fclose(fp);
            return -1;
        }
    }
    fclose(fp);
    return 0;
}

void unique_id(const User *u, int id)
{
    FILE *fp;
    char path[24];
    int tmp;

    sprintf(path, "users/%s", u->id);
    fp = fopen(path, "r+");
    if (fp == NULL) {
        printf(ANSI_COLOR_RED"Unexpected error.\n"ANSI_COLOR_RESET);
        return;
    } else {
        fscanf(fp, "%*s\n");
        while (feof(fp) == 0) {
            fscanf(fp, "%d ", &tmp);
            if (tmp == id) {
                fclose(fp);
                return;
            }
        }
    }
    fp = fopen(path, "a");
    fprintf(fp, "%d ", id);
    fclose(fp);
}

void get_ids(const User *u, char *str)
{
    FILE *fp;
    char path[24];   
    sprintf(path, "users/%s", u->id);
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf(ANSI_COLOR_RED"Unexpected error.\n"ANSI_COLOR_RESET);
        return;
    } else {
        /* skip a password, path is not used anymore */
        fgets(path, 23, fp);
        if (feof(fp) == 0) {
            /* read a line with ids */
            fgets(str, 1023, fp);
        }
    }
    fclose(fp);
}
