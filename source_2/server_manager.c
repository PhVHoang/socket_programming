#ifndef _CLIENT_H_
#include "client.h"
#endif

#ifndef _SERVER_H_
#include "server.h"
#endif

void find_infomation_client_by_id( private_server_data *data);
void config_client( private_server_data *data);
void create_new_client( private_server_data *data);
void export_config_of_client(private_server_data *data);
void showall_data( private_server_data *data);

int main(int argc, char *argv[]){
  char choice;
  FILE *config_f;
  private_server_data data_server;

  // Normal mode
  if( argc == 1){
    if( NULL == (config_f = fopen("config_server.data", "r"))){
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

    if( NULL == (config_f = fopen("config_server.data", "w"))){
      printf("Can't create file!!\n\n");
      exit(1);
    }

    data_server.current_number_client = 0;
    memset( &(data_server.clients[0]), 0, sizeof(client_define) * MAX_CLIENTS);

    fwrite( &data_server, sizeof(private_server_data), 1, config_f);
    system("rm -rf history.log");
    system("rm -rf ./client[0-9] ./client[0-9][0-9] ./client[0-9][0-9][0-9]");
    system("clear");
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
    printf("4) Export config file for client\n");
    printf("5) Show data server!\n");
    printf("q) Quit.\n");
    printf("\n\nChoice: ");

    scanf("%c", &choice);
    while( getchar() != '\n');

    switch(choice){
    case '1':{
      find_infomation_client_by_id( &data_server);      
      break;
    }
    case '2':{
      config_client( &data_server);	
      break;
    }
    case '3':{
      create_new_client( &data_server);
      break;
    }
    case '4':{
      export_config_of_client( &data_server);
      break;
    }      
    case '5':{
      showall_data( &data_server);
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

void find_infomation_client_by_id( private_server_data *data){
  int fid;

  system("clear");
  printf("What is id of client? ");
  scanf("%d", &fid);
  while( getchar() != '\n');
  if( fid > (data->current_number_client) ){
    printf("Invalid this client!\n");
    printf("Press Enter to back to main menu!\n");
    getchar();
    return;
  }

  system("clear");
  printf("Infomation of client:\n\tID client: %d\n\tClient name: %s\n\tPort: %hu\n\n\nPress Enter to back to main menu!\n", (data->clients[fid - 1]).id_client, (data->clients[fid - 1]).name_client, (data->clients[fid - 1]).port);

  getchar();
  return;
}
void config_client( private_server_data *data){
  FILE *config_f;
  uint16_t port;
  int i, fid, flag;

  
  system("clear");
  printf("What is id of client? ");
  scanf("%d", &fid);
  while( getchar() != '\n');
  if( fid > (data->current_number_client) ){
    printf("Invalid this client!\n");
    printf("Press Enter to back to main menu!\n");
    getchar();
    return;
  }

  printf("Enter new name of client: ");
  gets( (data->clients[fid -1]).name_client);
  printf("Please input port of client (example %hu): ", (data->clients[ data->current_number_client -1]).port + 2);
  flag = 1;
  while( flag ){
    scanf("%hu", &port);
    while( getchar() != '\n');
    if( (port < 10000) || ( (port%2) != 0)){
      printf("Invalid port!!\n");
      printf("Please input port of client (example %hu): ", (data->clients[ data->current_number_client -1]).port + 2);
      continue;
    }
    for(i = 0; i < (data->current_number_client); i++){
      if( port == (data->clients[ i ]).port){
	printf("Duplicate port!\n");
	printf("Please input port of client (example %hu): ", (data->clients[ data->current_number_client -1 ]).port + 2);
	break;
      }
    }
    if( i == (data->current_number_client)){
      flag = 0;
    }
  }
  (data->clients[fid -1]).port = port;
  if( NULL == (config_f = fopen("config_server.data", "w"))){
    system("clear");
    printf("Warring!!!!\nSystem was hack?\nServer data file not found!\n\n");
    exit(1);
  }

  fwrite(data, sizeof(private_server_data), 1, config_f );
  
  fclose(config_f);

  system("clear");
  printf("Created new client:\n\tID client: %d\n\tClient name: %s\n\tPort: %hu\n\n\nPress Enter to back to main menu!\n", (data->clients[fid - 1]).id_client, (data->clients[fid - 1]).name_client, (data->clients[fid - 1]).port);

  getchar();
  return;
}
void create_new_client( private_server_data *data){
  FILE *config_f;
  uint16_t port;
  int i, flag;

  
  system("clear");
  printf("What is name of new client? ");
  gets( (data->clients[data->current_number_client]).name_client);
  printf("Please input port of client (example %hu): ", (data->clients[ data->current_number_client -1]).port + 2);
  flag = 1;
  while( flag ){
    scanf("%hu", &port);
    while( getchar() != '\n');
    if( (port < 10000) || ( (port%2) != 0)){
      printf("Invalid port!!\n");
      printf("Please input port of client (example %hu): ", (data->clients[ data->current_number_client -1]).port + 2);
      continue;
    }
    for(i = 0; i < (data->current_number_client); i++){
      if( port == (data->clients[i]).port){
	printf("Duplicate port!\n");
	printf("Please input port of client (example %hu): ", (data->clients[ data->current_number_client -1]).port + 2);
	break;
      }
    }
    if( i == (data->current_number_client)){
      flag = 0;
    }
  }
  (data->clients[data->current_number_client]).port = port;
  //scanf("%hu", &((data->clients[data->current_number_client]).port));
  (data->clients[data->current_number_client]).id_client = (data->current_number_client++) + 1;
  if( NULL == (config_f = fopen("config_server.data", "w"))){
    system("clear");
    printf("Warrning!!!!\nSystem was hack?\nServer data file not found!\n\n");
    exit(1);
  }

  fwrite(data, sizeof(private_server_data), 1, config_f );
  
  fclose(config_f);

  system("clear");
  printf("Created new client:\n\tID client: %d\n\tClient name: %s\n\tPort: %hu\n\n\nPress Enter to back to main menu!\n", (data->clients[data->current_number_client - 1]).id_client, (data->clients[data->current_number_client - 1]).name_client, (data->clients[data->current_number_client - 1]).port);

  getchar();
  return;
}
void export_config_of_client(private_server_data *data){
  int i;
  private_data_of_client client_data;
  FILE *export_f;
  char mini_buffer[100];
  
  system("clear");
  i = (data->current_number_client) + 1;
  while( i > (data->current_number_client)){
  printf("ID of client want to export config file: ");
  scanf("%d", &i);
  while( getchar() != '\n');
  }

  client_data.port = (data->clients[i-1]).port;
  client_data.id_client = i;
  strcpy( client_data.server_address_ip, "127.0.0.1");
  strcpy( client_data.name_client, (data->clients[i -1]).name_client);

  client_data.number_item = 0;
  for( i = 0; i < MAX_ITEM_ON_INVENTORY; i++){
    memset( (client_data.list_item[i]).name, 0, 100);
    (client_data.list_item[i]).cost = 0;
    (client_data.list_item[i]).max_element = 0;
    (client_data.list_item[i]).current_number_element = 0;
    (client_data.list_item[i]).warring_number_element = 0;

  }
  printf("Export infomations:\n\tName client: %s\n\tID client: %d\n\tServer infomation: %s\n\tPort: %hu\n\tNumber item: %d\n", client_data.name_client, client_data.id_client, client_data.server_address_ip, client_data.port, client_data.number_item);

  memset( mini_buffer, 0, 100);
  sprintf( mini_buffer, "mkdir client%d", client_data.id_client);
  system(mini_buffer);
  memset( mini_buffer, 0, 100);
  sprintf( mini_buffer, "client%d/config.cfg", client_data.id_client);
  if( NULL == (export_f = fopen( mini_buffer, "w"))){
    system("clear");
    printf("Warring!!!!\nSystem was hack?\nCan't create new file!\n\n");
    exit(1);
  }

  fwrite( &client_data, sizeof(private_data_of_client), 1, export_f );
  
  fclose(export_f);
  
  memset( mini_buffer, 0, 100);
  sprintf( mini_buffer, "cp client.exec client%d", client_data.id_client);
  system(mini_buffer);
  
  printf("\n\nPress Enter to back to main menu!\n");  
  getchar();
  return;
}
void showall_data( private_server_data *data){
  int i;
  
  system("clear");
  printf("Total clients: %d\n", data->current_number_client);

  if( data->current_number_client ){
    for( i = 0; i < data->current_number_client; i++){
      printf("%d)\n\tID client: %d\n\tName client:%s\n\tPort connect: %hu\n", i+ 1, data->clients[i].id_client, data->clients[i].name_client, data->clients[i].port);
    }
  }
  
  printf("\n\nPress Enter to back to main menu!\n");
  getchar();  
  return;
}
