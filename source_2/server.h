#ifndef _SERVER_H_
#define _SERVER_H_
#endif

#ifndef _STDIO_H_
#include <stdio.h>
#endif

#ifndef _SOCKET_H_
#include <sys/socket.h> 
#endif

#ifndef _INET_H_
#include <arpa/inet.h>
#endif

#ifndef _STDLIB_H_
#include <stdlib.h>
#endif

#ifndef _STRING_H_
#include <string.h>
#endif

#ifndef _UNISTD_H_
#include <unistd.h>
#endif

#ifndef _TIME_H_
#include <time.h>
#endif

#ifndef _TYPES_H_
#include <sys/types.h>
#endif

#ifndef _IPC_H_
#include <sys/ipc.h>
#endif

#ifndef _SHM_H_
#include <sys/shm.h>
#endif

#ifndef _ASSERT_H_
#include <assert.h>
#endif

#ifndef _PTHREAD_
#include <pthread.h>
#endif


#define MAX_CLIENTS 100

#define MAX_QUEUE_CONNECTION 10    //Max connection queue to connect server

#define MAX_BUFFER_SIZE 102400
#define MAXQUEST 10
#define PROC_OK  0
#define PROC_NG  -1

typedef struct{
  char name_client[100];
  int id_client;
  uint16_t port;

} client_define;

typedef struct{
  int current_number_client;
  client_define clients[MAX_CLIENTS];
} private_server_data;

typedef struct{
  private_server_data core_data;
  int threads;
  char status[MAX_CLIENTS];
  char ip_clients[MAX_CLIENTS][16];
  pthread_mutex_t mutex_obj;
}data_server;

typedef struct {
  int num;
  char question[1024];
  char a[512];
  char b[512];
  char c[512];
  char d[512];
  char answer;
}question;

typedef struct {
  char user[20];
  char pass[20];
}acc;

void prepair_communication();
void service_client(int client_socket_ds, client_define *client_struct, pthread_mutex_t *mutex_obj_screen);
void salesMng(int clntSocket,char buffer[MAX_BUFFER_SIZE]);
void equipInfoAccess(char buffer[MAX_BUFFER_SIZE]);

void *thread_handler(void *arg_data_struct);
