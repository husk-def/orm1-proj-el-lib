
//#include <netinet/in.h>
#include <stdio.h>      //printf
#include <stdlib.h>
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

int main(int argc, char*argv[])
{
    int sock;
    //int len;
    int read_size;
    int n_blocks;
    //instr_t current_instr;
    int current_instr;
    struct sockaddr_in server;
    char message_block[1024];
    char out[200];
    char user[25] = ">\n";
    char bookname[50];
    char path[75];
    FILE *fp;

    /*Loradmi*/
    if(!(argc == 3)) {
        //printf("Too many or too few arguments!");
        puts("client [IP_ADDR] [SOCK]");
        return -1;
    }
    /********/
 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));

    /*Loradmi*/
    printf("Server address: %s\n", inet_ntoa(server.sin_addr));
    printf("Server port: %u\n", ntohs(server.sin_port));
    /********/

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");

    while (1) {
        //printf("%s", user);
        //read_size = recv(sock, message_block, 19, 0);
        //message_block[read_size] = 0;
        puts("******************************************************************");

        printf("%s", user);
        fgets(out, 199, stdin);
        if (out[0] == 'n') break;

        /* send an instruction */
        send(sock, out, strlen(out), 0);
        /* receive first echo - mig */
        memset(message_block, 0, 1024 * sizeof (char));
        read_size = recv(sock, message_block, 1023, 0);
        printf("read_size: %d\n", read_size);
        message_block[read_size] = 0;
        sscanf(message_block, "%d %d", &current_instr, &n_blocks);
        printf("\n%d %d\n", current_instr, n_blocks);
        /* receive second echo - instruction response */
        memset(message_block, 0, 1024 * sizeof (char));
        read_size = recv(sock, message_block, 1023, 0);
        printf("read_size: %d\n", read_size);
        message_block[read_size] = 0;
        printf("srv>\n");
        if (current_instr == LOGIN) {
            sscanf(message_block, "successful login: user -> %s\n", user);
            strcat(user, ">\n");
        } else if (current_instr == LOGOUT) {
            sprintf(user, ">\n");
        } else if (current_instr == DOWNL && n_blocks > 0) {
            sscanf(message_block, "book to be downloaded id: %*d -> %[^\n]s\n", bookname);
            sprintf(path, "biblioteka_clientside/%s", bookname);
            printf("%s\n", message_block);
            fp = fopen(path, "w");
            if (fp == NULL) {
                printf("fopen fault\n");
            } 
            while (n_blocks-- > 0) {
                read_size = recv(sock, message_block, 1023, 0);
                printf("read_size: %d\n", read_size);
                message_block[read_size] = 0;
                if (fp != NULL) {
                    fwrite(message_block, sizeof (char), strlen(message_block), fp);
                } else {
                    printf("%s", message_block);
                }
                //message_block[0] = 0;
                memset(message_block, 0, 1024 * sizeof (char));
            }
            if (fp != NULL) {
                fclose(fp);
            }
        } else if (current_instr == NO_INSTR) {
            printf(ANSI_COLOR_RED"%s\n"ANSI_COLOR_RESET, message_block);
            continue;
        }
        printf(ANSI_COLOR_GREEN"%s\n"ANSI_COLOR_RESET, message_block);
        memset(message_block, 0, 1024 * sizeof (char));
    }
    close(sock);
    puts("socket closed.");
}
