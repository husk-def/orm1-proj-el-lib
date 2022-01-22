
#include "colors.h"
#include "download_server.h"
#include "header.h"
#include "includes.h"
#include "instruction.h"
#include "search.h"
#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static inline int is_init(const User *u)
{
    if ((u->id[0] == 0) || (u->pass[0] == 0)) {
        /* no */
        return 0;
    } else {
        /* yes */
        return 1;
    }
}

int main()
{
    Header arr[30];
    User active_users[5];
    char input[100];
    Instruction current;
    instr_t type;
    int arr_size;
    

    /* user variables, put them in a function (thread) */
    /* идеја: клијент шаље стринг (инструкцију) и очекује:
     *  - instr_t вредност праћену бројем блокова (блок нам је 1024 бајтова);
     *  - број блокова је пропорционалан величини поруке коју шаљемо са сервера;
     * овај стринг се шаље у формату "%d %d", (int)type, n_blocks и зове се mig;
     * потом сервер шаље ехо у смислу:
     *  - да ли је исправна инструкција (1 блок);
     *  - одзив инструкције (successful login as.../book to download...) (1 блок);
     * серверски ехо се подразумева, број блокова који се прослеђује
     * служи за петљу приликом download-а.
     * сервер након еха шаље наредне поруке, уколико има потребе (download).
     */
    User this_user;
    int i;
    int n_fetched = 0;
    int n_blocks = 0;
    int user_place;
    int code;
    int filesize = 0;
    Header criteria;
    Header fetched[30];
    char tmp[100];
    char *message_block;
    char mig[20];
    char *book = (char *)NULL;
    char path[65];
    struct stat st;

    init_users(active_users, 5);
    init_user(&this_user);
    init_criteria(&criteria);
    message_block = (char *)calloc(1024, sizeof (char));

    /* fill struct */
    arr_size = fill_struct(arr, 30);
    while (1) {
        printf("your instruction: ");
        fgets(input, 99, stdin);
        init_instruction(&current);
        type = parse_instr(input, &current);
        print_instr(&current);

        switch (current.instr) 
        {
            case NO_INSTR:
                sprintf(message_block, ANSI_COLOR_RED"invalid instruction.\n"ANSI_COLOR_RESET);
                break;
            case LOGIN:
                if (is_init(&this_user) == 1) {
                    sprintf(message_block, ANSI_COLOR_RED "you are already logged in -> %s"ANSI_COLOR_RESET, utos(&this_user, tmp));
                    //print_user(&this_user);
                } else {
                    code = login(&current);
                    if (code < 0) {
                        /* incorrect password */
                        sprintf(message_block, ANSI_COLOR_RED"could not add a user: incorrect password.\n"ANSI_COLOR_RESET);
                    } else {
                        user_place = add_user(active_users, &current.inf.usr, 5);
                        if (user_place < 0) {
                            sprintf(message_block, ANSI_COLOR_RED"could not add a user: number of users exceeded.\n"ANSI_COLOR_RESET);
                        } else {
                            this_user = current.inf.usr;
                            utos(&this_user, tmp);
                            sprintf(message_block, ANSI_COLOR_GREEN"succesfully added a user -> %s"ANSI_COLOR_RESET, tmp);
                            print_user(&this_user);
                        }
                    }
                }
                break;
            case LOGOUT:
                if (is_init(&this_user) == 0) {
                    sprintf(message_block, "you must login first.\n");
                    type = NO_INSTR;
                } else {
                    remove_user(active_users, user_place);
                    init_user(&this_user);
                    sprintf(message_block, ANSI_COLOR_GREEN"removed a user.\n"ANSI_COLOR_RESET);
                }
                break;
            case CHKST:
                if (is_init(&this_user) == 0) {
                    sprintf(message_block, "you must login first.\n");
                } else {
                    //TODO: implement this
                }
                break;
            case DOWNL:
                if (is_init(&this_user) == 0) {
                    sprintf(message_block, "you must login first.\n");
                } else {
                    n_fetched = search_h(arr, current.inf.hdr, fetched, arr_size);
                    if (n_fetched < 1) {
                        sprintf(message_block, "book with id:%d not found\n", current.inf.hdr.id);
                    } else {
                        sprintf(message_block, "book to be downloaded id: %d -> %s\n", fetched[0].id, htos(fetched[0], tmp));
                        sprintf(path, "./biblioteka/%s", fetched[0].name);
                        stat(path, &st);
                        filesize = st.st_size;
                        n_blocks = ceil(filesize / 1023.);
                        printf("\nsizeof %s is %d\n", path, filesize);
                        printf(ANSI_COLOR_MAGENTA"calloc: %d\n"ANSI_COLOR_RESET, filesize);
                        book = (char *)calloc(filesize, sizeof (char));
                        download_server(book, path, filesize);
                        // TODO: add unique_add of id into user file
                    }
                }
                break;
            case SCHALL:    
            /* FALLTHROUGH */
            case SEARCH: 
            case SEARCH_I:
            case SEARCH_A:
            case SEARCH_T:
            case SEARCH_Y:
                criteria = current.inf.hdr;
                n_fetched = search_h(arr, criteria, fetched, arr_size);
                sprinth_arr(fetched, n_fetched, message_block);
            default:
                break;
        }

        sprintf(mig, "%d %d", (int)type, n_blocks);
        /* send mig */
        puts(mig);
        /* send echo */
        puts(message_block);
        /* send blocks (if download) */
        for (i = 0; i < n_blocks; ++i) {
            strncpy(message_block, (book + i * 1023 * sizeof (char)), 1023);
            puts(message_block);
        }
        mig[0] = 0;
        tmp[0] = 0;
        message_block[0] = 0;
        n_blocks = 0;
        if (book != NULL) {
            free(book);
            book = (char *)NULL;
        }
        if (type == LOGOUT) 
            break;
    }
}

