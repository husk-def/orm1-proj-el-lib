#include "download_server.h"
#include <stdlib.h>

void download_server(const Header *a, char *output, char *output_name, const char *path)
{
    FILE* fp;
    int c = 0;

    strcpy(output_name, a->name);

    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("download_server: fopen error\n");
    } else {
        /* sizeof output is equal to sizeof file, no need to control */
        while(1) {
            output[c] = fgetc(fp);
            //strcat(output, &c);
            if(feof(fp)) break;
            ++c;
        }
        fclose(fp);
    }

}