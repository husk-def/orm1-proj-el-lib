
#ifndef SEARCH_H
#define SEARCH_H

#include <string.h>
#include "header.h"


short compare(Header a, Header b);

/* search_h function does a search by parameters
 * hs       - pointer to array of book headers
 * seh      - searching parameters
 * @return  - number of matched elements
 * this function compares elements in hs with data in seh
 * to skip comparison with certain member in seh:
 *   char member[0] must be equal to '\0'
 *   int member must be == -1
 * matched elements will be put in ret array
 * sizeof ret must be at least the sizeof hs
 */
int search_h(Header *hs, Header criteria, Header *ret, int size);



#endif
