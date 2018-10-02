#include "client.h"

int recv_file(int ,char*, char*);
char *recv_msg(int conn_sock);
int send_msg(int conn_sock, char *message, int msg_len);
int send_eof_msg(int conn_sock);
void enter_path_file();
int create_folder(char* folder_name);

int recv_file(int sock, char* file_name, char* newfilename) {
	char send_str [MAX_SEND_BUF]; 
	int f; 
	ssize_t sent_bytes, rcvd_bytes, rcvd_file_size;
	int recv_count; 
	char recv_str[MAX_RECV_BUF]; 
    char error[MAX_RECV_BUF] = "File not found\n";
	size_t send_strlen; 
	sprintf(send_str, "%s\n", file_name); 
	send_strlen = strlen(send_str); 
	if( (sent_bytes = send_msg(sock, file_name, send_strlen)) < 0 ) {
		perror("send error");
		return -1;
	}
	if ( (f = open(newfilename, O_WRONLY|O_CREAT, 0644)) < 0 ) {
		perror("error creating file");
		return -1;
	}
    
	recv_count = 0; 
	rcvd_file_size = 0; 

	while ( (rcvd_bytes = recv(sock, recv_str, MAX_RECV_BUF, 0)) > 0 ) {
        if (strcmp(recv_str, error) == 0) {
            printf("File not found in server\n");
            break;
        }
        recv_count++;
		rcvd_file_size += rcvd_bytes;
		if (write(f, recv_str, rcvd_bytes) < 0 ) {
			perror("error writing to file");
			return -1;
		}
        if(rcvd_bytes < 256) break;

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

int create_folder(char* folder_name) {
    return mkdir(folder_name, 0755);
}

void cmdLOUT (char *str) {
	char code[CODE_LEN + 1];
	char pass[PASS_LEN + 1];

	sscanf(str, "%s", code);

	if (strcmp(code, "LOUT")) return;
	
	printf("Enter your password: ");
	__fpurge(stdin);
	scanf("%s", pass);

	strcat(str, " ");
	strncat(str, pass, PASS_LEN);
}

// Check exit
_Bool wannaExit (char *buff) {
	if (buff[0] == '\0' || buff[0] == '\n') return 1;
	else return 0;
}

// Check arguments is valid or not. If valid ip -> *serv_ip, port -> &serv_port
void validArguments (int argc, char *argv[], char *serv_ip, int *serv_port) {
	if (argc > 2) {
		// Check valid ip address
		struct sockaddr_in tmp_addr;
		if (inet_pton(AF_INET, argv[1], &(tmp_addr.sin_addr)) == 0) {
			printf("IP Address is invalid\n!");
			exit(EXIT_FAILURE);
		} else {
			strcpy(serv_ip, argv[1]);
		}

		// Check valid port
		int i;
		char *port_str = argv[2];
		for (i = 0; port_str[i] != '\0'; i++) {
			if (!isdigit(port_str[i])) {
				printf("Port is invalid\n!");
				exit(EXIT_FAILURE);
			}
		}
		if (port_str[i] == '\0') *serv_port = atoi(port_str);
	} else {
		printf("(ERROR) To few arguments!\n");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, const char* argv[]) {

    /*Initialize*/
    if (argc < 3) {
        printf("Invalid Arugment !\n");
        return 0;
    }
    strcpy(serv_ip_child, argv[1]);
    memset(&server_addr, 0, sizeof(server_addr)); 
    conn_sock = socket(AF_INET,SOCK_STREAM,0);
	
    server_addr.sin_family = AF_INET;
	serv_port = (in_port_t) strtol(argv[2], &endptr, 10);
    server_addr.sin_port = htons(serv_port);
	
	server_addr.sin_addr.s_addr = inet_addr(serv_ip_child);
	
	if(connect(conn_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}

    // Register - Login - Logout Menu
    int login_option, choice, upload_option, download_option;
    while (1) {
        printf("*************************************************************\n");
        printf("**********************Files manager**************************\n");
        printf("*******                                          ***********\n");
        printf("*******               1. REGISTER                 ***********\n");
        printf("*******               2. LOGIN                    ***********\n");
        printf("*******                                           ***********\n");
        printf("*************************************************************\n");
        printf("*************************************************************\n\n\n");
        printf("Please choose 1 if you want to create a new account\n");
        printf("or choose 2 if you already have an account\n");
        printf("Enter your choice : ");
        scanf("%d", &login_option);
        __fpurge(stdin);
        switch(login_option) {
            case 1:
                printf("\nOK. Please enter this bellow form to complete your register: \n");
                printf("*******   RESG your_new_username your_password ***********");
                __fpurge(stdin);
                printf("\nEnter a command: ");
                bzero(buff, BUFF_SIZE);
                fgets(buff, BUFF_SIZE, stdin);
                buff[strlen(buff) - 1] = '\0';

                // Check exit
                if (wannaExit(buff)) return;

                // Xu ly neu laf command LOUT
                cmdLOUT(buff);
                msg_len = strlen(buff) + 1;

                if( send_msg(conn_sock, buff, msg_len) == -1) {
                    printf("Connection closed!\n");
                    break;
                }

                //receive echo reply
                bytes_received = recv(conn_sock, buff, BUFF_SIZE-1, 0);
                if(bytes_received <= 0){
                    printf("\nError!Cannot receive data from sever!\n");
                    break;
                }

                buff[bytes_received] = '\0';
                printf("\nReply from server: %s\n", buff);
                printf("------------------------------------------------------------------------------\n");
                break;
            case 2:
                printf("OK. Please enter this bellow form to complete your login: \n");
                printf("*******   LGIN your_new_username your_password  ***********");
                __fpurge(stdin);
                printf("\nEnter a command: ");
                bzero(buff, BUFF_SIZE);
                fgets(buff, BUFF_SIZE, stdin);
                buff[strlen(buff) - 1] = '\0';

                if (wannaExit(buff)) return;
                msg_len = strlen(buff) + 1;

                if( send_msg(conn_sock, buff, msg_len) == -1) {
                    printf("Connection closed!\n");
                    break;
                }
                
                bytes_received = recv(conn_sock, buff, BUFF_SIZE-1, 0);
                if(bytes_received <= 0){
                    printf("\nError!Cannot receive data from sever!\n");
                    break;
                }

                buff[bytes_received] = '\0';
                printf("\nReply from server: %s\n", buff);
                printf("------------------------------------------------------------------------------\n");
                if (buff[0] == '1' && buff[1] == '0') {
                    printf("Welcome to our cloud\n\n\nPlease look at this bellow menu and select your choice\n\n");
                    do {
                        printf("1. Upload your file into cloud server\n");
                        printf("2. Download file from cloud server\n");
                        printf("3. LOGOUT\n");
                        printf("Please take your choice by typing 1 or 2 or 3 : ");
                        scanf("%d", &choice);
                        __fpurge(stdin);
                        switch(choice) {
                            /*Upload option*/
                            case 1:
                                while(1){
                                    printf("\nEnter 1 if you want to upload single file\n");
                                    printf("Enter 2 if you want to choose a difference option\n");
                                    printf("And, what is your choice : \n");
                                    printf("            1 or 2          \n");
                                    printf("Enter here: ");
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
                                            
                                            strcpy(filename, rindex(filelink, '/') + 1);
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
                                            data = recv_msg(conn_sock);
                                            printf("error number :%s\n", data);
                                            errnum = atoi(data);
                                            if(errnum == 1){		// if file is existent on server
                                                printf("Error: File is existent on server\n");
                                            }else if(errnum == 0){		// if there is no error
                                                while(errnum == 0){		// until there is an error, keep reading from file
                                                    memset(buff,'\0', BUFF_SIZE);
                                                    if(fread(buff, BUFF_SIZE, 1, fp) == 1){
                                                        if(send_msg(conn_sock, buff, sizeof(buff)) == -1){
                                                            break;
                                                        }
                                                        printf("Uploaded: %.2lf MB\n", (bytes_transfered += sizeof(buff)) / (1024*1024));
                                                    }else{
                                                        if(send_msg(conn_sock, buff, sizeof(buff)) == -1){
                                                            break;
                                                        }
                                                        printf("Uploaded: %.2lf MB\n", (bytes_transfered += sizeof(buff)) / (1024*1024));
                                                        
                                                        data = recv_msg(conn_sock);
                                                        errnum = atoi(data);

                                                        if(send_eof_msg(conn_sock) == -1){
                                                            break;
                                                        }
                                                    }
                                                    data = recv_msg(conn_sock);
                                                    errnum = atoi(data);
                                                }
                                                if(errnum == -1){
                                                    printf("\nSuccessful Uploading\n");
                                                }else{
                                                    printf("\nError: File uploading is interupted\n");
                                                }
                                            }
                                        }
                                        fclose(fp);
                                    
                                    }
                                    else if (upload_option == 2) {
                                    break;
                                    
                                }
                                
                                }
                                
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
                                break;
                            case 3:
                                printf("OK. Please enter this bellow form to complete your logout: \n");
                                printf("*******   LOUT  ***********");
                                __fpurge(stdin);
                                printf("\nEnter a command: ");
                                bzero(buff, BUFF_SIZE);
                                fgets(buff, BUFF_SIZE, stdin);
                                buff[strlen(buff) - 1] = '\0';

                                // Check exit
                                if (wannaExit(buff)) return;
                                cmdLOUT(buff);
                                msg_len = strlen(buff) + 1;

                                if (send_msg(conn_sock, buff, msg_len) == -1) {
                                    printf("Connection closed!\n");
                                    break;
                                }

                                //receive echo reply
                                bytes_received = recv(conn_sock, buff, BUFF_SIZE-1, 0);
                                if(bytes_received <= 0){
                                    printf("\nError!Cannot receive data from sever!\n");
                                    break;
                                }

                                buff[bytes_received] = '\0';
                                printf("\nReply from server: %s\n", buff);
                                break;
                            default:
                                printf("Wrong choice. Please only type 1, 2 or 3\n");
                                break;
                        }
                    } while (choice != 3);
                }
                break;
            default:
                printf("Wrong choice. Make sure that your choice is 1 or 2\n");
                break;
        }
    }
       
    close(conn_sock);
    return 0;
}