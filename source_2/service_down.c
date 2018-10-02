#include "client.h"

void service_down(int client_socket_ds, arg_struct *arg){
  char buffer[MAX_BUFFER_SIZE], minibuff[MAX_BUFFER_SIZE];        
  int recvMsgSize, i, status, n, t, flag;

  //printf("Downloading command!\n");
  if ( (recvMsgSize = recv(client_socket_ds, buffer, MAX_BUFFER_SIZE, 0)) < 0){
    printf("recv() failed!\n");
    exit(1);
  }

  while (recvMsgSize > 0){
    buffer[recvMsgSize] = '\0';
    //printf("Recived packet: {|%s|}\n", buffer);
    n = strlen("GetInfo");
    strcpy( minibuff, buffer);
    minibuff[n] = 0;
    //printf("Header: {|%s|}\n", minibuff);
    if( (strcmp( minibuff, "GetInfo") == 0) ){      
      // Send infomation
      // Lock mutex
      //printf("Lock mutex!\n");
      // Lock mutex
      while( pthread_mutex_lock( &(arg->mutex_obj) ) != 0){
	printf("Locking mutex");
	sleep(1);
      }
      //printf("Get Infomation command!\n");
      memset(minibuff, 0, MAX_BUFFER_SIZE);
      sprintf( minibuff, "Inventory client %d:\n", (arg->data).id_client);
      n = strlen(minibuff);
      for(t= 0; t < (arg->data).number_item; t++ ){
	sprintf( &(minibuff[n]), "\tName item: %s\n", ((arg->data).list_item[t]).name);
	n = strlen(minibuff);
	sprintf( &(minibuff[n]), "\tCost: %g\n", ((arg->data).list_item[t]).cost);
	n = strlen(minibuff);
	sprintf( &(minibuff[n]), "\tElement: %d/%d warring on %d\n=====================\n", ((arg->data).list_item[t]).current_number_element, ((arg->data).list_item[t]).max_element, ((arg->data).list_item[t]).warring_number_element);
	n = strlen(minibuff);
      }
      send( arg->up_fd, minibuff, n, 0);
      // Unlock
      while( pthread_mutex_unlock( &(arg->mutex_obj)) != 0){
	printf("Unlocking mutex!\n");
	sleep(1);
	}
      goto Continue;
    }

    n = strlen("Warring");
    strcpy( minibuff, buffer);
    minibuff[n] = 0;
    if( (strcmp( minibuff, "Warring") == 0) ){      
      // Send warring infomation
      while( pthread_mutex_lock( &(arg->mutex_obj)) != 0){
	sleep(1);
      }
      memset(minibuff, 0, MAX_BUFFER_SIZE);
      flag = 0;
      for(t= 0; t < (arg->data).number_item; t++ ){
	if( ((arg->data).list_item[t]).current_number_element <= ((arg->data).list_item[t]).warring_number_element ){
	  flag = 1;
	  break;
	}
      }
      // Check if has some item warring
      if( flag ){
	sprintf( minibuff, "Warring client %d:\n", (arg->data).id_client);
	n = strlen(minibuff);
	for(t= 0; t < (arg->data).number_item; t++ ){
	  if( ((arg->data).list_item[t]).current_number_element <= ((arg->data).list_item[t]).warring_number_element ){
	    sprintf( &(minibuff[n]), "\tName item: %s\n", ((arg->data).list_item[t]).name);
	    n = strlen(minibuff);
	    sprintf( &(minibuff[n]), "\tCost: %g\n", ((arg->data).list_item[t]).cost);
	    n = strlen(minibuff);
	    sprintf( &(minibuff[n]), "\tElement: %d/%d warring on %d\n=====================\n", ((arg->data).list_item[t]).current_number_element, ((arg->data).list_item[t]).max_element, ((arg->data).list_item[t]).warring_number_element);
	    n = strlen(minibuff);	    
	  }	  
	}
	send( arg->up_fd, minibuff, n, 0);      
	while( pthread_mutex_unlock( &(arg->mutex_obj) ) != 0){
	  sleep(1);
	}
      }
      goto Continue;
    }    

  Continue:
    if ((recvMsgSize = recv(client_socket_ds, buffer, MAX_BUFFER_SIZE, 0)) < 0){
      printf("recv() failed");      
      exit(1);
    }
  }
  
  return;
}
