#ifndef SERVER_SEND
#define SERVER_SEND
#include "common.h"

void get_file_name(int, char*);
int send_file(int , char*);
void main_server_send(int listen_fd, int conn_fd, struct sockaddr_in srv_addr, struct sockaddr_in cli_addr);

#endif