#include "server_send.h"

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

void main_server_send(int listen_fd, int conn_fd, struct sockaddr_in srv_addr, struct sockaddr_in cli_addr) {
    // int listen_fd, conn_fd;
	// struct sockaddr_in srv_addr, cli_addr;
	socklen_t cli_len;
	pid_t child_pid; /* pid of child process */
	char file_name [MAX_RECV_BUF]; /* name of the file to be sent */
	char print_addr [INET_ADDRSTRLEN]; /* readable IP address */

	for( ; ; ) /* run forever*/ {
		cli_len = sizeof(cli_addr);
		printf ("Waiting for a client to connect...\n\n");
		/* block until some client connects */
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
}