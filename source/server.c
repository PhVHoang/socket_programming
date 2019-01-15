#include "server.h"

#define USER_FILE "account.txt"

void get_file_name(int sock, char* file_name) {
	char recv_str[MAX_RECV_BUF]; 
	ssize_t rcvd_bytes; 

	if ( (rcvd_bytes = recv(sock, recv_str, MAX_RECV_BUF, 0)) < 0) {
		perror("recv error");
		return;
	}
	sscanf (recv_str, "%s\n", file_name); /* discard CR/LF */
    
}

int send_file(int sock, char *file_name) {
	int sent_count; 
	ssize_t read_bytes, 
	sent_bytes,
	sent_file_size;
	char send_buf[MAX_SEND_BUF]; 
	char * errmsg_notfound = "File not found\n";
	int f; 
	sent_count = 0;
	sent_file_size = 0;
	if( (f = open(file_name, O_RDONLY)) < 0)  {
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

void delete_file_on_server(int sock, char* filename) {
	int f;
	char * err_signal;
	char * errmsg_notfound = "File not found\n";
	ssize_t sent_bytes;

	if ((f=open(filename, O_RDONLY)) < 0) {
		perror(file_name);
		err_signal = "1";
		send_msg(sock, err_signal);
		// if( (sent_bytes = send(sock, errmsg_notfound , strlen(errmsg_notfound), 0)) < 0 ) {
		// 	perror("send error");
		// 	err_signal = "1";
		// 	send_msg(sock, err_signal);
		// 	return -1;
		// }
	}
	else {
		printf("This file is on the server\n");
		// delete_file(file_name);
		remove(filename);
		err_signal = "0";
		send_msg(sock, err_signal);
		printf("This file was completely deleted\n");
	}
}
char *recv_msg(int conn_sock, int *errnum, int *msg_len){
	int ret, nLeft, index = 0;
	char recv_data[BUFF_SIZE], *data;

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
   for ( i = 0; s[i] != '\0'; i++ ) 
   {
         s[j++] = s[i+1]; 
   }
   s[j] = '\0'; 
}

void set_null(char **ptr) {
    *ptr = NULL;
}

char *extract_username(const char *const string, const char *const left, const char *const right) {
    char  *head;
    char  *tail;
    size_t length;
    char  *result;

    if ((string == NULL) || (left == NULL) || (right == NULL))
        return NULL;
    length = strlen(left);
    head   = strstr(string, left);
    if (head == NULL)
        return NULL;
    head += length;
    tail  = strstr(head, right);
    if (tail == NULL)
        return tail;
    length = tail - head;
    result = malloc(1 + length);
    if (result == NULL)
        return NULL;
    result[length] = '\0';

    memcpy(result, head, length);
    return result;
}


int main(int argc, const char* argv[]) {

	if (argc < 2) {
		printf("Invalid argument!\n");
		return 0;
	}
	serv_port = (in_port_t) strtol(argv[1], &endptr, 10);
	readUser(USER_FILE);
    
    /*Initialize*/
    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  
		perror("\nError: ");
		return 0;
	}
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;         
	server_addr.sin_port = htons(serv_port);   
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);     
	if(bind(listen_sock, (struct sockaddr*)&server_addr, sizeof(server_addr))==-1){ 
		perror("\nError: ");
		return 0;
	}     
	
	if(listen(listen_sock, BACKLOG) == -1){  
		perror("\nError: ");
		return 0;
	}

    
    signal (SIGCHLD, sig_chld);
    while (1) {
		readUser(USER_FILE);
        sin_size = sizeof(struct sockaddr_in);
		client_addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
		if ((conn_sock = accept(listen_sock,( struct sockaddr *)client_addr, &sin_size)) == -1) 
			perror("\nError: ");
  		if (newSession(client_addr) == FN_ERROR) {
			printf("Error: Number of sessions is maximum!");
			continue;
		}
		printf("You got a connection from %s\n", inet_ntoa(client_addr->sin_addr) ); /* prints client's IP */
		child_pid = fork();
		
		if (child_pid == 0) {
			 while (1) {
				 	
                    if((data = recv_msg(conn_sock, &errnum, &msg_len)) ){ 
						printf("data = %s\n", data);
						if (data[0]=='1') {
							printf("Signal from client : %s\n", data);
							remove_first_char(data);
							
							saveusername = (char*)malloc(sizeof(char)*1000);
							bzero(saveusername, 1000);
							strcat(saveusername,username);
							strcat(saveusername, "/");
							data = strcat(saveusername, data);
                    		if((fp = fopen(data, "rb"))){
                           		strcpy(error_message, "1");
                            	send_msg(conn_sock, error_message);
                            	continue;
							} else{// if not
								fp = fopen(data, "wb+");
								strcpy(error_message, "0");
								send_msg(conn_sock, error_message);
								bytes_tranfered = 0;
								
								while(strcmp(error_message, "0") == 0){	
									data = recv_msg(conn_sock, &errnum, &msg_len);
									if(data != NULL && errnum == 0){	
										printf("Received: %.2lf MB\n", (bytes_tranfered += msg_len) / (1024 * 1024));
										fwrite(data, msg_len, 1, fp);
									}else if(errnum == 0){		
										strcpy(error_message, "-1");
										fclose(fp);
									}else{		
										strcpy(error_message, "2");
										fclose(fp);
									}
									send_msg(conn_sock, error_message);
	
								}
							}
							//set_null(&data);	
						}
						else if (data[0]=='2') {
							remove_first_char(data);
							saveusername = (char*)malloc(sizeof(char)*1000);
							bzero(saveusername, 1000);
							strcat(saveusername,username);
							strcat(saveusername, "/");
							data = strcat(saveusername, data);
							
							printf("data = %s\n", data);
							send_file(conn_sock, data);
							send(conn_sock, "", 0, 0);
							
						}
						else if (data[0] == '3') {
							remove_first_char(data);
							saveusername = (char*)malloc(sizeof(char)*1000);
							bzero(saveusername, 1000);
							strcat(saveusername, username);
							strcat(saveusername, "/");
							data = strcat(saveusername, data);
							printf("data = %s\n", data);
							delete_file_on_server(conn_sock, data);
							send(conn_sock, "", 0, 0);
						}
						else {
							// recv_data[msg_len] = '\0';
							data[msg_len] = '\0';
							printf("\nReceive: |%s|\n", data);
							username = (char*)malloc(sizeof(char)*1000);
							
							username = extract_username(data, space, space);
							printf("username = %s\n", username);

							Output *op = processCmd(client_addr, data);
							
							bytes_output = op2str(op, output, BUFF_SIZE);
							bytes_sent = send(conn_sock, output, bytes_output, 0); /* send to the client welcome message */
							if (bytes_sent <= 0){
								printf("\nConnection closed\n");
							break;
							}
						}
                
                }

            }
			exit(0);
		}
		free(username);
        
        close(conn_sock);
		removeSession(client_addr);
    }
    close(listen_sock);
    return 0;
}

