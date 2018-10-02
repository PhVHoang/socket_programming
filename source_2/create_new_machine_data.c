#ifndef _CLIENT_H_
#include "client.h"
#endif

#ifndef _SERVER_H_
#include "server.h"
#endif

void find_infomation_client_by_id();
void config_client();
void create_new_client();
void showall_data();

int main(int argc, char *argv[]){
  char choice;
  FILE *config_f;
  private_server_data data_server;

  // Normal mode
  if( argc == 1){
    if( NULL == (config_f = fopen("config_server.dat", "r"))){
      system("clear");
      printf("Warring!!!!\nSystem was hack?\nServer data file not found!\n\n");
      exit(1); 
    }
  }

  // Init mode?
  if( argc == 2){
    system("clear");    
    if( strcmp(argv[1], "init") != 0){
      printf("Usage: create_new_machine_data \"init\"\n\n");
    }

    if( NULL == (config_f = fopen("config_server.dat", "w"))){
      printf("Can't create file!!\n\n");
      exit(1);
    }

    data_server.current_number_client = 0;
    memset( &(data_server.clients[0]), 0, sizeof(client_define) * MAX_CLIENTS);

    fwrite( &data_server, sizeof(private_server_data), 1, config_f);
    printf("Created empty server data!\n\n");
    fclose(config_f);
    return 0;
  }


  // Load data
  fread( &data_server, sizeof(private_server_data), 1, config_f);
  fclose(config_f);

  // Menu
  choice = 'a';
  while( choice != 'q'){
    system("clear");
    printf("1) Search client by id.\n");
    printf("2) Change client config.\n");
    printf("3) Create new client.\n");
    printf("4) Show data server!\n");
    printf("q) Quit.\n");
    printf("\n\nChoice: ");

    scanf("%c", &choice);
    while( getchar() != '\n');

    switch(choice){
    case '1':{
      find_infomation_client_by_id();      
      break;
    }
    case '2':{
      config_client();	
      break;
    }
    case '3':{
      create_new_client();
      break;
    }
    case '4':{
      showall_data();
      break;
    }
    case 'q':{
      system("clear");
      printf("See you again!\n\n");
      return 0;
      break;
    }
    }
    
  }
  return 0;
}

void find_infomation_client_by_id(){

  return;
}
void config_client(){

  return;
}
void create_new_client(){

  return;
}
void showall_data(){

  return;
}
