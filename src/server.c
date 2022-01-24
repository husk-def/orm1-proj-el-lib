
#include "includes.h"
#include "instruction.h"
#include "user.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

#define DEFAULT_PORT 27015
#define N_USERS 1

Header arr[30];
int arr_size;
User active_users[N_USERS];

void * thread_server(void *arg);

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
    int socket_desc;
    int c;
    int *cl[N_USERS];
    int i;
    int client_sock[N_USERS];
    struct sockaddr_in server;
    struct sockaddr_in client[N_USERS];
    pthread_t client_th[N_USERS];

    arr_size = fill_struct(arr, 30);

    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
	    return 1;
    }
    puts("Socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);
    
    if (bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    listen(socket_desc , N_USERS);
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    for (i = 0; i < N_USERS; ++i) {
        client_sock[i] = accept(socket_desc, (struct sockaddr *)&client[i], (socklen_t *)&c);
        if (client_sock[i] < 0) {
            printf("accept failed: sock %d\n", i);
            return 1;
        }
        printf("connection accepted: sock %d - %s\n", i, inet_ntoa(client[i].sin_addr));

        pthread_create(&client_th[i], NULL, thread_server, &client_sock[i]);
    }

    for (i = 0; i < N_USERS; ++i) {
        pthread_join(client_th[i], NULL);
    }

    return 0;
}

void * thread_server(void *arg)
{
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
    int client_sock;
    int i;
    int n_fetched = 0;
    int n_blocks = 0;
    int user_place;
    int code;
    int filesize = 0;
    int read_size;
    Header criteria;
    Header fetched[30];
    char tmp[100];
    char *message_block;
    char mig[20];
    char *book = (char *)NULL;
    char path[65];
    char input[200];
    struct stat st;
    Instruction current;
    instr_t type;

    init_users(active_users, 5);
    init_user(&this_user);
    init_criteria(&criteria);
    message_block = (char *)calloc(1024, sizeof (char));
    client_sock = *((int *)arg);

    /* fill struct */
    
    while (1) {
        //printf("your instruction: ");
        //send(client_sock, "your instruction: ", strlen("your instruction: "), 0);

        //fgets(input, 199, stdin);
        read_size = recv(client_sock, input, 200, 0);
        input[read_size] = 0;
        puts(input);

        if (read_size == 0) {
            puts("client disconnected");
            return (void *)0;
        } else if (read_size == -1) {
            puts("recv failed");
            continue;
        }

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
                    type = NO_INSTR;
                } else {
                    //TODO: add mutex
                    code = login(&current.inf.usr);
                    if (code < 0) {
                        /* incorrect password */
                        sprintf(message_block, ANSI_COLOR_RED"could not add a user: incorrect password.\n"ANSI_COLOR_RESET);
                        type = NO_INSTR;
                    } else {
                        //TODO: add mutex
                        user_place = add_user(active_users, &current.inf.usr, 5);
                        if (user_place < 0) {
                            sprintf(message_block, ANSI_COLOR_RED"could not add a user: number of users exceeded.\n"ANSI_COLOR_RESET);
                            type = NO_INSTR;
                        } else {
                            this_user = current.inf.usr;
                            utos(&this_user, tmp);
                            sprintf(message_block, ANSI_COLOR_GREEN"succesfully added a user -> %s\n"ANSI_COLOR_RESET, this_user.id);
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
                    //TODO: add mutex
                    remove_user(active_users, user_place);
                    init_user(&this_user);
                    sprintf(message_block, ANSI_COLOR_GREEN"removed a user.\n"ANSI_COLOR_RESET);
                }
                break;
            case CHKST:
                if (is_init(&this_user) == 0) {
                    sprintf(message_block, "you must login first.\n");
                } else {
                    get_ids(&this_user, message_block);
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
                        sprintf(message_block, "book to be downloaded id: %d -> %s\n", fetched[0].id, fetched[0].name);
                        sprintf(path, "./biblioteka/%s", fetched[0].name);
                        stat(path, &st);
                        filesize = st.st_size;
                        n_blocks = ceil(filesize / 1023.);
                        printf("\nsizeof %s is %d\n", path, filesize);
                        printf(ANSI_COLOR_MAGENTA"calloc: %d\n"ANSI_COLOR_RESET, filesize);
                        book = (char *)calloc(filesize, sizeof (char));
                        download_server(book, path, filesize);
                        //TODO: add mutex
                        unique_id(&this_user, fetched[0].id);
                    }
                }
                break;
            case SCHALL:    //FALLTHROUGH
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
        //puts(mig);
        send(client_sock, mig, strlen(mig), 0);
        /* send echo */
        //puts(message_block);
        send(client_sock, message_block, strlen(message_block), 0);
        /* send blocks (if download) */
        for (i = 0; i < n_blocks; ++i) {
            strncpy(message_block, (book + i * 1023 * sizeof (char)), 1023);
            //puts(message_block);
            send(client_sock, message_block, strlen(message_block), 0);
        }
        mig[0] = 0;
        tmp[0] = 0;
        message_block[0] = 0;
        n_blocks = 0;
        if (book != NULL) {
            free(book);
            book = (char *)NULL;
        }
    }
}

