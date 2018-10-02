#ifndef _CLIENT_H_
#define _CLIENT_H_
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

#ifndef _CTYPE_H_
#include <ctype.h>
#endif

#ifndef _SIGNAL_H_
#include <signal.h>
#endif

#ifndef _SERVER_H_
#include "server.h"
#endif

#ifndef _PTHREAD_
#include <pthread.h>
#endif

#define maxbuf_size 1024
#define nbq 10
#define PROC_OK  0
#define PROC_NG  -1

#define MAX_ITEM_ON_INVENTORY 100

#define MAX_MENU_BUFFER_SIZE 100
#define MAX_NAME_ITEM_MENU 20
#define MAX_MONEY_BUFFER_SIZE 20

/*typedef struct{
  char name[20];
  int num;
  }com;*/

typedef struct{
  char name[100];
  float cost;
  int max_element;
  int current_number_element;  
  int warring_number_element;
} item;

typedef struct{
  char name_client[100];
  int id_client;
  char server_address_ip[16];
  uint16_t port;
  
  int number_item;
  item list_item[MAX_ITEM_ON_INVENTORY];
  
}private_data_of_client;

typedef struct{
  private_data_of_client data;
  int up_fd;
  pthread_mutex_t mutex_obj;
  //pthread_t sub_thread;
}arg_struct;

void config_module();
void sale(int sock, arg_struct *arg);
void equipMain(int sock);
void commoditySales(int choice, int sock);
int beverages();
void chosen(int choice);
float drink_cost(int choice);
void readCommodityVM(char str[100], int sock);
void service_down(int client_socket_ds, arg_struct *arg);
void *thread_down_handler(void* arg_struct);
