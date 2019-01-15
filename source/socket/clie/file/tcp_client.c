/*
*   Network programming - 20171
*   Homework 4: Session management
*   Author: Hoang Van Hai
*/

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#define BUFF_SIZE 2048

#define PASS_LEN 30			// do dai lon nhat cua password
#define CODE_LEN 4          // do dai lon nhat cua ma lenh

void cmdLOUT (char *str);
void validArguments (int argc, char *argv[], char *serv_ip, int *serv_port);
_Bool wannaExit (char *buff);

int main(int argc, char *argv[]) {
	int server_port = 0;
	char server_ip[16] = "";		// 16 = (max length of ipv4 string) + 1
	// Check arguments
	validArguments(argc, argv, server_ip, &server_port);

	int client_sock;
	char buff[BUFF_SIZE];
	struct sockaddr_in server_addr; /* server's address information */
	int msg_len, bytes_sent, bytes_received;

	client_sock = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = inet_addr(server_ip);

	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0) {
		printf("\nError!Can not connect to sever! Client exit imediately!");
		return 0;
	}

	printf("HOANG VAN HAI - 20141355\n");

	while(1){
		//send message
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

		bytes_sent = send(client_sock, buff, msg_len, 0);
		if(bytes_sent <= 0){
			printf("\nConnection closed!\n");
			break;
		}

		//receive echo reply
		bytes_received = recv(client_sock, buff, BUFF_SIZE-1, 0);
		if(bytes_received <= 0){
			printf("\nError!Cannot receive data from sever!\n");
			break;
		}

		buff[bytes_received] = '\0';
		printf("\nReply from server: %s\n", buff);
		printf("------------------------------------------------------------------------------\n");
	}

	//Step 4: Close socket
	close(client_sock);
	return 0;
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
