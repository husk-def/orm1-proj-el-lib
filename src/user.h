
#ifndef USER_H
#define USER_H

typedef struct {
    char id[16];
    char pass[16]; 
} User;

void init_user(User *u);

void init_users(User *us, int size);

int user_match(const User *u1, const User *u2);

/* user struct to string function
 * string will be returned via str pointer to char array
 */
char *utos(const User *usr, char* str);

void print_user(const User *u);

void print_users(const User *us, int size);

/*
 * 0 - unique
 * 1 - not unique
 */
int unique_user(const User *us, const User *u, int size);

/*
 * -1  - not added
 * any - added on any place
 */
int add_user(User *us, const User *u, int size);

void remove_user(User *us, int code);

int login(const User *u);

/*
 * binds an id to user, if its not already binded
 */
void unique_id(const User *u, int id);

void get_ids(const User *u, char *str);

#endif //USER_H
