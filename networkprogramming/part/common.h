#ifndef COMMON
#define COMMON

#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#define BUFF_SIZE 819600
#define WINDOW_SIZE 102400
#define  LISTEN_ENQ 5 /* for listen backlog */
#define  MAX_RECV_BUF 256
#define  MAX_SEND_BUF 256
#define BACKLOG 2   /* Number of allowed connections */
#define STRING_SIZE 819600



void sig_chld(int);

#endif