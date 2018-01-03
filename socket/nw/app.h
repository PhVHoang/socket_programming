#ifndef APP_H
#define APP_H

#include <stdio.h>
#include <string.h>

#include "user.h"
#include "session.h"
#define LOGIN_ED "00 "
#define LOGIN_USER_INCORRECT "01"
#define LOGIN_PASS_INCORRECT "02"
#define LOGIN_SUCCESS  "10"
#define LOGIN_DECLINE "11"
#define LOUT_ACCEPT  "20"
#define LOUT_DECLINE  "21"
#define RESG_ACCEPT  "30"
#define RESG_DECLINE "31"
#define USER_ID_LEN 30
#define PASS_LEN 30
#define CODE_LEN 4
#define DATA_LEN 30         
#define OCODE_LEN 2         
#define OSTR_LEN 1024 

enum CommandCode {
    LGIN,
    RESG,
    LOUT
};

struct Command_ {
    enum CommandCode code;
    char id[USER_ID_LEN + 1];
    char pass[PASS_LEN + 1];
};

typedef struct Output_ {
    char code[OCODE_LEN + 1];
    char str[OSTR_LEN + 1];
} Output;

int op2str (Output *op, char *str, int str_size);
Output *processCmd (struct sockaddr_in *cliaddr, char *command_str);
Output *processLOUT (struct sockaddr_in *cliaddr, char *id, char *pass);
Output *processLGIN (struct sockaddr_in *cliaddr , char *id ,char *pass);
Output *processRESG (struct sockaddr_in *cliaddr,char *id, char *pass);
struct Command_ *command (char *input_str);


#endif