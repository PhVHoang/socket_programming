/*
*   Network programming - 20171
*   Homework 4: Session management
*   Author: Hoang Van Hai
*/

#ifndef APP_H
#define APP_H

#include <stdio.h>
#include <string.h>

#include "user.h"
#include "session.h"

#define USER_VALID "00"         // them thanh cong user
#define USER_INVALID "01"       // khong them duoc user
#define USER_BLOCKED "31"       // khoa tai khoan
#define PASS_MATCH "10"         // password phu hop
#define PASS_NOT_MATCH "11"     // password khong phu hop
#define LOUT_ACCEPT "20"        // chap nhan dang xuat
#define LOUT_DECLINE "21"       // tu choi dang xuat
#define RESIGTER_ACCEPT "50"    // dang ky thanh cong 
#define RESIGTER_DECLINE "51"   // dang ky that bai 
#define USER_ID_LEN 30
#define PASS_LEN 30
#define CODE_LEN 4          // do dai lon nhat cua ma lenh
#define DATA_LEN 30         // max (USER_ID_LEN, PASS_LEN)
#define OCODE_LEN 2         // do dai lon nhat cua output code
#define OSTR_LEN 1024       // do dai lon nhat cua output string

enum CommandCode {
    USER,
    PASS,
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
Output *processPASS (struct sockaddr_in *cliaddr, char *pass);
Output *processUSER (struct sockaddr_in *cliaddr, char *id);
Output *processRESIGTER (struct sockaddr_in *cliaddr,char *id, char *pass);
struct Command_ *command (char *input_str);

#endif