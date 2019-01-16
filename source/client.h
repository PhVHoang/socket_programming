#include<dirent.h>
#include<errno.h>
#ifndef CLIENT
#define CLIENT
#include "common.h"
#define PASS_LEN 30
#define CODE_LEN 4

#define USER_FILE "account.txt"

int conn_sock;
int msg_len, bytes_sent, bytes_received;
char buff[BUFF_SIZE], filelink[BUFF_SIZE], filename[BUFF_SIZE], *data;
char *download_filename, *new_download_filename, *wanna_delete_filename, *folder_name, *other_user_name;
struct sockaddr_in server_addr; /* server's address information */
int serv_port;
char serv_ip_child[16];
char *endptr;
FILE *fp = NULL;
int errnum;
double bytes_transfered = 0;

char add_1[2] = "1";
char add_2[2] = "2";
char space[2] = " ";
char* extended_filename;
char output[BUFF_SIZE];
int bytes_output;

int recv_file(int ,char*, char*);
int delete_file(int, char*);
char *recv_msg(int conn_sock);
int send_msg(int conn_sock, char *message, int msg_len);
int send_eof_msg(int conn_sock);
void enter_path_file();
void create_sub_client_folder(char* sub_folder_name);

void cmdLOUT (char *str);
void validArguments (int argc, char *argv[], char *serv_ip, int *serv_port);
_Bool wannaExit (char *buff);
#endif