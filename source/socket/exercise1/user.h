/*
*   Network programming - 20171
*   Homework 4: Session management
*   Author: Hoang Van Hai
*/

#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NO_USER 100
#define USER_ID_LEN 30
#define PASS_LEN 30

enum UserStatus { 
    BLOCKED,
    ACTIVE
};

typedef struct User_ {
    char id[USER_ID_LEN + 1];
    char pass[PASS_LEN + 1];
    enum UserStatus status;
} User;

User user[MAX_NO_USER];
extern int no_user;



User *findByUserId (char *id);
int indexOfUser (char *id);
void writeUser (char *file_name, User *us );
void readUser (char *file_name);
User* addUser(char *id, char *pass);
_Bool isFullNoUser ();
FILE *openFile (char *file_name, char *mode);


#endif
