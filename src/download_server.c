#include "download_server.h"
#include <stdlib.h>

void download_server(char *output, const char *path, int size)
{
    FILE* fp;
    //int c = 0;

    //strcpy(output_name, a->name);

    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("download_server: fopen error\n");
    } else {
        /* sizeof output is equal to sizeof file, no need to control */
        // while(1) {
        //     if(feof(fp)) break;
        //     output[c] = fgetc(fp);
        //     ++c;
        // }
        fread(output, sizeof (char), size, fp);
        fclose(fp);
    }

}
