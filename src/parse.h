
#ifndef PARSE_H
#define PARSE_H

#define BUF_LEN 100

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "header.h"



int fill_struct(Header* niz_knjiga, int max_knjiga, int size);
void flush_in();

#endif
