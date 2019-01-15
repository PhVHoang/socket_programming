/*
*   Network programming - 20171
*   Homework 4: Session management
*   Author: Hoang Van Hai
*/

#include "session.h"

enum LogoutStatus logout (struct sockaddr_in *cliaddr, char *id, char *pass) {
    Session *ss = findBySessionCliaddr(cliaddr);
    if (ss == NULL) return -1;
    else if (ss->status == NOT_IDENTIFIED_USER) return LO_NOT_IDENTIFIED_USER;

    User *us = findByUserId(id);
    // id khong ton tai hoac sai nguoi dung
    if (us == NULL || us != ss->user) return LO_WRONG_USER;

    // Dung nguoi dung da dang nhap
    if (ss->status == NOT_AUTHENTICATED     // Da ton tai user nhung chua dang nhap
        || !strcmp(us->pass, pass)) {       // Dung password -> Dang xuat thanh cong
        ss->user = NULL;
        ss->status = NOT_IDENTIFIED_USER;
        ss->no_login_fail = 0;
        
        return LO_SUCCESS;
    } else {    // Sai password
        return LO_WRONG_PASS;
    }
}

enum LoginPassStatus loginPass (struct sockaddr_in *cliaddr, char *pass) {
    Session *ss = findBySessionCliaddr(cliaddr);
    if (ss == NULL) return -1;
    else if (ss->status == NOT_IDENTIFIED_USER) return LP_NOT_IDENTIFIED_USER;
    else if (ss->status == AUTHENTICATED) return LP_EXISTED_ONE_USER;

    // Da dang ki nguoi dung
    if (!strcmp(ss->user->pass, pass)) {    // Login success
        ss->status = AUTHENTICATED;
        ss->no_login_fail = 0;

        return LP_SUCCESS;
    } else {    // Login fail
        ss->no_login_fail++;

        if (ss->no_login_fail >= MAX_NO_LOGIN_FAIL) {    // Login fail too much
            // block user
            ss->user->status = BLOCKED;

            // remove user from session
            ss->user = NULL;
            ss->status = NOT_IDENTIFIED_USER;
            ss->no_login_fail = 0;

            return LP_USER_BLOCKED;
        } else {
            return LP_WRONG_PASS;
        }
    }
}

enum LoginUserStatus loginUser (struct sockaddr_in *cliaddr, char *id) {
    Session *ss = findBySessionCliaddr(cliaddr);
    if (ss == NULL || ss->status == USER_BLOCKED) return -1;
    else if (ss->status == NOT_AUTHENTICATED || ss->status == AUTHENTICATED) return LU_EXISTED_ONE_USER;

    User *us = findByUserId(id);
    if (us == NULL) return LU_NOT_FOUND;
    else if (us->status == BLOCKED) return LU_USER_BLOCKED;
    else {
        ss->user = us;
        ss->status = NOT_AUTHENTICATED;

        return LU_SUCCESS;
    }

}
enum ResigterStatus resigterStatus (struct sockaddr_in *cliaddr,char *id, char *pass)
{
    Session *ss =findBySessionCliaddr(cliaddr) ;
    if (ss == NULL || ss->status ==USER_BLOCKED) return -1;
    User *us = findByUserId(id);
   if(us == NULL ) {
    addUser(id,pass);
   return RE_SUCCESS;} else if return RE_EXIST_USER;
}

enum SessionStatus sessionStatus (struct sockaddr_in *cliaddr) {
    Session *ss = findBySessionCliaddr(cliaddr);
    if (ss == NULL) return -1;

    return ss->status;
}

_Bool removeSession (struct sockaddr_in *cliaddr) {
    int target = findBySessionCliaddr(cliaddr);

    if (target == -1) return FN_ERROR;

    for (int i = target; i < no_session - 1; i++) {
        copySession(session + i, session + i + 1);
    }
    no_session--;

    return FN_SUCCESS;
}

_Bool addSession (User *user, enum SessionStatus status, struct sockaddr_in *cliaddr, int no_login_fail) {
    if (isFullNoSession()) return FN_ERROR;
    
    session[no_session].user = user;
    session[no_session].status = status;
    session[no_session].cliaddr = cliaddr;
    session[no_session].no_login_fail = no_login_fail;

    no_session++;

    return FN_SUCCESS;
}

_Bool newSession (struct sockaddr_in *cliaddr) {
    if (isFullNoSession()) return FN_ERROR;
    
    session[no_session].user = NULL;
    session[no_session].status = NOT_IDENTIFIED_USER;
    session[no_session].cliaddr = cliaddr;
    session[no_session].no_login_fail = 0;
    
    no_session++;
    
    return FN_SUCCESS;
}

Session *findBySessionCliaddr (struct sockaddr_in *cliaddr) {
    for (int i = 0; i < no_session; i++) {
        if (isEqualSockAddrIn(session[i].cliaddr, cliaddr))
            return (session + i);
    }

    return NULL;
}

int indexOfSession (struct sockaddr_in *cliaddr) {
    for (int i = 0; i < no_session; i++) {
        if (isEqualSockAddrIn(session[i].cliaddr, cliaddr))
            return i;
    }

    return -1;
}

void copySession (Session *src, Session *dest) {
    dest->user = src->user;
    dest->status = src->status;
    dest->cliaddr = src->cliaddr;
    dest->no_login_fail = src->no_login_fail;
}

_Bool isFullNoSession () {
    if (no_session > MAX_NO_SESSION) {
        printf("Number of sessions is full!\n");
        return 1;
    }

    return 0;
}

_Bool isEqualSockAddrIn (struct sockaddr_in *addr1, struct sockaddr_in *addr2) {
    return (addr1->sin_addr.s_addr == addr2->sin_addr.s_addr && addr1->sin_port == addr2->sin_port);
}
