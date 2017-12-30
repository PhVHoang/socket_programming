#ifndef CLIENT_DOWNLOAD
#define CLIENT_DOWNLOAD
#include "common.h"
int recv_file(int ,char*, char*);
char* concat(const char *s1, const char *s2);
void main_client_download(int sock_fd, struct sockaddr_in srv_addr, char* filename, char* newfilename);
#endif