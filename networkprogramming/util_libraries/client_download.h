#ifndef CLIENT_DOWNLOAD
#define CLIENT_DOWNLOAD
int recv_file(int ,char*, char*);
void main_client_download(int sock_fd, char* filename, char* newfilename);
#endif