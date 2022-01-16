
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
    criteria.id = 2;
    /*test_name kao prva poruka da klijent zna kako da imenuje novi fajl, ostatak sadrzaja je u test_string*/
    char test_string[200];
    char test_name[30]; 
    download_server(arr, criteria, test_string, test_name, size);
    
    printf("\n%s\n%s\n",test_name, test_string);

    ////////////////////algoritam za podelu i slanje poruke/////////////////////
    
    int bufferSize = 10;
    int k = 0;
    char recvString[200];
    char sendBuffer[bufferSize];
    printf("VELICINA BUFFERA : %i !!", strlen(sendBuffer));
    int num_of_sends = strlen(test_string) / bufferSize;
    if( (strlen(test_string) % bufferSize) != 0) num_of_sends++;

    strcat(test_string, "&"); //char koji oznacava kraj falja
    memset(sendBuffer, '\0', bufferSize);
    for(int i = 0; i < num_of_sends; i++) {
        /*BUG!!! +3 iz nekog razloga reseva, popunjava rupu u bufferu jer iz nekog razloga postane velicine 13*/
        for(int j = 0; j < bufferSize+3; j++) {
            if(test_string[k] == '&') break;          
            sendBuffer[j] = test_string[k];
            k++;
        }
        /*umesto strcat() treba da ide sendv()*/
        strcat(recvString, sendBuffer);
        memset(sendBuffer, '\0', bufferSize);
    }
    printf("\n%s\n", recvString);
    
}
