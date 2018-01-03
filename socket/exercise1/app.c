/*
*   Network programming - 20171
*   Homework 4: Session management
*   Author: Hoang Van Hai
*/

#include "app.h"

int op2str (Output *op, char *str, int str_size) {
    bzero(str, str_size);

    if (op == NULL) {
        strcpy(str, "41\nError: Command not found!");
    } else {
        strcpy(str, op->code);
        strcat(str, "\n");
        strncat(str, op->str, str_size - strlen(op->code) - 2);
    
        free(op);
    }

    return (strlen(str) + 1);
}

Output *processCmd (struct sockaddr_in *cliaddr, char *command_str) {
    struct Command_ *cmd = command(command_str);
	if (cmd == NULL) return;

	switch (cmd->code) {
    case USER:
        return processUSER(cliaddr, cmd->id);
		break;
    case PASS:
        return processPASS(cliaddr, cmd->pass);
		break;
    case LOUT:
        return processLOUT(cliaddr, cmd->id, cmd->pass);
		break;
    case RESG:
    
        return processRESIGTER(cliaddr,cmd->id,cmd->pass);
		break;

    default:
        return NULL;
		break;
    }
}

Output *processLOUT (struct sockaddr_in *cliaddr, char *id, char *pass) {
    Output *op = (Output *) malloc(sizeof(Output));
    
    enum LogoutStatus logout_status = logout(cliaddr, id, pass);

    switch (logout_status) {
    case LO_SUCCESS: 
        strcpy(op->code, LOUT_ACCEPT);
        strcpy(op->str, "Logout successful! Use 'USER userid' to add an user.");
        break;
    case LO_NOT_IDENTIFIED_USER: 
        strcpy(op->code, LOUT_DECLINE);
        strcpy(op->str, "Logout fail! Don't exist any user in session, use 'USER userid' to add an user.");
        break;
    case LO_WRONG_USER: 
        strcpy(op->code, LOUT_DECLINE);
        strcpy(op->str, "Logout fail! UserID doesn't match, try again.");
        break;
    case LO_WRONG_PASS: 
        strcpy(op->code, LOUT_DECLINE);
        strcpy(op->str, "Logout fail! Wrong password, try again.");
        // Wrong password -> BLOCK???
        break;
    default:
        strcpy(op->code, LOUT_DECLINE);
        strcpy(op->str, "Add user fail! Something is wrong.");
		break;
    }

    return op;
}

Output *processRESIGTER (struct sockaddr_in *cliaddr, char *id, char *pass)
{
    Output *op = (Output *) malloc(sizeof(Output));
    enum ResigterStatus resgigter_status = resigterStatus(cliaddr, id, pass);
    switch (resgigter_status) {
        case RE_SUCCESS:
        strcpy(op->code,RESIGTER_ACCEPT);
        strcpy(op->str, " Resgiter successed, login to use ");
        case RE_EXIST_USER:
        strcpy(op->code,RESIGTER_DECLINE);
        strcpy(op->str, " User is exist, can't resigter");
    }
    return op;
}

Output *processPASS (struct sockaddr_in *cliaddr, char *pass) {
    Output *op = (Output *) malloc(sizeof(Output));

    enum LoginPassStatus login_pass_status = loginPass(cliaddr, pass);

    switch (login_pass_status) {
    case LP_SUCCESS:
        strcpy(op->code, PASS_MATCH);
        strcpy(op->str, "Login successful! Use 'LOUT userid' to logout.");
        break;
    case LP_NOT_IDENTIFIED_USER:
        strcpy(op->code, PASS_NOT_MATCH);
        strcpy(op->str, "Login fail! Don't exist any user in session, use 'USER useid' to add an user.");
        break;
    case LP_WRONG_PASS:
        strcpy(op->code, PASS_NOT_MATCH);
        strcpy(op->str, "Login fail! Password doesn't match, try again.");
        break;
    case LP_EXISTED_ONE_USER:
        strcpy(op->code, PASS_NOT_MATCH);
        strcpy(op->str, "Login fail! Existed an user in session.");
        break;
    case LP_USER_BLOCKED:
        strcpy(op->code, PASS_NOT_MATCH);
        strcpy(op->str, "Login fail too much! User was blocked and removed from session. Use 'USER userid' to add an other user.");
        break;
    default:
        strcpy(op->code, PASS_NOT_MATCH);
        strcpy(op->str, "Add user fail! Something is wrong.");
    }

    return op;
}

Output *processUSER (struct sockaddr_in *cliaddr, char *id) {
    Output *op = (Output *) malloc(sizeof(Output));

    enum LoginUserStatus login_user_status = loginUser(cliaddr, id);
    
	switch (login_user_status) {
    case LU_SUCCESS:
        strcpy(op->code, USER_VALID);
        strcpy(op->str, "Add user successful! Use 'PASS password' to login.");
        break;
    case LU_EXISTED_ONE_USER:
        strcpy(op->code, USER_INVALID);
        strcpy(op->str, "Add user fail! Existed an user in session.");
        break;
    case LU_USER_BLOCKED:
        strcpy(op->code, USER_BLOCKED);
        strcpy(op->str, "Add user fail! This user is blocked.");
        break;
    case LU_NOT_FOUND:
        strcpy(op->code, USER_INVALID);
        strcpy(op->str, "Add user fail! User not found.");
        break;
    default:
        strcpy(op->code, USER_INVALID);
        strcpy(op->str, "Add user fail! Something is wrong.");
    }

    return op;
}

 struct Command_ *command (char *input_str) {
    char code[CODE_LEN + 1];
    char argv1[DATA_LEN + 1];
    char argv2[PASS_LEN + 1];

    struct Command_ *cmd = (struct Command_ *) malloc (sizeof(struct Command_));

    sscanf(input_str, "%s %s %s", code, argv1, argv2);

    if (!strcmp(code, "USER")) {
        cmd->code = USER;
        strncpy(cmd->id, argv1, USER_ID_LEN);

        return cmd;
    } else if (!strcmp(code, "PASS")) {
        cmd->code = PASS;
        strncpy(cmd->pass, argv1, PASS_LEN);

        return cmd;
    } else if (!strcmp(code, "LOUT")) {
        cmd->code = LOUT;
        strncpy(cmd->id, argv1, USER_ID_LEN);
        strncpy(cmd->pass, argv2, PASS_LEN);

        return cmd;
    } else if (!strcmp(code, "RESG")) {
        cmd->code = RESG;
        strncpy(cmd->id,argv1,USER_ID_LEN);
        strncpy(cmd->pass,argv2, PASS_LEN);
        return cmd;

    

    } else {
        return NULL;
    }
}
