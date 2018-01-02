#include "client_upload.h"

char *recv_msg_client(int conn_sock){
	int ret, nLeft, msg_len, index = 0;
	char recv_data[WINDOW_SIZE], *data;
	// receive the length of message
	int bytes_received = recv(conn_sock, &msg_len, sizeof(int), MSG_WAITALL);
	if (bytes_received <= 0){
		return NULL;
	}
	nLeft = msg_len;
	data = (char*) malloc(msg_len);
	memset(data, 0, msg_len);
	index = 0;
	
	//receives message from server
	while(nLeft > 0){
		ret = recv(conn_sock, recv_data, WINDOW_SIZE, 0);
		if(ret == -1){
			break;
		}
		memcpy(data + index, recv_data, ret);
		index += ret;
		nLeft -= ret;
	}
	data[msg_len] = '\0';
	return data;
}

int send_msg_client(int conn_sock, char *message, int msg_len){
	int bytes_sent;
	//send the length of the message to server
	bytes_sent = send(conn_sock, &msg_len, sizeof(int), 0);
	if(bytes_sent <= 0){
		return -1;
	}

	// send the message to server
	bytes_sent = send(conn_sock, message, msg_len, 0);
	if (bytes_sent <= 0){
		return -1;
	}
	return 0;
}

int send_eof_msg(int conn_sock){
	int msg_len = 0;
	int bytes_sent = send(conn_sock, &msg_len, sizeof(int), 0);
	if(bytes_sent <= 0){
		return -1;
	}
	return 0;
}

void enter_path_file() {
	printf("\nEnter path file :");
}

void main_client_upload(int client_sock, struct sockaddr_in server_addr) {
	
	char buff[BUFF_SIZE], filelink[BUFF_SIZE], filename[BUFF_SIZE], *data;
	char *endptr;
	FILE *fp = NULL;
	int errnum;
	double bytes_transfered = 0;
	
    int choice;
    while(1) {
        printf("Enter 1 if you want to upload single file\nEnter 2 if you want to disconnect with server\n\n");
        printf("Now, what is your choice? 1 or 2. Please let me know... \n");
        
		scanf("%d", &choice);
		__fpurge(stdin);

		if (choice == 1) {

				bytes_transfered = 0;
				// input file link
				enter_path_file();
				memset(buff,'\0', BUFF_SIZE);
				fgets(buff, BUFF_SIZE, stdin);
				buff[strlen(buff) - 1] = '\0';
				if (strlen(buff) == 0) break;
				strcpy(filelink, buff);
				printf("Filelink: %s\n", filelink);
				printf("length of filelink: %d\n",strlen(filelink));
				// char test_concat[BUFF_SIZE];
				// strcpy(test_concat, filelink);
				// printf("test_concat: %s\n", test_concat);
				// concat("1", test_concat);
				// printf("test concat: %s", test_concat);
				if((fp = fopen(filelink, "rb")) == NULL){
					printf("Error: File not found\n");
					continue;
				}else{
					strcpy(filename, rindex(filelink, '/') + 1);
					printf("filename :%s\n", filename);
					// send filename to server
					char add_1[2] = "1";
					char* extended_filename = malloc(sizeof(char)*BUFF_SIZE);
					extended_filename = strcat(add_1, filename);
					
					printf("filename : %s\n", filename);
					
					if(send_msg_client(client_sock, extended_filename, strlen(filename)+1) == -1){
						continue;
					}
					// Step 6: Check error message from server
					// receive error message from the server
					data = malloc(sizeof(char));
					data = recv_msg_client(client_sock);
					printf("error: %s\n", data);
					errnum = atoi(data);
					if(errnum == 1){		// if file is existent on server
						printf("Error: File is existent on server\n");
					}else if(errnum == 0){		// if there is no error
						while(errnum == 0){		// until there is an error, keep reading from file
							memset(buff,'\0', BUFF_SIZE);
							if(fread(buff, BUFF_SIZE, 1, fp) == 1){
								// send the block to server
								if(send_msg_client(client_sock, buff, sizeof(buff)) == -1){
									break;
								}
								printf("Transfered: %.2lf MB\n", (bytes_transfered += sizeof(buff)) / (1024*1024));
							}else{
								// tell server that EOF has been reached
								if(send_msg_client(client_sock, buff, sizeof(buff)) == -1){
									break;
								}
								printf("Transfered: %.2lf MB\n", (bytes_transfered += sizeof(buff)) / (1024*1024));
								
								// check if there is any error
		
								data = recv_msg_client(client_sock);
								errnum = atoi(data);

								if(send_eof_msg(client_sock) == -1){
									break;
								}
							}
							// check if there is any error
							data = recv_msg_client(client_sock);
							errnum = atoi(data);
						}
						if(errnum == -1){
							printf("\nSuccessful transfering\n");
						}else{
							printf("\nError: File transfering is interupted\n");
						}
					}
				}
				fclose(fp);
				close(client_sock);
		}
		else if (choice == 2) break;
		else {	
			printf("Wrong choice. Please check your choice again and guarantee that choice = 1 or choice = 2. \n");
			continue;
		}
	}
}

void concat(const char *s1,  char s2[BUFF_SIZE]) {
    for (int i = 0; i < strlen(s2); i++) {
		strcpy(&s2[i+1], &s2[i]);
	}
	strcpy(&s2[0], s1);
}