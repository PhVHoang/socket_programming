#include "server.h"

void service_client(int client_socket_ds, client_define *client_struct, pthread_mutex_t *mutex_obj_screen){  
  char buffer[MAX_BUFFER_SIZE], minibuff[MAX_BUFFER_SIZE];        
  int recvMsgSize, i, status, n;
  time_t rawtime;
  struct tm *timeinfo;
  FILE *f_log;
  
  if ( (recvMsgSize = recv(client_socket_ds, buffer, MAX_BUFFER_SIZE, 0)) < 0){
    printf("recv() failed!\n");
    exit(1);
  }

  while (recvMsgSize > 0){
    buffer[recvMsgSize] = '\0';
    n = strlen("Warring");
    strcpy( minibuff, buffer);
    minibuff[n] = 0;
    if( (strcmp( minibuff, "Warring") == 0) ){      
      // Display warring
      // Lock mutex
      while( pthread_mutex_lock(mutex_obj_screen) != 0){
	sleep(1);
      }
      puts(buffer);
      printf("\n\nPress enter please!");
      // Unlock
      while( pthread_mutex_unlock(mutex_obj_screen) != 0){
	sleep(1);
      }
      goto Continue;
    }

    n = strlen("Infomation");
    strcpy( minibuff, buffer);
    minibuff[n] = 0;
    if( (strcmp( minibuff, "Infomation") == 0) ){      
      // Display infomation
      while( pthread_mutex_lock(mutex_obj_screen) != 0){
	sleep(1);
      }
      //printf("Client: %d %s\n%s", client_struct->id_client, client_struct->name_client, buffer);
      puts(buffer);
      printf("\n\nPress enter please!");
      while( pthread_mutex_unlock(mutex_obj_screen) != 0){
	sleep(1);
      }
      goto Continue;
    }
    n = strlen("quit");
    strncpy( minibuff, buffer, n);
    minibuff[n] = 0;
    if( (strcmp( minibuff, "quit") == 0) ){      
      return;
    }
    n = strlen("Inventory");
    strcpy( minibuff, buffer);
    minibuff[n] = 0;
    //printf("Header: %s\n", minibuff);
    if( (strcmp( minibuff, "Inventory") == 0) ){      
      // Display infomation
      while( pthread_mutex_lock(mutex_obj_screen) != 0){
	sleep(1);
      }
      puts(buffer);
      while( pthread_mutex_unlock(mutex_obj_screen) != 0){
	sleep(1);
      }
      //printf("\n\nPress enter please!");
      goto Continue;
    }    

    time( &rawtime );
    timeinfo = localtime( &rawtime);
    f_log = fopen("history.log", "a");
    if( f_log == NULL){
      printf("Cannot open file history.log!!\n");
      return;
    }

    //fprintf( f_log, "Client ID: %d %s %d %d %d %d %d %d\nBought: %s\n==================\n", client_struct->id_client, client_struct->name_client,  rawtime.tm_mday, rawtime.tm_mon + 1, rawtime.tm_year + 1900, rawtime.tm_hour, rawtime.tm_min, rawtime.tm_sec , buffer);
    fprintf( f_log, "Client ID: %d %s %sBought: %s\n=========================================\n", client_struct->id_client, client_struct->name_client, asctime (timeinfo), buffer);
    fclose(f_log);
  Continue:
    if ((recvMsgSize = recv(client_socket_ds, buffer, MAX_BUFFER_SIZE, 0)) < 0){
      printf("recv() failed");      
      exit(1);
    }
  }
  
  return;
}
