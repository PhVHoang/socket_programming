#include "server.h"

int main(int argc, char *argv[]){
  int i, n, t, counts;
  FILE *private_data;
  pthread_t thread_arr[MAX_CLIENTS];
  int shmid;
  char *segptr, buffer[MAX_BUFFER_SIZE];
  key_t Sign;
  char choice;

  int socket_fd;
  struct sockaddr_in server_socket;

  /* Create shared memory segment */
  Sign = ftok( "/boot", '3');
  printf("Key on main thread %d\n", Sign);
  // Create new share memory segment, if cause a error try as client
  if((shmid = shmget( Sign, sizeof(data_server), IPC_CREAT|IPC_EXCL|0777)) == -1){
    printf("Can't creat share segment memory on main function!\n");
    //Segment probably already exists - try as a client
    if((shmid = shmget( Sign, sizeof(data_server), 0)) == -1) {
      printf("shmget error!\n");
      exit(1);
    } else {
      printf("Success! Open share segment memory on main thread!\n");
    }
  } else {
    printf("Success! Created share segment memory on main thread!\n");
  }

  /* Attack shared memory segment */
  // Attach (map) the shared memory segment into the current process
  if( (segptr = (char *)shmat(shmid, 0, 0)) == (char *)-1 ){
    printf("Can't attach shared memory segment!\n");
    exit(1);
  } else {
    printf("Success! Attached share segment memory on main thread!\n");
  }
  
  // Load data
  if( NULL == (private_data = fopen("config_server.data", "r"))){
    printf("Your system was hack?\nConfig data server not found!!\n");
    // Remove shared memory segment
    shmctl(shmid, IPC_RMID, 0);
    exit(1);
  }

  fread( &(((data_server*)segptr)->core_data) , sizeof(private_server_data), 1, private_data);

  fclose(private_data);
  // Init data_server;
  for( i = 0; i < (((data_server*)segptr)->core_data).current_number_client; i++){
    (((data_server*)segptr)->status)[i] = 0;
  }

  // Create mutex
  if(pthread_mutex_init( &(((data_server*)segptr)->mutex_obj), NULL) != 0){
    printf("Mutex init failed!\n");
    // Remove shared memory segment
    shmctl(shmid, IPC_RMID, 0);
    return 1;
  }
  
  // Create cluster thread
  for(i = 0; i < (((data_server*)segptr)->core_data).current_number_client; i++){
    printf("Creating new thread %d...\n", i + 1);
    while( pthread_create( &(thread_arr[i]), NULL ,  thread_handler, (void*) &( (((data_server*)segptr)->core_data).clients[i] ) ) < 0){
      printf("\tCreating thread %d...\n", i + 1);
    }
  }

  choice = 0;
  while( choice != 'q'){
    system("clear");
    printf("1) List all client online.\n");
    printf("2) List all warring client.\n");
    printf("3) Infomation a client.\n");
    printf("q) Quit\n\nChoice: ");
    scanf("%c", &choice);
    while( getchar() != '\n');
    
    switch( choice ){
      
    case '1':{
      system("clear");
      counts = 0;
      for( n = 0; n < (((data_server*)segptr)->core_data).current_number_client + 2; n++ ){
	if( 1 == (((data_server*)segptr)->status)[n]){
	  printf("Client %d %s is online.\n", (((data_server*)segptr)->core_data).clients[n -1].id_client, (((data_server*)segptr)->core_data).clients[n -1].name_client);
	  counts = 1;
	}
      }
      if( counts == 0){
	printf("No client online!\n");
      }
      printf("\n\n\n\nPress enter key!");
      getchar();
      break;
    }
    case '2':{
      system("clear");
      for( counts = 0;  counts < MAX_CLIENTS; counts++){
	if( (((data_server*)segptr)->status)[counts] != 0 ){
	  /* Connect to client and send warring command*/
	  // Port (((data_server*)segptr)->core_data).clients[n -1].port - 1
	  // Create socket file descriptor
	  while( (socket_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0){
	    printf("Creating socket file descriptor!\n");
	    sleep(5000);
	  }
	  
	  // Setup server_socket
	  memset( &server_socket, 0, sizeof(server_socket));
	  server_socket.sin_family = AF_INET;
	  server_socket.sin_addr.s_addr = inet_addr( (((data_server*)segptr)->ip_clients)[counts] );
	  server_socket.sin_port = htons( (((data_server*)segptr)->core_data).clients[counts -1].port -1);
	  
	  // Connect to server
	  //printf("All right! Try connect to server {|%s|} on port {|%hu|}\n", (((data_server*)segptr)->ip_clients)[counts], (((data_server*)segptr)->core_data).clients[counts -1].port -1);
	  while ( connect( socket_fd, (struct sockaddr*)&server_socket, sizeof(server_socket)) < 0){
	    printf("Connect error!\nReconnectiong to ...{|%s|} on port {|%hu|}\n", (((data_server*)segptr)->ip_clients)[counts], ((((data_server*)segptr)->core_data).clients[counts -1]).port -1);
	    usleep(500000);
	  }
	  //printf("Success! Connected to client!.\n");
	  //printf("Sedding command!\n");
	  strcpy( buffer, "Warring");
	  send( socket_fd, buffer, strlen(buffer), 0);
	  // Close connection
	  shutdown( socket_fd, 2);
	  close( socket_fd );
	}
      }
      getchar();
      break;
    }
    case '3':{
      while( 1 ){
	system("clear");
	printf("ID of client? ");
	scanf("%d", &counts);
	while( getchar() != '\n');
	if( (((data_server*)segptr)->status)[counts] == 0 ){
	  printf("Client not online!\n");
	  printf("\n\n\nPress Enter please!");
	  getchar();
	}else {
	  /* Connect to client and send get infomation command*/
	  // Port (((data_server*)segptr)->core_data).clients[n -1].port - 1
	  // Create socket file descriptor
	  while( (socket_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0){
	    printf("Creating socket file descriptor!\n");
	    sleep(5000);
	  }
	  
	  // Setup server_socket
	  memset( &server_socket, 0, sizeof(server_socket));
	  server_socket.sin_family = AF_INET;
	  server_socket.sin_addr.s_addr = inet_addr( (((data_server*)segptr)->ip_clients)[counts] );
	  server_socket.sin_port = htons( (((data_server*)segptr)->core_data).clients[counts -1].port -1);
	  
	  // Connect to server
	  //printf("All right! Try connect to server {|%s|} on port {|%hu|}\n", (((data_server*)segptr)->ip_clients)[counts], (((data_server*)segptr)->core_data).clients[counts -1].port -1);
	  while ( connect( socket_fd, (struct sockaddr*)&server_socket, sizeof(server_socket)) < 0){
	    printf("Connect error!\nReconnectiong to ...{|%s|} on port {|%hu|}\n", (((data_server*)segptr)->ip_clients)[counts], ((((data_server*)segptr)->core_data).clients[counts -1]).port -1);
	    usleep(500000);
	  }
	  //printf("Success! Connected to client!.\n");
	  //printf("Sedding command!\n");
	  strcpy( buffer, "GetInfo");
	  send( socket_fd, buffer, strlen(buffer), 0);
	  // Close connection
	  shutdown( socket_fd, 2);
	  close( socket_fd );
	  break;
	}
      }
      getchar();
      break;
    }
    }
  }

  // Waiting, when number of thread is 0 => Shutdown server.
  /*for(i = 0; i < (((data_server*)segptr)->core_data).current_number_client; i++){
    pthread_join( thread_arr[i], NULL);
    }*/

  // Remove shared memory segment.
  shmctl(shmid, IPC_RMID, 0);
  
  // Shutdown server.
  system("clear");
  printf("Server go to shutdown!\nSee you again!");
  getchar();
  system("pkill -9 server.exec");

  return 0;

}
