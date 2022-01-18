
#include "search.h"
#include "parse.h"
#include "header.h"
#include "download_server.h"
#include "download_client.h"
#include "instruction.h"
#include <sys/stat.h>
#include <math.h>

#define TCP_SUBSTRING_LEN (10)

int main()
{
    int size;
    int n_matched = 0;
    Header arr[30];
    Header tmp[30];
    Header criteria;
    char output[100];
    char path[50] = "./biblioteka/";
    struct stat st;
    int filesize;

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
    char *test_string;
    char test_name[50]; 

    n_matched = search_h(arr, criteria, tmp, size);
    if (n_matched < 1) {
        strcpy(output, "download_server: The book with the given id doesn't exist\n");
    } else {
        /* fetch only one book */
        strcat(path, tmp[0].name);
        stat(path, &st);
        filesize = st.st_size;
        printf("\nsizeof %s is %d\n", path, filesize);
    }

    test_string = (char *)calloc(filesize, sizeof(char));
    if (test_string == NULL) {
        printf("calloc unsuccessful\n");
    }

    download_server(&tmp[0], test_string, test_name, path);
    
    printf("***************************************\n\nfilename:%s\ncontent->\n%s\n****************************",test_name, test_string);

    ////////////////////algoritam za podelu i slanje poruke/////////////////////
    
    int k = 0;
    char recvString[200];
    char sendBuffer[TCP_SUBSTRING_LEN];
    int num_of_sends = ceil(filesize / (float)(TCP_SUBSTRING_LEN - 1));
    printf("\nnumber of substrings = %d\n\n", num_of_sends);

    //strcat(test_string, "&"); //char koji oznacava kraj falja
    memset(sendBuffer, 0, TCP_SUBSTRING_LEN * sizeof(char));
    for (int i = 0; i < num_of_sends; i++) {
        /*BUG!!! +3 iz nekog razloga reseva, popunjava rupu u bufferu jer iz nekog razloga postane velicine 13*/
        for (int j = 0; j < TCP_SUBSTRING_LEN - 1; j++) {
            if (test_string[k] == 0) {
                sendBuffer[j] = test_string[k];
                break;
            }          
            sendBuffer[j] = test_string[k];
            k++;
        }
        if (test_string[k] != 0) {
            sendBuffer[TCP_SUBSTRING_LEN - 1] = 0;
        }
        /*umesto strcat() treba da ide sendv()*/
        strcat(recvString, sendBuffer);
        memset(sendBuffer, 0, TCP_SUBSTRING_LEN * sizeof(char));
    }
    printf("\n%s\n", recvString);
    
    /* free calloc-ed buffer */
    free(test_string);

    /*!!!! sendv(test_name) <------ potrebno je na pocetku ili kraju poslati i poruku koja sadrzi ime fajla*/

    //////////////////////test download_client//////////////////////////////
    // printf("\n\n***************test download_client**************\n");
    // char client_path[50] = "./biblioteka_client/";
    // download_client(recvString, test_name, client_path);
}
