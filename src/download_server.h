#ifndef DOWNLOAD_SERVER_H
#define DOWNLOAD_SERVER_H

#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "header.h"
#include "search.h"

/* download a book from server function
 * parameters:
 *  a - pointer to header of a book to be downloaded
 *  output - output string to be sent afterwards
 *  output_name - name of a book
 *  path - path of a certain book
 * 
 */
void download_server(const Header *a, char *output, char *output_name, const char *path);

#endif
