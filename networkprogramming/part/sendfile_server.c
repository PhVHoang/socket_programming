/* getfile server that can handle multiple clients using fork */
#include <stdio.h> /* printf and standard i/o */
#include <sys/socket.h> /* socket, bind, listen, accept, socklen_t */
#include <arpa/inet.h> /* sockaddr_in, inet_ntop */
#include <string.h> /* strlen */
#include <stdlib.h> /* atoi, EXIT_FAILURE */
#include <fcntl.h> /* open, O_RDONLY */
#include <unistd.h> /* close, read */
#include <signal.h> /* signal */
#include <sys/wait.h> /* waitpid */
#define  SRV_PORT 5103 /* default port number */
#define  LISTEN_ENQ 5 /* for listen backlog */
#define  MAX_RECV_BUF 256
#define  MAX_SEND_BUF 256

void get_file_name(int, char*);
int send_file(int , char*);
void sig_chld(int);

int main(int argc, char* argv[]) {
	int listen_fd, conn_fd;
	struct sockaddr_in srv_addr, cli_addr;
	socklen_t cli_len;
	pid_t child_pid; /* pid of child process */
	char file_name [MAX_RECV_BUF]; /* name of the file to be sent */
	char print_addr [INET_ADDRSTRLEN]; /* readable IP address */
	memset(&srv_addr, 0, sizeof(srv_addr)); /* zero-fill srv_addr structure*/
	memset(&cli_addr, 0, sizeof(cli_addr)); /* zero-fill cli_addr structure*/
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	/* if port number supplied, use it, otherwise use SRV_PORT */
	srv_addr.sin_port = (argc > 1) ? htons(atoi(argv[1])) : htons(SRV_PORT);
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
	/* install signal handler */
	signal (SIGCHLD, sig_chld);
	for( ; ; ) /* run forever*/ {
		cli_len = sizeof(cli_addr);
		printf ("Waiting for a client to connect...\n\n");
		/* block until some client connects */
		/*Interrupt here*/
		if ( (conn_fd = accept(listen_fd, (struct sockaddr*) &cli_addr, &cli_len)) < 0 ) {
			perror("accept error");
			break; /* exit from the for loop */
		}
		/* convert numeric IP to readable format for displaying */
		inet_ntop(AF_INET, &(cli_addr.sin_addr), print_addr, INET_ADDRSTRLEN);
		printf("Client connected from %s:%d\n",
		print_addr, ntohs(cli_addr.sin_port) );
		/* fork a new child process */
		if ( (child_pid = fork()) == 0 ) /* fork returns 0 for child */ {
			close (listen_fd); /* close child's copy of listen_fd */
			/* do your work*/
			get_file_name(conn_fd, file_name);
			send_file(conn_fd, file_name);
			printf("Closing connection\n");
			/* done */
			close(conn_fd); /* close connected socket*/
			exit(0); /* exit child process */
		}
	close(conn_fd); /* close parent's copy of conn_fd */
	} /* end for */
	close(listen_fd); /* close listening socket*/
	return 0;
}

void get_file_name(int sock, char* file_name) {
	char recv_str[MAX_RECV_BUF]; /* to store received string */
	ssize_t rcvd_bytes; /* bytes received from socket */
	/* read name of requested file from socket */
	if ( (rcvd_bytes = recv(sock, recv_str, MAX_RECV_BUF, 0)) < 0) {
		perror("recv error");
		return;
	}
	sscanf (recv_str, "%s\n", file_name); /* discard CR/LF */
}

int send_file(int sock, char *file_name) {
	int sent_count; /* how many sending chunks, for debugging */
	ssize_t read_bytes, /* bytes read from local file */
	sent_bytes, /* bytes sent to connected socket */
	sent_file_size;
	char send_buf[MAX_SEND_BUF]; /* max chunk size for sending file */
	char * errmsg_notfound = "File not found\n";
	int f; /* file handle for reading local file*/
	sent_count = 0;
	sent_file_size = 0;
	/* attempt to open requested file for reading */
	if( (f = open(file_name, O_RDONLY)) < 0) /* can't open requested file */ {
		perror(file_name);
		if( (sent_bytes = send(sock, errmsg_notfound , strlen(errmsg_notfound), 0)) < 0 ) {
			perror("send error");
			return -1;
		}
	}
	else /* open file successful */ {
		printf("Sending file: %s\n", file_name);
		while( (read_bytes = read(f, send_buf, MAX_RECV_BUF)) > 0 ) {
			if( (sent_bytes = send(sock, send_buf, read_bytes, 0)) < read_bytes ) {
				perror("send error");
				return -1;
			}
			sent_count++;
			sent_file_size += sent_bytes;
		}
		close(f);
	} /* end else */
	printf("Done with this client. Sent %d bytes in %d send(s)\n\n", sent_file_size, sent_count);
	return sent_count;
}
/* define signal hanlder */
void sig_chld(int signo) {
	pid_t pid;
	int stat;
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	/* for debugging only, i/o not recommended here */
	return;
}
