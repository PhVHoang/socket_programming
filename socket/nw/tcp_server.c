
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "user.h"
#include "session.h"
#include "app.h"

#define USER_FILE "account.txt"

#define BACKLOG 100   /* Number of allowed connections */
#define BUFF_SIZE 2048

void validArguments (int argc, char *argv[], int *port);

int no_user = 0;
int no_session = 0;

int main(int argc, char *argv[]) {
	// Read list-user from file
	readUser(USER_FILE);

	int port = 0;
	// checking arguments
	validArguments(argc, argv, &port);
 
	int listen_sock, conn_sock; /* file descriptors */
	char recv_data[BUFF_SIZE];
	int bytes_sent, bytes_received;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in *client; /* client's address information */
	int sin_size;
	
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}
	
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);   /* Remember htons() from "Conversions" section? =) */
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */
	if(bind(listen_sock, (struct sockaddr*)&server, sizeof(server))==-1){ /* calls bind() */
		perror("\nError: ");
		return 0;
	}

	if(listen(listen_sock, BACKLOG) == -1){  /* calls listen() */
		perror("\nError: ");
		return 0;
	}
	
	char output[BUFF_SIZE];
	int bytes_output;

	while(1){
		//accept request
		sin_size = sizeof(struct sockaddr_in);
		client = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
		if ((conn_sock = accept(listen_sock,( struct sockaddr *)client, &sin_size)) == -1)
			perror("\nError: ");

		// Add session
		if (newSession(client) == FN_ERROR) {
			printf("Error: Number of sessions is maximum!");
			continue;
		}

		printf("You got a connection from %s\n", inet_ntoa(client->sin_addr) ); /* prints client's IP */


		//start conversation
		while(1){
			//receives message from client
			bytes_received = recv(conn_sock, recv_data, BUFF_SIZE - 1, 0); //blocking
			if (bytes_received <= 0) {
				printf("\nConnection closed\n");
				break;
			} else {
				recv_data[bytes_received] = '\0';
				printf("\nReceive: |%s|\n", recv_data);

				Output *op = processCmd(client, recv_data);
				bytes_output = op2str(op, output, BUFF_SIZE);
			}
			
			//echo to client
			bytes_sent = send(conn_sock, output, bytes_output, 0); /* send to the client welcome message */
			if (bytes_sent <= 0){
				printf("\nConnection closed\n");
				break;
			}
		}//end conversation
		close(conn_sock);

		// Remove from session
		removeSession(client);

		// Write list-user to file
	
	}

	
	close(listen_sock);
	return 0;
}

void validArguments (int argc, char *argv[], int *port) {
	if (argc > 1) {
		// Check valid port
		int i;
		char *port_str = argv[1];
		for (i = 0; port_str[i] != '\0'; i++) {
			if (!isdigit(port_str[i])) {
				printf("Port is invalid\n");
				exit(EXIT_FAILURE);
			}
		}
		if (port_str[i] == '\0') *port = atoi(port_str);
	} else {
		printf("(ERROR) To few arguments!\n");
		exit(EXIT_FAILURE);
	}
}
