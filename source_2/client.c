#include "client.h"

int main(int argc, char *argv[]){
  //int socket_fd;
  struct sockaddr_in server_socket;
 
  FILE *config_f;
  private_data_of_client data;

  pthread_t down_thread;

  arg_struct arg_struct_p;
  char minicommand[100];

  // What is mode?
  if( argc != 2){
    system("clear");
    printf("Usage: client <mode>\nMode: \n\t1) sale\n\t\tThis mode is saling.\n\t2) config\n\t\tThis mode is config of machine.\n");
    printf("\n\n\n\n");
    exit(1);
  }

  if( (strcmp(argv[1], "sale") != 0) && ( strcmp(argv[1], "config") != 0)){
    system("clear");
    printf("Usage: client <mode>\nMode:\n\t1) sale\n\t\tThis mode is saling.\n\t2) config\n\t\tThis mode is config of machine.\n");
    printf("\n\n\n\n");
    exit(1); 
  }

  if( !strcmp(argv[1], "config")){
    config_module();
  }
  if( strcmp(argv[1], "sale")){
    return 0;
  }
  
  // Load data
  if( NULL == (config_f = (fopen( "config.cfg", "r")))){
    printf("Config file not found!\nPlease check config file!\n");
    exit(1);
  }

  fread( &(arg_struct_p.data), sizeof(private_data_of_client), 1, config_f);
  fclose( config_f );

   // Create mutex
  if( pthread_mutex_init( &(arg_struct_p.mutex_obj), NULL) != 0){
    printf("Mutex init failed!\n");
    return 1;
  } 
  // Create thread down
  printf("Creating new thread down.\n");
  while( pthread_create( &(down_thread), NULL , thread_down_handler, (void*) &arg_struct_p) < 0){
    printf("\tCreating thread down\n");
  }

  // Create socket file descriptor
  while( ( arg_struct_p.up_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0){
    printf("Creating socket file descriptor!\n");
    sleep(5000);
  }

  // Setup server_socket
  memset( &server_socket, 0, sizeof(server_socket));
  server_socket.sin_family = AF_INET;
  server_socket.sin_addr.s_addr = inet_addr( (arg_struct_p.data).server_address_ip );
  server_socket.sin_port = htons( (arg_struct_p.data).port );

  // Connect to server
  printf("All right! Try connect to server {|%s|} on port {|%hu|}\n", (arg_struct_p.data).server_address_ip, (arg_struct_p.data).port);
  while ( connect( arg_struct_p.up_fd, (struct sockaddr*)&server_socket, sizeof(server_socket)) < 0){
    printf("Connect error!\nReconnectiong to ...{|%s|} on port {|%hu|}\n", (arg_struct_p.data).server_address_ip, (arg_struct_p.data).port);
    usleep(500000);
  }
  printf("Success! Connected ! I'm online.\n");

  sale(arg_struct_p.up_fd, &(arg_struct_p));
  
  shutdown( arg_struct_p.up_fd, 2);
  close( arg_struct_p.up_fd );
  //system("pkill -9 client.exec");
  // Kill all process using port
  // kill -9 `lsof -i:10010 -t`
  memset(minicommand, 0, 100);
  sprintf( minicommand, "kill -9 `lsof -i:%hu -t` 2> /dev/null", (arg_struct_p.data).port -1);
  //puts(minicommand);
  system(minicommand);

  return 0;
}
