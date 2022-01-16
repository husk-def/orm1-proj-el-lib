
#include "parse.h"
#include "header.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int fill_struct(Header* niz_knjiga, int size)
{ 
    /*pointer to a file stream inside a directory*/
    DIR *dir;   
    /*struct that stores the characteristicts of a file*/
    struct dirent *de; 
    FILE* fp;
    int c;
    /*local counters, for books, parsing of header and parsing of id*/
    int i = 0;
    char path[50]= "./biblioteka/";
    char header[BUF_LEN]; 
    char *token;

    /*opendir returns a pointer to the first file inside a directory*/
    dir = opendir("biblioteka");
    if (dir != NULL) { 
        printf("opendir success\n");
    } else {
        printf("opendir error\n");
        return -1;
    }

    /*goes around the directory until it gets a NULL value 
     *signaling it's at the end of directory if no errors have occured
     */
    de = readdir(dir);
    
    while (de != NULL || i > size) {       
        if(*(de->d_name) == '.') {
            /* skip . and .. */ 
            --i;
        } else {
            strncpy(niz_knjiga[i].name, de->d_name, 50);
            printf("-----\nwf: %s|-----\n", niz_knjiga[i].name);
            strncat(path, niz_knjiga[i].name, 50);
            //sprintf(path, "biblioteka/%s", niz_knjiga[i].name);
            printf("-----\nwd: %s|\n-----\n", path);
            fp = fopen(path, "r");
            //fp = fopen(niz_knjiga[i].name, "r");
            /* reset path */
            path[13] = 0;
            if (fp == NULL) {
                printf("fopen error\n");
                //closedir(dir);
                //return i;
            } else {
                fgets(header, BUF_LEN, fp);
                /* format is id|author|trunc_name|year| */
                /* id */
                token = strtok(header, "|");
                niz_knjiga[i].id = atoi(token);
                /* author */
                token = strtok(NULL, "|");
                strncpy(niz_knjiga[i].author, token, 50);
                /* trunc_name */
                token = strtok(NULL, "|");
                strncpy(niz_knjiga[i].trunc_name, token, 50);
                /* year */
                token = strtok(NULL, "|");
                niz_knjiga[i].year = atoi(token);
            }
        }
        de = readdir(dir); 
        ++i;
    }
    closedir(dir);

    return i;
}
