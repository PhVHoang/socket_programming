#include "client.h"

int recv_file(int ,char*, char*);
char *recv_msg(int conn_sock);
int send_msg(int conn_sock, char *message, int msg_len);
int send_eof_msg(int conn_sock);
void enter_path_file();

int recv_file(int sock, char* file_name, char* newfilename) {
	char send_str [MAX_SEND_BUF]; /* message to be sent to server*/
	int f; /* file handle for receiving file*/
	ssize_t sent_bytes, rcvd_bytes, rcvd_file_size;
	int recv_count; /* count of recv() calls*/
	char recv_str[MAX_RECV_BUF]; /* buffer to hold received data */
	size_t send_strlen; /* length of transmitted string */
	sprintf(send_str, "%s\n", file_name); /* add CR/LF (new line) */
	send_strlen = strlen(send_str); /* length of message to be transmitted */
	if( (sent_bytes = send_msg(sock, file_name, send_strlen)) < 0 ) {
		perror("send error");
		return -1;
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
    //Cho nay anh phai them ham send to server nua vi cho nay dang gui chu ko co nhan
	while ( (rcvd_bytes = recv(sock, recv_str, MAX_RECV_BUF, 0)) > 0 ) {
        recv_count++;
		rcvd_file_size += rcvd_bytes;
        printf("Recbytes: %d\n", rcvd_bytes);
        //printf("receive %d bytes of file", rcvd_file_size);
		if (write(f, recv_str, rcvd_bytes) < 0 ) {
			perror("error writing to file");
			return -1;
		}
        // ben server khi xong het r anh gui cho no mot phat deo co gi laf nos out ma
        printf("Loop here\n");
        if(rcvd_bytes == 144) break;

	}
	close(f); /* close file*/
	printf("Client Received: %d bytes in %d recv(s)\n", rcvd_file_size, recv_count);
	return rcvd_file_size;
}

char *recv_msg(int conn_sock){
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

int send_msg(int conn_sock, char *message, int msg_len){
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

void set_null(char **ptr) {
    *ptr = NULL;
}
int main(int argc, const char* argv[]) {

    /*Initialize*/
    if (argc < 2) {
        printf("Invalid Arugment !\n");
        return 0;
    }
    strcpy(serv_ip_child, argv[1]);
    memset(&server_addr, 0, sizeof(server_addr)); 
    conn_sock = socket(AF_INET,SOCK_STREAM,0);
	
    server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(serv_port);
	server_addr.sin_addr.s_addr = inet_addr(serv_ip_child);
	
	if(connect(conn_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}

    printf("Welcome to our cloud\n\n\nPlease look at this bellow menu and select your choice\n\n");
    int choice, upload_option, download_option;
    do {
        printf("1. Upload your file into cloud server\n");
        printf("2. Download file from cloud server\n");
        printf("3. Disconnect with server\n");
        printf("Please take your choice by typing 1 or 2 or 3 : ");
        scanf("%d", &choice);
        __fpurge(stdin);
        switch(choice) {
            /*Upload option*/
            case 1:
                while(1){
                    printf("        1 or 2          \n");
                    scanf("%d", &upload_option);
                    __fpurge(stdin);
                    if (upload_option == 1) {
                        bytes_transfered = 0;
                        // input file link
                        enter_path_file();
                        memset(buff,'\0', BUFF_SIZE);
                        fgets(buff, BUFF_SIZE, stdin);
                        buff[strlen(buff) - 1] = '\0';
                        if (strlen(buff) == 0) break;
                        strcpy(filelink, buff);

                        if((fp = fopen(filelink, "rb")) == NULL){
                            printf("Error: File not found\n");
                            continue;
                        }else{
                            // Step 5: Send filename to server
                            // extract filename from link
                            strcpy(filename, rindex(filelink, '/') + 1);
                            //printf("filename: %s\n", filename);
                            // send filename to server
                            extended_filename = (char*) malloc(sizeof(char)*BUFF_SIZE);
                            bzero(extended_filename,BUFF_SIZE);
                            strcat(extended_filename, "1");
                            strcat(extended_filename,filename);
                            printf("extended filename: %s", extended_filename);
                            if(send_msg(conn_sock, extended_filename, strlen(extended_filename)) == -1){
                                printf("Hard!\n");
                                continue;
                            }
                            printf("filename = %s\n", filename);
                            fflush(stdin);
                            //set_null(&extended_filename);
                            // Step 6: Check error message from server
                            // receive error message from the server
                            data = recv_msg(conn_sock);
                            printf("error number :%s\n", data);
                            errnum = atoi(data);
                            if(errnum == 1){		// if file is existent on server
                                printf("Error: File is existent on server\n");
                            }else if(errnum == 0){		// if there is no error
                                while(errnum == 0){		// until there is an error, keep reading from file
                                    memset(buff,'\0', BUFF_SIZE);
                                    if(fread(buff, BUFF_SIZE, 1, fp) == 1){
                                        // send the block to server
                                        if(send_msg(conn_sock, buff, sizeof(buff)) == -1){
                                            break;
                                        }
                                        printf("Transfered: %.2lf MB\n", (bytes_transfered += sizeof(buff)) / (1024*1024));
                                    }else{
                                        // tell server that EOF has been reached
                                        if(send_msg(conn_sock, buff, sizeof(buff)) == -1){
                                            break;
                                        }
                                        printf("Transfered: %.2lf MB\n", (bytes_transfered += sizeof(buff)) / (1024*1024));
                                        
                                        // check if there is any error
                                        data = recv_msg(conn_sock);
                                        errnum = atoi(data);

                                        if(send_eof_msg(conn_sock) == -1){
                                            break;
                                        }
                                    }
                                    // check if there is any error
                                    data = recv_msg(conn_sock);
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
                        //Step 4: Close socket
                        //close(conn_sock);
                    }
                     else if (upload_option == 2) {
                     break;
                    
                 }
                
                }
                
                  //close(conn_sock);
                   
		    break;
            
            /*Download option*/
            case 2:
                download_filename = malloc(sizeof(char)*BUFF_SIZE);
                new_download_filename = malloc(sizeof(char)*BUFF_SIZE);
                printf("Enter filename you want to download : ");
                scanf("%s", download_filename);
                printf("Enter new filename you want to save : ");
                scanf("%s", new_download_filename);
                extended_filename = (char*) malloc(sizeof(char)*BUFF_SIZE);
                bzero(extended_filename,BUFF_SIZE);
                strcat(extended_filename, "2");
                strcat(extended_filename,download_filename);
                recv_file(conn_sock,extended_filename, new_download_filename);
	            /* close socket*/
                // if(close(conn_sock) < 0) {
                //     perror("socket close error");
                //     exit(EXIT_FAILURE);
                // }
                // close(conn_sock);
                break;
            case 3:
                close(conn_sock);
                printf("Thanks for using our serveice. See you again.\nIf you have any problem, please contact us via email\n      cloud.hedspi@hust.edu.vn\n");
                break;
            default:
                printf("Wrong choice. Please only type 1, 2 or 3\n");
                break;
        }
    } while (choice != 3);

    return 0;
}