// int main()
// {
//     int size;
//     int n_matched = 0;
//     Header arr[30];
//     Header tmp[30];
//     Header criteria;
//     char output[100];
//     char path[50] = "./biblioteka/";
//     struct stat st;
//     int filesize;

//     printf("***********fill_struct************\n\n");
//     size = fill_struct(arr, 30);
//     printf("\nfetched %d files.\n\n***********printh_arr*************\n\n", size);
//     printh_arr(arr, size);
//     printf("\n\n");
// ///////////////////////////////////////////////////////////////////
//     /* setup criteria */
//     /* search only by id = 3 */
//     criteria = init_criteria();
//     criteria.id = 3;

//     n_matched = search_h(arr, criteria, tmp, size);

//     printf("\n***********criteria***********\n");
//     printh(&criteria);
//     printf("\n***********matched************ n_matched = %d\n", n_matched);
//     printh_arr(tmp, n_matched);
// ///////////////////////////////////////////////////////////////////
//     /* setup criteria */
//     /* search only by author = "Trifunovic Milorad" */
//     criteria = init_criteria();
//     strcpy(criteria.author, "Trifunovic Milorad");

//     n_matched = search_h(arr, criteria, tmp, size);

//     printf("\n***********criteria***********\n");
//     printh(&criteria);
//     printf("\n***********matched************ n_matched = %d\n", n_matched);
//     printh_arr(tmp, n_matched);
// ///////////////////////////////////////////////////////////////////
//     /* setup criteria */
//     /* search by 
//      * author = "Trifunovic Milorad" 
//      * year = 2022
//      */
//     criteria = init_criteria();
//     strcpy(criteria.author, "Trifunovic Milorad");
//     criteria.year = 2022;

//     n_matched = search_h(arr, criteria, tmp, size);

//     printf("\n***********criteria***********\n");
//     printh(&criteria);
//     printf("\n***********matched************ n_matched = %d\n", n_matched);
//     printh_arr(tmp, n_matched);
// ///////////////////////////////////////////////////////////////////
//     /* setup criteria */
//     /* search all
//      *  
//      * 
//      */
//     criteria = init_criteria();

//     n_matched = search_h(arr, criteria, tmp, size);

//     printf("\n***********criteria***********\n");
//     printh(&criteria);
//     printf("\n***********matched************ n_matched = %d\n", n_matched);
//     printh_arr(tmp, n_matched);

// ////////////////////////////////////////////////////////////////////
//     /*test download*/
//     printh(arr);
//     criteria = init_criteria();
//     criteria.id = 2;
//     /*test_name kao prva poruka da klijent zna kako da imenuje novi fajl, ostatak sadrzaja je u test_string*/
//     char *test_string;
//     char test_name[50]; 

//     n_matched = search_h(arr, criteria, tmp, size);
//     if (n_matched < 1) {
//         strcpy(output, "download_server: The book with the given id doesn't exist\n");
//     } else {
//         /* fetch only one book */
//         strcat(path, tmp[0].name);
//         stat(path, &st);
//         filesize = st.st_size;
//         printf("\nsizeof %s is %d\n", path, filesize);
//     }
//     test_string = (char *)calloc(filesize, sizeof(char));
//     if (test_string == NULL) {
//         printf("calloc unsuccessful\n");
//     }

//     download_server(&tmp[0], test_string, test_name, path);
    
