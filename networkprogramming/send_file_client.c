/*
Send a file over a socket.

Interface:

    ./executable [<input_path> [<sever_hostname> [<port>]]]

Defaults:

- input_path: input.tmp
- server_hostname: 127.0.0.1
- port: 12345
*/

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>

#include "common.h"

int main(int argc, char **argv) {
	char protoname[] = "tcp";
    // char *file_path = "input.tmp";
    char *file_path;
    char *server_hostname = "127.0.0.1";
    char *server_reply = NULL;
    char *user_input = NULL;
    char buffer[BUFSIZ];
    int filefd;
    int sockfd;
    ssize_t read_return;
    unsigned short server_port = 12345;

    // if (argc > 1) {
    //     file_path = argv[1];
    //     if (argc > 2) {
    //         server_hostname = argv[2];
    //         if (argc > 3) {
    //             server_port = strtol(argv[3], NULL, 10);
    //         }
    //     }
    // }
    if (argc > 1) {
        server_hostname = argv[1];
    }
    printf("tcp protocol - upload files system\n");
    int choice;
    do {
        printf("Enter 1 if you want to upload single file\nEnter 2 if you want to disconnect with server\nEnter 3 if you want to quit\n");
        printf("Now, what is your choice? 1,2 or 3. Please let me know... \n");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                printf("Enter your file path : ");
                file_path = malloc(sizeof(char));
                scanf("%s", file_path);
                filefd = open(file_path, O_RDONLY);
                // if (filefd == -1) {
                //     perror("open");
                //     exit(EXIT_FAILURE);
                // }
                while (filefd == -1) {
                    printf("File not found. Please check your file path again\n");
                    printf("Enter your file path: ");
                    file_path = malloc(sizeof(char));
                    scanf("%s", file_path);
                    filefd = open(file_path, O_RDONLY);
                }
                sockfd = connect_server(protoname, server_hostname, server_port);
                while (1) {
                    read_return = read(filefd, buffer, BUFSIZ);
                    if (read_return == 0)
                        break;
                    if (read_return == -1) {
                        perror("read");
                        exit(EXIT_FAILURE);
                    }
                    if (write(sockfd, buffer, read_return) == -1) {
                        perror("write");
                        exit(EXIT_FAILURE);
                    }
                }
                free(user_input);
                free(server_reply);
                close(filefd);

                break;
            case 2:
                exit(EXIT_SUCCESS);
                break;
            case 3:
                printf("See you again\n");
                break; 
            default:
                printf("Wrong choice. Please enter either 1 or 2 or 3.");
                break; 
        }
    } while (choice != 3);
    // filefd = open(file_path, O_RDONLY);
    // if (filefd == -1) {
    //     perror("open");
    //     exit(EXIT_FAILURE);
    // }
    // sockfd = connect_server(protoname, server_hostname, server_port);
    // while (1) {
    //     read_return = read(filefd, buffer, BUFSIZ);
    //     if (read_return == 0)
    //         break;
    //     if (read_return == -1) {
    //         perror("read");
    //         exit(EXIT_FAILURE);
    //     }
    //     if (write(sockfd, buffer, read_return) == -1) {
    //         perror("write");
    //         exit(EXIT_FAILURE);
    //     }
    // }
    // free(user_input);
    // free(server_reply);
}
