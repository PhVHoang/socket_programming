#ifndef USER_H
#define USER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_USER 100
#define USER_LEN 30
#define PASS_LEN 30

typedef struct User_ {
char id[USER_LEN+1];
char pass[PASS_LEN +1];


} User;

User user[MAX_USER];
extern int no_user;

User *findUser(char *id);
void writeUser(char *file_name,User *us);
void readUser(char *file_name);
FILE *openFile (char *file_name, char *mode);
User *addUser(char *id, char *pass);

#endif 

