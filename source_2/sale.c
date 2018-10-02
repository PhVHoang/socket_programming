#include "client.h"

void sale(int sock, arg_struct *arg){
  char i;
  int choice, number_items, leng_name_item;
  float money;
  char buffer[MAX_BUFFER_SIZE], menu_buffer[MAX_MENU_BUFFER_SIZE], money_buffer[MAX_MONEY_BUFFER_SIZE];
  int leng;
  FILE *config_f;
  private_data_of_client *data;

  data = &(arg->data);
  choice = 0;
  
  while( choice != (data->number_item + 1)){
    system("clear");
    // List item
    for(i = 0; i < data->number_item; i++){
      memset(menu_buffer, ' ', MAX_MENU_BUFFER_SIZE);
      sprintf( menu_buffer, "%d) %s", i+ 1, ((data->list_item)[i]).name);
      leng_name_item = strlen(menu_buffer);
      menu_buffer[leng_name_item] = ' ';
      menu_buffer[MAX_NAME_ITEM_MENU -1] = 0;
      //memset( &(menu_buffer[MAX_NAME_ITEM_MENU]), '\0', MAX_MENU_BUFFER_SIZE - MAX_NAME_ITEM_MENU);
      memset( money_buffer, 0, MAX_MONEY_BUFFER_SIZE);
      sprintf( money_buffer, "%g vnd", ((data->list_item)[i]).cost);
      //printf("%d) %s \t %g vnd\n", i+ 1, ((data->list_item)[i]).name, ((data->list_item)[i]).cost);
      strcat(menu_buffer, money_buffer);
      puts(menu_buffer);
    }
    printf("%d) Quit\n", i + 1);
    printf("Choice item: ");

    scanf("%d", &choice);
    while( getchar() != '\n');

    if( (choice < 0) || (choice > (data->number_item + 1)) ){
      printf("Lol");
      continue;
    } else {
      if(choice == (data->number_item + 1)){
	send( sock, "quit", strlen("quit"), 0);
	printf("Bye! See you again!\n\n\n\n\n");
	return;
      } else {
	// Lock mutex
	while( pthread_mutex_lock( &(arg->mutex_obj) ) != 0){
	  sleep(1);
	}
	while(1){
	  printf("Number element you want? ");
	  scanf("%d", &number_items);
	  while( getchar() != '\n');
	  
	  // Check current number of eletement in inventory
	  if( ((data->list_item[choice -1]).current_number_element) < number_items){
	    printf("Sorry! :( Not enought!!\nIn inventory has %d element!\n\n", ((data->list_item[choice -1]).current_number_element));
	    continue;
	  } else {
	    printf("Total is: %g * %d = %g vnd\n", ((data->list_item)[choice -1]).cost, number_items, ((data->list_item)[choice -1]).cost * number_items);
	    while(1){
	      printf("Insert money: ");
	      scanf("%f", &money);
	      while( getchar() != '\n');
	      if( money < ( ((data->list_item)[choice -1]).cost * number_items )){
		printf("Not enought money!!\n");
	      } else {
		break;
	      }
	    }
	    if( money > ( ((data->list_item)[choice -1]).cost * number_items ) ){
	      printf("Reject you: %g vnd\n", money - ( ((data->list_item)[choice -1]).cost * number_items ));
	    }
	    // Write back data;
	    if( NULL == (config_f = (fopen( "config.cfg", "w")))){
	      printf("Config file not found!\nPlease check config file!\n");
	      exit(1);
	    }
	    
	    ((data->list_item[choice -1]).current_number_element) = ((data->list_item[choice -1]).current_number_element) - number_items;	    
	    fwrite( data, sizeof(private_data_of_client), 1, config_f);
	    fclose( config_f );	    
	    // Log
	    strcpy( buffer, ((data->list_item)[choice -1]).name);
	    strcat( buffer, " x ");
	    leng = strlen(buffer);
	    memset( &(buffer[leng]), 0, 10);
	    sprintf( &(buffer[leng]), "%d", number_items);
	    //send( sock, ((data->list_item)[choice -1]).name, strlen(((data->list_item)[choice -1]).name), 0);
	    send( sock, buffer, strlen(buffer), 0);
	    printf("Done! Here is your %d %s\nPlease press Enter!", number_items, ((data->list_item)[choice -1]).name);
	    getchar();
	    break;
	  }
	}
	// Unlock mutex
	while( pthread_mutex_unlock( &(arg->mutex_obj) ) != 0){
	  sleep(1);
	}
      }
    }
    
  }

  return;
}
