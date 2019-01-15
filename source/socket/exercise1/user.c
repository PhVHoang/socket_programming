/*
*   Network programming - 20171
*   Homework 4: Session management
*   Author: Hoang Van Hai
*/
#define USER_FILE "account.txt"
#include "user.h"

User *findByUserId (char *id) {
    for (int i = 0; i < no_user; i++) {
        if (!strcmp(user[i].id, id))
            return (user + i);
    }

    return NULL;
}

int indexOfUser (char *id) {
    for (int i = 0; i < no_user; i++) {
        if (!strcmp(user[i].id, id))
            return i;
    }

    return -1;
}

void writeUser (char *file_name,User *us ) {
    FILE *f = openFile(file_name, "a");


        fprintf(f, "%s %s %d\n", us->id, us->pass,us->status);
    

    fclose(f);
}

void readUser (char *file_name) {
    FILE *f = openFile(file_name, "r");

    while (!isFullNoUser()) {
        if (feof(f)) break; // Ket thuc file

        fscanf(f, "%s %s %d", user[no_user].id, user[no_user].pass, &(user[no_user].status));

        // Bo qua dong trong cuoi cung neu co
        if (user[no_user].id[0] == '\0' || user[no_user].pass[0] == '\0') break;
        
        no_user++;
    }

    fclose(f);
}

_Bool isFullNoUser () {
    if (no_user > MAX_NO_USER) {
        printf("Number of users is full!\n");
        return 1;
    }

    return 0;
}

FILE *openFile (char *file_name, char *mode) {
    FILE *f = fopen(file_name, mode);

    if (f == NULL) {
        printf("ERROR: Can't open file %s!\n", file_name);
        exit(EXIT_FAILURE);
    }

    return f;
}



User* addUser (char *id, char *pass) {
    
    User *us = (User *) malloc(sizeof(User));
    strcpy(us->id ,id);
    strcpy(us->pass,pass);
   
    return us;
}
 