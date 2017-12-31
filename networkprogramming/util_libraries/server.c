#include "server_receive.h"
#include "server_send.h"

void sig_chld(int);

int main() {
    int listen_fd, conn_fd;
    struct sockaddr_in srv_addr, cli_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    memset(&cli_addr, 0, sizeof(cli_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = serv_port;
    if ( (listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket error");
		exit(EXIT_FAILURE);
	}
	/* bind to created socket */
	if( bind(listen_fd, (struct sockaddr*) &srv_addr, sizeof(srv_addr)) < 0 ){
		perror("bind error");
		exit(EXIT_FAILURE);
	}
	printf("Listening on port number %d ...\n", ntohs(srv_addr.sin_port));
	if( listen(listen_fd, LISTEN_ENQ) < 0 ) {
		perror("listen error");
		exit(EXIT_FAILURE);
	}
    signal(SIGCHLD, sig_chld);

    return 0;
}

void sig_chld(int signo) {
    pid_t pid;
	int stat;
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	/* for debugging only, i/o not recommended here */
	return;
}

void echo(int sockfd) {
	char buff[BUFF_SIZE];
	int bytes_sent, bytes_received;
	
	bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
	if (bytes_received < 0)
		perror("\nError: ");
	else if (bytes_received == 0)
		printf("Connection closed.");
	
	bytes_sent = send(sockfd, buff, bytes_received, 0); /* echo to the client */
	if (bytes_sent < 0)
		perror("\nError: ");
			
	close(sockfd);
}