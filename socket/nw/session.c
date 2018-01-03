#include "session.h"
#define USER_FILE "account.txt"
enum LoginStatus login(struct sockaddr_in *cliaddr,char *id,char *pass) {
	Session *ss = findSession(cliaddr);
	if(ss==NULL) return -1;
	 else {
	 	if(ss->status==AUTHENTICATED ) {
		return LG_ED;

	}
	if (ss->status ==NOT_LOGIN) {
		User *us= findUser(id);
		if(us==NULL) return LG_INCORRECT_USER;
		else {
		if(!strcmp(us->pass,pass)) return LG_INCORRECT_PASS;
		addSession(ss,us);
	}
}
}
}
enum LogoutStatus logout(struct sockaddr_in *cliaddr,char *pass)
{
	Session *ss =findSession(cliaddr);
	if(ss==NULL) return -1;
else {	
	switch(ss->status)
	{
		case NOT_LOGIN :
		return LO_NOT_LOGIN;
		break;
		case AUTHENTICATED :
		if(!strcmp(ss->user->pass,pass)) {
			removeUser(ss);
			return LO_SUCCESS;
			break;
		}
		else {
			return LO_INCORRECT_PASS;
		    break;}

		default : return -1;
		break;

	}
	}
}

enum ResigterStatus resigter(struct sockaddr_in *cliaddr ,char *id, char *pass)
{
	Session *ss =findSession(cliaddr);
	if(ss==NULL) return -1;
	else {
		
			if(ss->status== AUTHENTICATED )
			return RE_LOGIN_USER;
			
			if(ss->status== NOT_LOGIN ) {
			User *us = findUser(id);
			if(us!=NULL) return DUPLICATE_USER;
			else { 

				User *us2 =addUser(id,pass);
				writeUser(USER_FILE,us2);
				return RE_SUCCESS;
			}
		}
	}
}

			


		
enum SessionStatus sessionStatus (struct sockaddr_in *cliaddr) {
    Session *ss = findSession(cliaddr);
    if (ss == NULL) return -1;

    return ss->status;
}

_Bool removeSession (struct sockaddr_in *cliaddr) {
    int target = findSession(cliaddr);

    if (target == -1) return FN_ERROR;

    for (int i = target; i < no_session - 1; i++) {
        copySession(session + i, session + i + 1);
    }
    no_session--;

    return FN_SUCCESS;
}

_Bool addSession (Session *ss, User *us) {
  
  strcpy(ss->user->id,us->id);
  strcpy(ss->user->pass,us->pass);
  ss->status = AUTHENTICATED;

}
_Bool removeUser (Session *ss)
{
	ss->user=NULL;
	ss->status = NOT_LOGIN;
}


Session *findSession(struct sockaddr_in *cliaddr)
{
	for (int i = 0; i < no_session; i++) {
        if (isEqualSockAddrIn(session[i].cliaddr, cliaddr))
            return (session + i);
    }

    return NULL;
}
void copySession (Session *src, Session *dest) {
    dest->user = src->user;
    dest->status = src->status;
    dest->cliaddr = src->cliaddr;

}
_Bool isEqualSockAddrIn (struct sockaddr_in *addr1, struct sockaddr_in *addr2) {
    return (addr1->sin_addr.s_addr == addr2->sin_addr.s_addr && addr1->sin_port == addr2->sin_port);
}
_Bool newSession (struct sockaddr_in *cliaddr) {
    
    session[no_session].user = NULL;
    session[no_session].status = NOT_LOGIN;
    session[no_session].cliaddr = cliaddr;
   
    no_session++;
    
    return FN_SUCCESS;
}
