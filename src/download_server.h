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
 *  output - output string to be sent afterwards
 *  path - path of a certain book
 *  size - size of file in bytes
 */
void download_server(char *output, const char *path, int size);

#endif
