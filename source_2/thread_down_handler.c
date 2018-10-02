#include "client.h"

void *thread_down_handler(void* arg_struct_parameter){
  arg_struct *arg_struct_p;
  int server_socket_ds;                    
  int client_socket_ds;

  struct sockaddr_in serverAddr; 
  struct sockaddr_in clientAddr; 
  //unsigned short servPort;     
  unsigned int clientLen;        
  char sendBuffer[MAX_BUFFER_SIZE];

  int i;

  pthread_t processID;
  FILE *f_log;

  arg_struct_p= arg_struct_parameter;

  if ((server_socket_ds = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    printf("socket() failed!\n");
    return;
  }
  memset(&serverAddr, 0, sizeof(serverAddr));   
  serverAddr.sin_family = AF_INET;               
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
  serverAddr.sin_port = htons( (arg_struct_p->data).port -1);

  while (bind(server_socket_ds, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0){
    printf("Binding socket on to port: %hu\n", (arg_struct_p->data).port -1);
  }
  
  if (listen(server_socket_ds, MAX_QUEUE_CONNECTION) < 0){
    printf("Server cause a problem when listening!\n");
    return;
  }

  //printf("Watting command on port %hu!\n", (arg_struct_p->data).port -1);
  
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
	//close(server_socket_ds);   /* Child closes parent socket */	
	service_down(client_socket_ds, arg_struct_p);
      }
    }

  }

  return;  
}
