#include "download_client.h"

void download_client(char* input, char* input_name, const char* library_path)
{
    FILE* fp;
    char file_path[50];

    strcpy(file_path, library_path);
    strcat(file_path, input_name);

    fp = fopen(file_path, "w");

    fprintf(fp, "%s", input);

    fclose(fp);
}
