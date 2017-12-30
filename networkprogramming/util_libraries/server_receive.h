#ifndef SERVER_RECEIVE
#define SERVER_RECEIVE
#include "common.h"

char *recv_msg_server(int conn_sock, int *errnum, int *msg_len);
int send_msg_server(int conn_sock, char *message);
void main_server_receive(int listen_sock, int conn_sock, struct sockaddr_in server,
                            struct sockaddr_in client);
#endif