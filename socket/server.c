#include "server.h"

void get_file_name(int sock, char* file_name) {
	char recv_str[MAX_RECV_BUF]; /* to store received string */
	ssize_t rcvd_bytes; /* bytes received from socket */
	/* read name of requested file from socket */
	if ( (rcvd_bytes = recv(sock, recv_str, MAX_RECV_BUF, 0)) < 0) {
		perror("recv error");
		return;
	}
	sscanf (recv_str, "%s\n", file_name); /* discard CR/LF */
    //remove_first_char(file_name);
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
	//Sao cho nay doc co sent maf ko co recv v anh
	// cai nay ben server chi send cai file di thoi ma anh 
	else /* open file successful */ {
		printf("Sending file: %s\n", file_name);
		while( (read_bytes = read(f, send_buf, MAX_RECV_BUF)) > 0 ) {
			printf("Sending\n");
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

char *recv_msg(int conn_sock, int *errnum, int *msg_len){
	int ret, nLeft, index = 0;
	char recv_data[BUFF_SIZE], *data;

	// receive the length of message
	int bytes_received = recv(conn_sock, &(*msg_len), sizeof(int), MSG_WAITALL);
	if (bytes_received <= 0){
		*errnum = -1;
		return NULL;
	}

	if(*msg_len > 0){
		nLeft = *msg_len;
		data = (char*) malloc(*msg_len * sizeof(char));
		memset(data, 0, *msg_len);
		index = 0;
		
		//receives message from client
		while(nLeft > 0){
			ret = recv(conn_sock, recv_data, BUFF_SIZE, 0);
			if(ret == -1){
				*errnum = -1;
				return NULL;
			}
			memcpy(data + index, recv_data, ret);
			index += ret;
			nLeft -= ret;
		}
		data[*msg_len] = '\0';	
		*errnum = 0;
	}else{
		printf("\nSuccessful transfering\n");
		*errnum = 0;
		return NULL;
	}
	
	return data;
}

int send_msg(int conn_sock, char *message){
	int msg_len, bytes_sent;
	//send the length of the message to client
	msg_len = strlen(message);
	bytes_sent = send(conn_sock, &msg_len, sizeof(int), 0);
	if(bytes_sent <= 0){
		return -1;
	}

	// send message to client
	bytes_sent = send(conn_sock, message, msg_len, 0);
	if (bytes_sent <= 0){
		return -1;
	}
	return 0;
}

void remove_first_char(char *s) {
   int i, j = 0;
   for ( i = 0; s[i] != '\0'; i++ ) /* 'i' moves through all of original 's' */
   {
         s[j++] = s[i+1]; /* 'j' only moves after we write a non-'chr' */
   }
   s[j] = '\0'; /* re-null-terminate */
}

void set_null(char **ptr) {
    *ptr = NULL;
}

int main(int argc, const char* argv[]) {
    
    /*Initialize*/
    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;         
	server_addr.sin_port = htons(serv_port);   /* Remember htons() from "Conversions" section? =) */
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */   
	if(bind(listen_sock, (struct sockaddr*)&server_addr, sizeof(server_addr))==-1){ /* calls bind() */
		perror("\nError: ");
		return 0;
	}     
	
	if(listen(listen_sock, BACKLOG) == -1){  /* calls listen() */
		perror("\nError: ");
		return 0;
	}
    
    signal (SIGCHLD, sig_chld);
    while (1) {
        sin_size = sizeof(struct sockaddr_in);
		if ((conn_sock = accept(listen_sock,( struct sockaddr *)&client_addr, &sin_size)) == -1) 
			perror("\nError: ");
  		
		printf("You got a connection from %s\n", inet_ntoa(client_addr.sin_addr) ); /* prints client's IP */
		child_pid = fork();
		if (child_pid == 0) {
			 while (1) {
                    if((data = recv_msg(conn_sock, &errnum, &msg_len)) ){ // no khong nhan dc cai nay
						printf("data = %s\n", data);
						if (data[0]=='1') {
							printf("Signal from client : %s\n", data);
							remove_first_char(data);
                    		if((fp = fopen(data, "rb"))){
                           		strcpy(error_message, "1");
                            	send_msg(conn_sock, error_message);
                            	continue;
							} else{// if not
								fp = fopen(data, "wb+");
								strcpy(error_message, "0");
								send_msg(conn_sock, error_message);
								bytes_tranfered = 0;
								
								while(strcmp(error_message, "0") == 0){		// while there's no error
									data = recv_msg(conn_sock, &errnum, &msg_len);
									if(data != NULL && errnum == 0){		// if file content is received
										printf("Received: %.2lf MB\n", (bytes_tranfered += msg_len) / (1024 * 1024));
										fwrite(data, msg_len, 1, fp);
									}else if(errnum == 0){		// if file reached EOF
										strcpy(error_message, "-1");
										fclose(fp);
									}else{		// if file transfering is interupted
										strcpy(error_message, "2");
										fclose(fp);
									}
									send_msg(conn_sock, error_message);//Anh send phat nay anh nhan lai mess chua?
	
								}
							}
							//set_null(&data);	
						}
						else if (data[0]=='2') {
							remove_first_char(data);
							printf("data = %s\n", data);
							//get_file_name(conn_sock, data);
							send_file(conn_sock, data);
							// em nghi nen send o day phat xem sao, cx dk day nhung no khac gi sent vua nay dau nhi
							send(conn_sock, "", 0, 0);
						}
                
                }
				printf("LOL\n");
				//Doan nay anh nghi no loi o dau chua :v, em chi nghi dk den doan nay thoi

            }
			exit(0);
		}
        
			printf("out\n");
        close(conn_sock);
    }
    close(listen_sock);
    return 0;
}

