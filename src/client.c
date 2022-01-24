
#include <netinet/in.h>
#include <stdio.h>      //printf
#include <string.h>     //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr
#include <unistd.h>
#include "colors.h"

#define DEFAULT_PORT 27015

typedef enum 
{
    NO_INSTR = 10,
    LOGIN = 0,
    LOGOUT,
    SCHALL,
    CHKST,
    DOWNL,
    SEARCH,
    SEARCH_I,
    SEARCH_A,
    SEARCH_T,
    SEARCH_Y,
} instr_t;

int main()
{
    int sock;
    //int len;
    //int read_size;
    int n_blocks;
    //instr_t current_instr;
    int current_instr;
    struct sockaddr_in server;
    char message_block[1024];
    char out[200];
    char user[25] = "> ";
    char bookname[50];
    char path[72];
    FILE *fp;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(DEFAULT_PORT);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");

    while (1) {
        printf("%s", user);
        recv(sock, message_block, 1023, 0);
        fgets(out, 199, stdin);
        if (out[0] == 'n') break;

        /* send an instruction */
        send(sock, out, strlen(out), 0);
        /* receive first echo - mig */
        recv(sock, message_block, 10, 0);
        puts(message_block);
        sscanf(message_block, "%d %d", &current_instr, &n_blocks);
        /* receive second echo - instruction response */
        recv(sock, message_block, 1023, 0);
        puts(message_block);
        if (current_instr == LOGIN) {
            sscanf(message_block, ANSI_COLOR_GREEN"succesfully added a user -> %s"ANSI_COLOR_RESET, user);
            strcat(user, "> ");
        } else if (current_instr == LOGOUT) {
            sprintf(user, "> ");
        } else if (current_instr == DOWNL && n_blocks > 0) {
            sscanf(message_block, "book to be downloaded id: %*d -> %s\n", bookname);
            sprintf(path, "biblioteka_clientside/%s", bookname);
            
            fp = fopen(path, "w");

            while (n_blocks-- > 0) {
                recv(sock, message_block, 1024, 0);
                fwrite(message_block, sizeof (char), strlen(message_block), fp);
            }
            fclose(fp);
        }

    }
    close(sock);
    puts("socket closed.");
}