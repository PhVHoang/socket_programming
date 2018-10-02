#ifndef SERVER
#define SERVER
#include "common.h"
#include "user.h"
#include "session.h"
#include "app.h"

int serv_port;
char serv_ip_child[16];
int no_user = 0;
int no_session = 0;


int listen_sock, conn_sock, msg_len, errnum, sin_size;
struct sockaddr_in server_addr, *client_addr;
socklen_t cli_len;
pid_t child_pid; /* pid of child process */
char file_name [MAX_RECV_BUF]; /* name of the file to be sent */
char print_addr [INET_ADDRSTRLEN]; /* readable IP address */
char *endptr, *data;
char error_message[STRING_SIZE];
char recv_data[BUFF_SIZE];
FILE *fp = NULL;
double bytes_tranfered = 0;
pid_t child_pid;
char output[BUFF_SIZE];
int bytes_output,bytes_sent;
void get_file_name(int, char*);
int send_file(int , char*);
char *recv_msg(int conn_sock, int *errnum, int *msg_len);
int send_msg(int conn_sock, char *message);
char *extract_username(const char *const string, const char *const left, const char *const right);
char *username, *saveusername;
char space[2] = " ";
void remove_first_char(char *s);

#endif