#include "server_receive.h"
#include "server_send.h"
int serv_port = 5550;
char* serv_ip="127.0.0.1";

void sig_chld(int);

int main() {
    int listen_fd, conn_fd, sin_size;
	int err_signal, msg_length;
    struct sockaddr_in srv_addr, cli_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    memset(&cli_addr, 0, sizeof(cli_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(serv_port);
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

	while (1) {
		sin_size = sizeof(struct sockaddr_in);
		if ((conn_fd = accept(listen_fd,( struct sockaddr *)&cli_addr, &sin_size)) == -1) 
			perror("\nError: ");
  		
		printf("You got a connection from %s\n", inet_ntoa(cli_addr.sin_addr) ); /* prints client's IP */
		char* data = malloc(sizeof(char)*BUFF_SIZE);
		data=recv_msg_server(conn_fd, &err_signal, &msg_length);
		printf("Signal from client: %s\n", data);
		//if (!data) printf("Null---------------\n");
		// if (strcmp(data, "1" == 0)) printf("Signal detected : 1\n");
		if (strcmp(data[0],"1")==0) { // core dump here {
			printf("Signal detected :  1\n");
			main_server_receive(listen_fd, conn_fd, srv_addr, cli_addr);

		}
		else if (strcmp(data[0], "2") == 0) {
			printf("Signal detecte : 2\n");
			main_server_send(listen_fd, conn_fd, srv_addr, cli_addr);

		}
		else if (strcmp(data[0], "3" == 0)) {
			printf("Signal detected: 3\n");
			close(conn_fd);
		}
	}
	close(listen_fd);
    return 0;
}

