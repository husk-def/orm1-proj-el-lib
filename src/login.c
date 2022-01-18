
#include "login.h"
#include <stdio.h>
#include <string.h>


int login(const Instruction *i)
{
    char path[24] = "users/";
    FILE *fp;
    char password[16];

    strcat(path, i->inf.usr.id);
    printf("path: %s|\nsent password: %s|\n", path, i->inf.usr.pass);
    /* a file must exist for this to work */
    fp = fopen(path, "r+");
    if (fp == NULL) {
        printf("user doesnt exist, creating a new one\n");
        /* create a new file */
        fp = fopen(path, "w");
        /* put a password in it */
        fprintf(fp, "%s", i->inf.usr.pass);
    } else {
        /* fetch a password */
        fscanf(fp, "%s", password);
        printf("fetched password:%s|\n", password);
        if (strcmp(i->inf.usr.pass, password) == 0) {
            printf("correct password.\n");
            fclose(fp);
            return 1;
        } else {
            printf("incorrect password.\n");
            fclose(fp);
            return -1;
        }
    }
    fclose(fp);
    return 0;
}
