#include "common.h"

int recv_file(int sock, char* file_name, char* newfilename) {
	char send_str [MAX_SEND_BUF]; /* message to be sent to server*/
	int f; /* file handle for receiving file*/
	ssize_t sent_bytes, rcvd_bytes, rcvd_file_size;
	int recv_count; /* count of recv() calls*/
	char recv_str[MAX_RECV_BUF]; /* buffer to hold received data */
	size_t send_strlen; /* length of transmitted string */
	sprintf(send_str, "%s\n", file_name); /* add CR/LF (new line) */
	send_strlen = strlen(send_str); /* length of message to be transmitted */
	char add_1[2] = "2";
	char* extended_filename = malloc(sizeof(char)*BUFF_SIZE);
	extended_filename = strcat(add_1, file_name);
	printf("Extended filename: %s\n", extended_filename);
	if( (sent_bytes = send(sock, extended_filename, send_strlen+1, 0)) < 0 ) {
		perror("send error");
		return -1;
	} else {
		printf("Signal has been sent already. \n");
	}
	/* attempt to create file to save received data. 0644 = rw-r--r-- */
	printf("Downloading file : %s\n", file_name);
	if ( (f = open(newfilename, O_WRONLY|O_CREAT, 0644)) < 0 ) {
		perror("error creating file");
		return -1;
	}
	recv_count = 0; /* number of recv() calls required to receive the file */
	rcvd_file_size = 0; /* size of received file */
	/* continue receiving until ? (data or close) */
	printf("Processing.....\n");
	while ( (rcvd_bytes = recv(sock, recv_str, MAX_RECV_BUF, 0)) > 0 ) {
		printf("In queue....\n");
		recv_count++;
		rcvd_file_size += rcvd_bytes;
		if (write(f, recv_str, rcvd_bytes) < 0 ) {
			perror("error writing to file");
			return -1;
		}
	}
	close(f); /* close file*/
	printf("Client Received: %d bytes in %d recv(s)\n", rcvd_file_size, recv_count);
	printf("Done\n");
	return rcvd_file_size;
}


void main_client_download(int sock_fd, char* filename, char* newfilename) {
    
	recv_file(sock_fd, filename, newfilename);
	/* close socket*/
	
}