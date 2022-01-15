
#include "search.h"
#include "parse.h"
#include "header.h"
#include "download_server.h"

int main()
{
    int size;
    int n_matched = 0;
    Header arr[30];
    Header tmp[30];
    Header criteria;

    printf("***********fill_struct************\n\n");
    size = fill_struct(arr, 30);
    printf("\nfetched %d files.\n\n***********printh_arr*************\n\n", size);
    printh_arr(arr, size);
    printf("\n\n");
///////////////////////////////////////////////////////////////////
    /* setup criteria */
    /* search only by id = 3 */
    criteria = init_criteria();
    criteria.id = 3;

    n_matched = search_h(arr, criteria, tmp, size);

    printf("\n***********criteria***********\n");
    printh(&criteria);
    printf("\n***********matched************ n_matched = %d\n", n_matched);
    printh_arr(tmp, n_matched);
///////////////////////////////////////////////////////////////////
    /* setup criteria */
    /* search only by author = "Trifunovic Milorad" */
    criteria = init_criteria();
    strcpy(criteria.author, "Trifunovic Milorad");

    n_matched = search_h(arr, criteria, tmp, size);

    printf("\n***********criteria***********\n");
    printh(&criteria);
    printf("\n***********matched************ n_matched = %d\n", n_matched);
    printh_arr(tmp, n_matched);
///////////////////////////////////////////////////////////////////
    /* setup criteria */
    /* search by 
     * author = "Trifunovic Milorad" 
     * year = 2022
     */
    criteria = init_criteria();
    strcpy(criteria.author, "Trifunovic Milorad");
    criteria.year = 2022;

    n_matched = search_h(arr, criteria, tmp, size);

    printf("\n***********criteria***********\n");
    printh(&criteria);
    printf("\n***********matched************ n_matched = %d\n", n_matched);
    printh_arr(tmp, n_matched);
///////////////////////////////////////////////////////////////////
    /* setup criteria */
    /* search all
     *  
     * 
     */
    criteria = init_criteria();

    n_matched = search_h(arr, criteria, tmp, size);

    printf("\n***********criteria***********\n");
    printh(&criteria);
    printf("\n***********matched************ n_matched = %d\n", n_matched);
    printh_arr(tmp, n_matched);

////////////////////////////////////////////////////////////////////
    /*test download*/
    printh(arr);
    criteria = init_criteria();
    criteria.id = 1;
    char test_string[200];
    download_server(arr, criteria, test_string, size);
    printf("\n\n%s\n", test_string);
    


}
