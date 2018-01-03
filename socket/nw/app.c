
#define USER_FILE "account.txt"
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
    case LGIN:
        return processLGIN(cliaddr, cmd->id, cmd->pass);
    case LOUT:
        return processLOUT(cliaddr, cmd->id, cmd->pass);
    case RESG:
	    return processRESG(cliaddr,cmd->id,cmd->pass);

    default:
        return NULL;
    }
}
Output *processLGIN(struct sockaddr_in *cliaddr, char *id, char *pass)
{
	Output *op = (Output *) malloc(sizeof(Output));
	enum LoginStatus login_status =login(cliaddr,id,pass);
	switch(login_status)
	{
		case LG_ED: 
		strcpy(op->code, LOGIN_ED);
		strcpy(op->str," You are logined, please logout to login different account ");
		break;
		case LG_INCORRECT_USER :
		strcpy(op->code, LOGIN_USER_INCORRECT);
		strcpy(op->str,"Username incorrect , please try ");
		break;
		case LG_INCORRECT_PASS :
		strcpy(op->code, LOGIN_PASS_INCORRECT);
		strcpy(op->str," Password incorrect, please try ");
		break;
		case LG_SUCCESS:
		strcpy(op->code, LOGIN_SUCCESS);
		strcpy(op->str," You are login successful");
		break;
		default:
		strcpy(op->code, LOGIN_DECLINE);
		strcpy(op->str, " Sorry, some thing is wrong");
		break;

	}
    
return op;	
}
Output *processLOUT (struct sockaddr_in *cliaddr, char *id, char *pass) {
    Output *op = (Output *) malloc(sizeof(Output));
    
    enum LogoutStatus logout_status = logout(cliaddr, pass);

    switch (logout_status) {
    case LO_NOT_LOGIN :
    strcpy(op->code,"LOUT_DECLINE");
    strcpy(op->str, " You are not login ");
    break;
    case LO_INCORRECT_PASS :
    strcpy(op->code,LOUT_DECLINE);
    strcpy(op->str," Password incorrect, please try ");
    break;
    case LO_SUCCESS :
    strcpy(op->code,LOUT_ACCEPT);
    strcpy(op->str,"You are logout successful!");
    break;
    default:
    strcpy(op->code,LOUT_DECLINE);
    strcpy(op->str, " Sorry, some thing is wrong");
	break;

}
    return op;
}

Output *processRESG (struct sockaddr_in *cliaddr, char *id, char *pass)
{
    Output *op = (Output *) malloc(sizeof(Output));
    enum ResigterStatus resgigter_status = resigter(cliaddr, id, pass);
    switch (resgigter_status)
     {
        case RE_SUCCESS:    
        strcpy(op->code,RESG_ACCEPT);
        strcpy(op->str, " Resgiter success, login to use ");
        break;
    
        case DUPLICATE_USER:
        strcpy(op->code,RESG_DECLINE);
        strcpy(op->str, " User is exist, can't resigter");
        break;
    
        case RE_LOGIN_USER:
        strcpy(op->code,RESG_DECLINE);
        strcpy(op->str, " You are logined, please logout to different resigter");
        break;
        default:
         strcpy(op->code,RESG_DECLINE);
         strcpy(op->str,"Can't  resigter, something is wrong ");
         break;         
    }
    return op;
}

 struct Command_ *command (char *input_str) {
    char code[CODE_LEN + 1];
    char argv1[DATA_LEN + 1];
    char argv2[PASS_LEN + 1];

    struct Command_ *cmd = (struct Command_ *) malloc (sizeof(struct Command_));

    sscanf(input_str, "%s %s %s", code, argv1, argv2);

    if (!strcmp(code, "LGIN")) {
        cmd->code = LGIN;
        strncpy(cmd->id, argv1, USER_ID_LEN);
        strncpy(cmd->pass, argv2, PASS_LEN);
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

