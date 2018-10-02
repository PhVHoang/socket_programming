#include "server.h"

void *thread_handler(void *arg_data_struct){
  client_define *client_define_struct;
  int server_socket_ds;                    
  int client_socket_ds;

  struct sockaddr_in serverAddr; 
  struct sockaddr_in clientAddr; 
  //unsigned short servPort;     
  unsigned int clientLen;        
  char sendBuffer[MAX_BUFFER_SIZE];

  int shmid_thread;
  char *segptr_thread;
  key_t Sign_thread;

  int i;

  pthread_t processID;
  FILE *f_log;

  time_t rawtime;
  struct tm *timeinfo;

  int rtn;
  const int       optVal = 1;
  const socklen_t optLen = sizeof(optVal);  

  client_define_struct = arg_data_struct;
  
  //printf("Thread %d is online!Will listen on port %hu\n", pthread_self(), client_define_struct->port);

  /* Create shared memory segment */
  Sign_thread = ftok( "/boot", '3');
  printf("Key on main thread %d\n", Sign_thread);
  // Create new share memory segment, if cause a error try as client
  if((shmid_thread = shmget( Sign_thread, sizeof(data_server), IPC_CREAT|IPC_EXCL|0777)) == -1){
    printf("Can't creat share segment memory on thread %d!\n", client_define_struct->id_client);
    //Segment probably already exists - try as a client
    if((shmid_thread = shmget(Sign_thread, sizeof(data_server), 0)) == -1) {
      printf("shmget error!\n");
      exit(1);
    } else {
      printf("Success! Open share segment memory on thread %d!\n", client_define_struct->id_client);
    }
  } else {
    printf("Success! Creat share segment memory on thread %d!\n", client_define_struct->id_client);
  }

  /* Attack shared memory segment */
  // Attach (map) the shared memory segment into the current process
  if( (segptr_thread = (char *)shmat(shmid_thread, 0, 0)) == (char *)-1 ){
    printf("Can't attach shared memory segment!\n");
    exit(1);
  } else {
    printf("Success! Attached share segment memory on thread [%d %d]!\n", i, client_define_struct->id_client);
  }
  

  if ((server_socket_ds = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    printf("socket() failed!\n");
    return;
  }

  rtn = setsockopt( server_socket_ds, SOL_SOCKET, SO_REUSEADDR, (void*) &optVal, optLen);

  assert(rtn == 0);  
  
  memset(&serverAddr, 0, sizeof(serverAddr));   
  serverAddr.sin_family = AF_INET;               
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
  serverAddr.sin_port = htons( client_define_struct->port);
  
  while (bind(server_socket_ds, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0){
    printf("Binding socket on to port: %d\n", client_define_struct->port);
  }
  
  if (listen(server_socket_ds, MAX_QUEUE_CONNECTION) < 0){
    printf("Server cause a problem when listening!\n");
    return;
  }
  
  while(1){    
    clientLen = sizeof(clientAddr);
    if ((client_socket_ds = accept(server_socket_ds, (struct sockaddr *) &clientAddr, &clientLen)) < 0){	
      //printf("Warring: accept failed!\n");
      return;
    }
    if ((processID = fork()) < 0){
      //printf("accept() failed!\n");
      return;
    } else{
      if (processID == 0){  /* If this is the child process */
	close(server_socket_ds);   /* Child closes parent socket */
	//printf("Client ID: %s", inet_ntoa(clientAddr.sin_addr));
	f_log = fopen("history.log", "a");
	if( f_log == NULL){
	  printf("Cannot open file history.log!!\n");
	  return;
	}
	time( &rawtime );
	timeinfo = localtime( &rawtime);	
	fprintf( f_log, "Client ID: %d %s just connected!\n%s\n", client_define_struct->id_client, client_define_struct->name_client, asctime (timeinfo));
	fclose(f_log);
	//printf( "Client ID: %d %s just connected!\n", client_define_struct->id_client, client_define_struct->name_client);
	while( pthread_mutex_lock( &(((data_server*)segptr_thread)->mutex_obj) ) != 0){
	  sleep(1);
	}
	(((data_server*)segptr_thread)->status)[client_define_struct->id_client] = 1;
	sprintf( (((data_server*)segptr_thread)->ip_clients)[client_define_struct->id_client], "%s", inet_ntoa(clientAddr.sin_addr));
	((((data_server*)segptr_thread)->ip_clients)[client_define_struct->id_client])[15] = 0;
	while( pthread_mutex_unlock( &(((data_server*)segptr_thread)->mutex_obj) ) != 0){
	  sleep(1);
	}
	service_client(client_socket_ds, client_define_struct, &(((data_server*)segptr_thread)->mutex_obj));
	f_log = fopen("history.log", "a");
	if( f_log == NULL){
	  printf("Cannot open file history.log!!\n");
	  return;
	}
	time( &rawtime );
	timeinfo = localtime( &rawtime);	
	fprintf( f_log, "Client ID: %d %s just shutdown!\n%s\n", client_define_struct->id_client, client_define_struct->name_client, asctime (timeinfo));
	fclose(f_log);
	while( pthread_mutex_lock( &(((data_server*)segptr_thread)->mutex_obj) ) != 0){
	  sleep(1);
	}
	(((data_server*)segptr_thread)->status)[client_define_struct->id_client] = 0;
	while( pthread_mutex_unlock( &(((data_server*)segptr_thread)->mutex_obj) ) != 0){
	  sleep(1);
	}
	//printf( "Client ID: %d %s just shutdown!\n", client_define_struct->id_client, client_define_struct->name_client);
      }
    }

  }
  printf("Terminal thread %d\n", client_define_struct->id_client);
  return;
}
