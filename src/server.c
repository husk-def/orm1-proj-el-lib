
#include "includes.h"

#define DEFAULT_PORT 27015
#define N_USERS 2

Header arr[30];
int arr_size;
User active_users[N_USERS];
pthread_mutex_t lock;
int socket_desc;

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

void int_handler()
{
    printf("closing socket...\n");
    close(socket_desc);
    printf("destroying mutex...\n");
    pthread_mutex_destroy(&lock);
    printf("closing threads...\n");
    exit(0);
}

int main()
{
    int c;
    int i;
    int client_sock[N_USERS];
    struct sockaddr_in server;
    struct sockaddr_in client[N_USERS];
    pthread_t client_th[N_USERS];

    signal(SIGINT, int_handler);

    init_users(active_users, N_USERS);

    
    printf("\n");
    system("hostname -I");
    printf("\n");

    if (pthread_mutex_init(&lock, NULL) != 0) {
        perror("Mutex init has failed\n");
        return -1;
    }

    arr_size = fill_struct(arr, 30);

    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) {
        perror("Could not create socket");
	    return 1;
    }
    puts("Socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);
    
    if (bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    listen(socket_desc , N_USERS);
    printf("Server listening on port %u\n", ntohs(server.sin_port));
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

    close(socket_desc);
    pthread_mutex_destroy(&lock);

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
   
    init_user(&this_user);
    init_criteria(&criteria);
    message_block = (char *)calloc(1025, sizeof (char));
    client_sock = *((int *)arg);
    
    while (1) {
        read_size = recv(client_sock, input, 200, 0);
        input[read_size] = 0;
        printf("%s", input);
        if (read_size == 0) {
            puts("client disconnected, log him out");
            if (is_init(&this_user) == 1) {
                pthread_mutex_lock(&lock);
                remove_user(active_users, user_place);
                pthread_mutex_unlock(&lock);
                init_user(&this_user);
            }
            free(message_block);
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
                sprintf(message_block, "invalid instruction.\n");
                break;
            case LOGIN:
                if (is_init(&this_user) == 1) {
                    sprintf(message_block, "you are already logged in -> %s", utos(&this_user, tmp));
                    type = NO_INSTR;
                } else {
                    pthread_mutex_lock(&lock);
                    code = login(&current.inf.usr);
                    pthread_mutex_unlock(&lock);
                    if (code < 0) {
                        /* incorrect password */
                        sprintf(message_block, "could not add a user: incorrect password.\n");
                        type = NO_INSTR;
                    } else {
                        pthread_mutex_lock(&lock);
                        user_place = add_user(active_users, &current.inf.usr, 5);
                        pthread_mutex_unlock(&lock);
                        if (user_place < 0) {
                            sprintf(message_block, "could not add a user: number of users exceeded or already logged in.\n");
                            type = NO_INSTR;
                        } else {
                            this_user = current.inf.usr;
                            utos(&this_user, tmp);
                            sprintf(message_block, "successful login: user -> %s\n", this_user.id);
                        }
                    }
                }
                break;
            case LOGOUT:
                if (is_init(&this_user) == 0) {
                    sprintf(message_block, "you must login first.\n");
                    type = NO_INSTR;
                } else {
                    pthread_mutex_lock(&lock);
                    remove_user(active_users, user_place);
                    pthread_mutex_unlock(&lock);
                    init_user(&this_user);
                    sprintf(message_block, "successful logout.\n");
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
                        
                        pthread_mutex_lock(&lock);
                        unique_id(&this_user, fetched[0].id);
                        pthread_mutex_unlock(&lock);
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
                if (n_fetched < 1) {
                    sprintf(message_block, "0 books matched criteria.\n");
                } else {
                    sprinth_arr(fetched, n_fetched, message_block);
                }
            default:
                break;
        }

        sprintf(mig, "%d %d", (int)type, n_blocks);
        /* send mig */
        usleep(10000);
        send(client_sock, mig, strlen(mig), 0);
        /* send echo */
        usleep(10000);
        send(client_sock, message_block, strlen(message_block), 0);
        /* pauza obavezna */
        usleep(10000);
        memset(message_block, 0, 1025 * sizeof (char));
        /* send blocks (if download) */
        for (i = 0; i < n_blocks; ++i) {
            strncpy(message_block, (book + i * 1023 * sizeof (char)), 1023);
            send(client_sock, message_block, strlen(message_block), 0);
            message_block[1024] = 0;
            usleep(10000);
        }
        //mig[0] = 0;
        memset(mig, 0, 20 * sizeof (char));
        //tmp[0] = 0;
        memset(tmp, 0, 100 * sizeof (char));
        //message_block[0] = 0;
        memset(message_block, 0, 1025 * sizeof (char));
        n_blocks = 0;
        if (book != NULL) {
            free(book);
            book = (char *)NULL;
        }
    }
}

