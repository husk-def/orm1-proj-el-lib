
#ifndef PARSE_H
#define PARSE_H

#define BUF_LEN 100

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "header.h"


/* fill_struct function
 * this function goes through biblioteka dir and
 * fetches at most size book headers, putting info
 * into header struct
 *  @return number of fetched headers
 */
int fill_struct(Header* niz_knjiga, int size);

void flush_in();

#endif
