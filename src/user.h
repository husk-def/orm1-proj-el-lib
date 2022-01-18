
#ifndef USER_H
#define USER_H

typedef struct {
    char id[16];
    char pass[16]; 
} User;

/* user struct to string function
 * string will bi returned via str pointer to char array
 */
char *utos(const User *usr, char* str);

#endif //USER_H