//     printf("***************************************\n\nfilename:%s\ncontent->\n%s\n****************************",test_name, test_string);

//     ////////////////////algoritam za podelu i slanje poruke/////////////////////
    
//     int k = 0;
//     char recvString[200];
//     char sendBuffer[TCP_SUBSTRING_LEN];
//     int num_of_sends = ceil(filesize / (float)(TCP_SUBSTRING_LEN - 1));
//     printf("\nnumber of substrings = %d\n\n", num_of_sends);

//     //strcat(test_string, "&"); //char koji oznacava kraj falja
//     memset(sendBuffer, 0, TCP_SUBSTRING_LEN * sizeof(char));
//     for (int i = 0; i < num_of_sends; i++) {
//         for (int j = 0; j < TCP_SUBSTRING_LEN - 1; j++) {
//             if (test_string[k] == 0) {
//                 sendBuffer[j] = test_string[k];
//                 break;
//             }          
//             sendBuffer[j] = test_string[k];
//             k++;
//         }
//         if (test_string[k] != 0) {
//             sendBuffer[TCP_SUBSTRING_LEN - 1] = 0;
//         }
//         /*umesto strcat() treba da ide sendv()*/
//         strcat(recvString, sendBuffer);
//         memset(sendBuffer, 0, TCP_SUBSTRING_LEN * sizeof(char));
//     }
//     printf("\n%s\n", recvString);
    
//     /* free calloc-ed buffer */
//     free(test_string);

//     /*!!!! sendv(test_name) <------ potrebno je na pocetku ili kraju poslati i poruku koja sadrzi ime fajla*/

//     //////////////////////test download_client//////////////////////////////
//     // printf("\n\n***************test download_client**************\n");
//     // char client_path[50] = "./biblioteka_client/";
//     // download_client(recvString, test_name, client_path);

// ////////////////////////////////////////////////////////////////////////////////////
//     Instruction i;
//     i.inf.hdr = init_criteria();
//     i.instr = LOGIN;
//     strcpy(i.inf.usr.id, "baja");
//     strcpy(i.inf.usr.pass, "bajinasifra");
//     print_instr(&i);

//     int code;
//     printf("********************test with existing user, correct password*********************\n");
//     code = login(&i);
//     printf("error code: %d\n", code);
//     printf("********************test with existing user, incorrect password*********************\n");
//     strcpy(i.inf.usr.pass, "nebajinasifra");
//     code = login(&i);
//     printf("error code: %d\n", code);
//     printf("********************test with non-existing user, should create a new file*********************\n");
//     strcpy(i.inf.usr.id, "novikorisnik");
//     code = login(&i);
//     printf("error code: %d\n", code);
// ///////////////////////////////////////////////////////////////////////////////////
//     printf("******************regex instruction check**********************************\n");
//     /* regex test */
//     Instruction instrukcija;
//     instr_t retval;

//     instrukcija.inf.hdr = init_criteria();
//     retval = parse_instr("search i:24", &instrukcija);
//     print_instr(&instrukcija);

//     instrukcija.inf.hdr = init_criteria();
//     retval = parse_instr("search a:mikrolad prezime", &instrukcija);
//     print_instr(&instrukcija);

//     instrukcija.inf.hdr = init_criteria();
//     retval = parse_instr("search f:24", &instrukcija);
//     print_instr(&instrukcija);

//     instrukcija.inf.hdr = init_criteria();
//     retval = parse_instr("login baja bajinasifra", &instrukcija);
//     print_instr(&instrukcija);

//     instrukcija.inf.hdr = init_criteria();
//     retval = parse_instr("logout", &instrukcija);
//     print_instr(&instrukcija);

//     instrukcija.inf.hdr = init_criteria();
//     retval = parse_instr("schall", &instrukcija);
//     print_instr(&instrukcija);

//     instrukcija.inf.hdr = init_criteria();
//     retval = parse_instr("chkst", &instrukcija);
//     print_instr(&instrukcija);

//     instrukcija.inf.hdr = init_criteria();
//     retval = parse_instr("search i:23 a:mikrolad fifi t:shortbook y:2019", &instrukcija);
//     print_instr(&instrukcija);

//     instrukcija.inf.hdr = init_criteria();
//     retval = parse_instr("search y:2023", &instrukcija);
//     print_instr(&instrukcija);

//     instrukcija.inf.hdr = init_criteria();
//     retval = parse_instr("search t:kratkoime", &instrukcija);
//     print_instr(&instrukcija);

//     instrukcija.inf.hdr = init_criteria();
//     retval = parse_instr("downl 24", &instrukcija);
//     print_instr(&instrukcija);
// }
