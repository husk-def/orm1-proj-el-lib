#ifndef DOWNLOAD_SERVER_H
#define DOWNLOAD_SERVER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "header.h"
#include "search.h"

void download_server(Header* a, Header criteria, char* output, char* output_name, int size);

#endif