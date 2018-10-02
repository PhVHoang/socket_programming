#ifndef SESSION_H
#define SESSION_H

#include <netinet/in.h>
#include "user.h"

#define MAX_NO_SESSION 100
#define MAX_NO_LOGIN_FAIL 5

enum SessionStatus {
    NOT_LOGIN ,
    AUTHENTICATED       // Logined 
   
               
};

typedef struct Session_ {
    User *user;
    enum SessionStatus status;
    struct sockaddr_in *cliaddr;
 
    
} Session;

Session session[MAX_NO_SESSION];
extern int no_session;

enum FunctionStatus {
    FN_SUCCESS,
    FN_ERROR
};

enum LoginStatus {
	LG_ED,             // Logged in account 
	LG_INCORRECT_USER,    // Username incorrect
	LG_INCORRECT_PASS,    // Password incorrect
   	LG_SUCCESS        // Login success
};
enum LogoutStatus {
	LO_NOT_LOGIN,      //
	LO_INCORRECT_PASS,    // Password incorrect
	LO_SUCCESS         // Logout success
};
enum ResigterStatus {
	RE_LOGIN_USER,             //Logged in account 
	DUPLICATE_USER,    // Username 
	RE_SUCCESS        //Resigter success
};
enum LoginStatus login(struct sockaddr_in *cliaddr,char *id,char *pass);
enum LogoutStatus logout(struct sockaddr_in *cliaddr,char *pass);
enum ResigterStatus resigter(struct sockaddr_in *cliaddr ,char *id, char *pass);
enum SessionStatus sessionStatus (struct sockaddr_in *cliaddr);
_Bool removeSession(struct sockaddr_in *cliaddr);
//_Bool addSession (Session *ss, User *us);
Session *findSession(struct sockaddr_in *cliaddr);
//int indexofSession(struct sockaddr_in  *cliaddr);
void copySession (Session *src, Session *dest);
_Bool isEqualSockAddrIn (struct sockaddr_in *addr1, struct sockaddr_in *addr2);
_Bool newSession (struct sockaddr_in *cliaddr) ;
_Bool removeUser (Session *ss);
#endif