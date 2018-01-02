#ifndef SERVER
#define SERVER
#include "common.h"

int serv_port = 5550;
char serv_ip_child[16] = "127.0.0.1";

int listen_sock, conn_sock, msg_len, errnum, sin_size;
struct sockaddr_in server_addr, client_addr;
socklen_t cli_len;
pid_t child_pid; /* pid of child process */
char file_name [MAX_RECV_BUF]; /* name of the file to be sent */
char print_addr [INET_ADDRSTRLEN]; /* readable IP address */
char *endptr, *data;
char error_message[STRING_SIZE];
FILE *fp = NULL;
double bytes_tranfered = 0;
pid_t child_pid;

void get_file_name(int, char*);
int send_file(int , char*);

char *recv_msg(int conn_sock, int *errnum, int *msg_len);
int send_msg(int conn_sock, char *message);

void remove_first_char(char *s);
#endif