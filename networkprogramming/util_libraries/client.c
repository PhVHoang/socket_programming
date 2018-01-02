#include "client_upload.h"
#include "client_download.h"
int serv_port = 5550;
char* serv_ip="127.0.0.1";

int main(int argc, const char* argv[]) {

    /*Client - Server*/
    int client_sock;
    int err_signal;
    char *receive_msg;
	struct sockaddr_in server_addr; /* server's address information */
	char serv_ip_child[16];
    char *filename, *newfilename;
	//strcpy(serv_ip_child, argv[1]);
	// client_sock = socket(AF_INET,SOCK_STREAM,0);
	memset(&server_addr, 0, sizeof(server_addr)); /* zero-fill srv_addr structure*/
	/* create a client socket */
	client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(serv_port);
	server_addr.sin_addr.s_addr = inet_addr(serv_ip);
	printf("Listening on port number %d ...\n", ntohs(server_addr.sin_port));
	//Step 3: Request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}

    printf("Welcome to our cloud\n\n\nPlease look at this bellow menu and select your choice\n\n");
    int choice;
    do {
        printf("1. Upload your file into cloud server\n");
        printf("2. Download file from cloud server\n");
        printf("3. Disconnect with server\n");
        printf("Please take your choice by typing 1 or 2 or 3 : ");
        scanf("%d", &choice);
        __fpurge(stdin);
        switch(choice) {
            case 1:
                main_client_upload(client_sock, server_addr);
                break;
            case 2:
                
                filename = malloc(sizeof(char)*BUFF_SIZE);
                newfilename = malloc(sizoeof(char)*BUFF_SIZE);
                printf("Enter filename you want to download : ");
                scanf("%s", filename);
                printf("Enter new filename you want to save : ");
                scanf("%s", newfilename);
                main_client_download(client_sock, filename, newfilename);
                if(close(client_sock) < 0) {
		            perror("socket close error");
		            exit(EXIT_FAILURE);
	            }
                break;
            case 3:
                printf("Thanks for using our serveice. See you again.\nIf you have any problem, please contact us via email\n      cloud.hedspi@hust.edu.vn\n");
                break;
            default:
                printf("Wrong choice. Please only type 1, 2 or 3\n");
                break;
        }
    } while (choice != 3);
    return 0;
}

void disconnected_with_server() {

}