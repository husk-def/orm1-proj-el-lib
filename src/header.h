
#ifndef HEADER_H
#define HEADER_H

#include <string.h>
#include <stdio.h>

typedef struct { 
    char name[30];
    int id; 
    char author[36]; 
    char trunc_name[16]; 
    int year; 
} Header;

/* init criteria function
 * this function returns a struct with no set criteria
 */
Header init_criteria(Header *h);

/* header to string function
 * parses header h into string pointed by *str
 * ! IMPORTANT ! String size must be large enough
 */
char * htos(Header h, char *str);

/* set header function
 * copies contents of header pointed by src to 
 * header pointed by dst
 */
void seth(Header *dst, const Header *src);

/* print header */
void printh(const Header *h);

/* print array of headers 
 *   size - size of array of headers pointed by *harr 
 */
void printh_arr(const Header *harr, int size);

/* sprint header array into str 
 * str[0] must be 0!!!
 */
void sprinth_arr(const Header *h, int size, char *str);

#endif
