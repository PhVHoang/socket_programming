#ifndef CLIENT_UPLOAD
#define CLIENT_UPLOAD
#include "common.h"
char *recv_msg_client(int conn_sock);
int send_msg_client(int conn_sock, char *message, int msg_len);
int send_eof_msg(int conn_sock);
void main_client_upload(int client_sock, struct sockaddr_in server_addr);
#endif