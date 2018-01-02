#ifndef CLIENT
#define CLIENT
#include "common.h"

int conn_sock;
char buff[BUFF_SIZE], filelink[BUFF_SIZE], filename[BUFF_SIZE], *data;
char *download_filename, *new_download_filename;
struct sockaddr_in server_addr; /* server's address information */
int serv_port = 5550;
char serv_ip_child[16] = "127.0.0.1";
char *endptr;
FILE *fp = NULL;
int errnum;
double bytes_transfered = 0;

char add_1[2] = "1";
char add_2[2] = "2";
char* extended_filename;

int recv_file(int ,char*, char*);
char *recv_msg(int conn_sock);
int send_msg(int conn_sock, char *message, int msg_len);
int send_eof_msg(int conn_sock);
void enter_path_file();
#endif