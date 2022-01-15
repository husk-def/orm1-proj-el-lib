#include "download_server.h"

void download_server(Header* a, Header criteria, char* output, int size)
{
    Header found;
    FILE* fp;
    int c;
    char filename[30];
    char path[50]= "./biblioteka/";
    struct stat st;
    int stringSize;

    int val = search_h(a, criteria, &found, size);
    if(!val) {
        strcpy(output, "The book with the given id doesn't exist");
        return;
    }

    strcpy(filename, found.name);         
    strcat(path, filename);
    stat(path, &st);
    stringSize = st.st_size;
    printf("%i", stringSize);
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("fopen error\n");
    } else {
        while(1) {
            c = fgetc(fp);
            strcat(output, &c);
            if(feof(fp)) break;
        }
        fclose(fp);
    }

}