#include "client.h"

void config_module(){
  private_data_of_client data;
  FILE *config_f;
  char choice, confirm, name_item[100];
  int i, flag, adjust_element;
  
  // Load data
  if( NULL == (config_f = (fopen( "config.cfg", "r")))){
    printf("Config file not found!\nPlease check config file!\n");
    exit(1);
  }

  fread( &data, sizeof(private_data_of_client), 1, config_f);
  fclose( config_f );

  choice = 0;
  while( choice != 'q' ){
    system("clear");
    printf("1) List all item\n");
    printf("2) Info of item\n");
    printf("3) Config a item\n");
    printf("4) Add new item\n");
    printf("5) Delete item\n");
    printf("6) Adjust number of item\n");
    printf("7) Print all infomation\n");
    printf("q) Quit\n");
    printf("Choice: ");
    scanf("%c", &choice);
    while( getchar() != '\n');

    switch( choice ){
    case '1': {
      system("clear");
      printf("Total %d item!\n\n", data.number_item);
      for(i = 0; i < data.number_item; i++){
	printf("%d)\n\tName: %s\n", i + 1, data.list_item[i].name);
	printf("\tCost per one element: %g vnd\n", data.list_item[i].cost);
	printf("\tNumber elements in inventory: %d\n", data.list_item[i].current_number_element);
	printf("\tMax element in inventory: %d\n", data.list_item[i].max_element);
	printf("\tWarring on: %d element\n", data.list_item[i].warring_number_element);
      }
      
      printf("Press Enter to back to main menu!\n");
      getchar();
      break;
    }
    case '2': {
      flag = 1;
      while( flag ){
	system("clear");
	printf("What is name of item? ");
	gets(name_item);
	for(i = 0; i < data.number_item; i++){
	  if( !strcmp( name_item, (data.list_item[i]).name)){
	    printf("Infomation of item:\n\tName: %s\n", (data.list_item[i]).name);
	    printf("\tCost per one element: %g vnd\n", data.list_item[i].cost);
	    printf("\tNumber elements in inventory: %d\n", data.list_item[i].current_number_element);
	    printf("\tMax element in inventory: %d\n", data.list_item[i].max_element);
	    printf("\tWarring on: %d element\n\n\n", data.list_item[i].warring_number_element);

	    flag = 0;
	    break;
	  }
	}
	if( i >= data.number_item){
	  printf("Invalite item!\n");
	  flag = 0;
	}

      }
      printf("Press enter to back to main menu!");
      getchar();
      break;      
    }
    case '3': {
      flag = 1;
      while( flag ){
	system("clear");
	printf("What is name of item? ");
	gets(name_item);
	for(i = 0; i < data.number_item; i++){
	  if( !strcmp( name_item, (data.list_item[i]).name)){
	    printf("Infomation of item:\n\tName: %s\n", (data.list_item[i]).name);
	    printf("\tCost per one element: %g vnd\n", data.list_item[i].cost);
	    printf("\tNumber elements in inventory: %d\n", data.list_item[i].current_number_element);
	    printf("\tMax element in inventory: %d\n", data.list_item[i].max_element);
	    printf("\tWarring on: %d element\n\n\n", data.list_item[i].warring_number_element);
	    // Re-config
	    printf("New infomation:\n");
	    printf("\tNew name: ");
	    gets( (data.list_item[i]).name);
	    printf("\tCost per one element in vnd: ");
	    scanf("%f", &(data.list_item[i].cost));
	    while( getchar() != '\n');
	    printf("\tNumber elements in inventory: ");
	    scanf("%d", &(data.list_item[i].current_number_element));
	    while( getchar() != '\n');
	    printf("\tMax element in inventory: ");
	    scanf("%d", &(data.list_item[i].max_element));
	    while( getchar() != '\n');	    
	    printf("\tWarring on: ");
	    scanf("%d", &(data.list_item[i].warring_number_element));
	    while( getchar() != '\n');
	    // Write back to config file
	    if( NULL == (config_f = fopen("config.cfg", "w"))){
	      printf("Has some error!!!\nPlease close program and retry!\n\n");
	    } else {
	      fwrite( &data, sizeof(private_data_of_client), 1, config_f);		
	      printf("Done! Changed a item!\n");
	      fclose(config_f);
	    }
	    flag = 0;
	    break;
	  }
	}
	if( i >= data.number_item){
	  printf("Invalite item!\n");
	  flag = 0;
	}
      }
      printf("Press enter to back to main menu!");
      getchar();
      break;
    }
    case '4': {
      printf("What is name of item? ");
      gets(name_item);
      flag = 1;
      for(i = 0; i < data.number_item; i++){
	if( !strcmp( name_item, (data.list_item[i]).name)){
	  printf("Dupplicate item!!!\n");
	  flag = 0;
	  break;
	}
      }
      if( (i >= data.number_item) && (flag == 1) ){
	// Input infomation
	printf("Infomation:\n");
	strcpy( (data.list_item[i]).name, name_item);
	printf("\tName: %s\n", (data.list_item[i]).name);
	printf("\tCost per one element in vnd: ");
	scanf("%f", &(data.list_item[i].cost));
	while( getchar() != '\n');	     
	printf("\tMax element in inventory: ");
	scanf("%d", &(data.list_item[i].max_element));
	while( getchar() != '\n');	    
	printf("\tWarring on: ");
	scanf("%d", &(data.list_item[i].warring_number_element));
	while( getchar() != '\n');	
	(data.list_item[i].current_number_element) = 0;
	(data.number_item) += 1;
	// Write back to config file
	if( NULL == (config_f = fopen("config.cfg", "w"))){
	  printf("Has some error!!!\nPlease close program and retry!\n\n");
	} else {
	  fwrite( &data, sizeof(private_data_of_client), 1, config_f);		
	  printf("Done! Add new a item!\n");
	  fclose(config_f);
	}
      } else {
	printf("Invalite item!!\n");
      }
      printf("Press enter to back to main menu!");
      getchar();
      break;
    }
    case '5': {
      flag = 1;
      while( flag ){
	system("clear");
	printf("What is name of item? ");
	gets(name_item);
	for(i = 0; i < data.number_item; i++){
	  if( !strcmp( name_item, (data.list_item[i]).name)){
	    printf("Infomation of item:\n\tName: %s\n", (data.list_item[i]).name);
	    printf("\tCost per one element: %g vnd\n", data.list_item[i].cost);
	    printf("\tNumber elements in inventory: %d\n", data.list_item[i].current_number_element);
	    printf("\tMax element in inventory: %d\n", data.list_item[i].max_element);
	    printf("\tWarring on: %d element\n\n\n", data.list_item[i].warring_number_element);
	    confirm = 0;
	    while( (confirm != 'y') && (confirm != 'n') ){
	      printf("Do you want to remove this item (y/n)?: ");
	      scanf("%c", &confirm);
	      while( getchar() != '\n');
	    }
	    if( confirm == 'y'){
	      // Delete item
	      for(; i < (data.number_item - 1); i++){
		strcpy( (data.list_item[i]).name, data.list_item[i + 1].name);
		(data.list_item[i]).cost = (data.list_item[i + 1]).cost;
		(data.list_item[i]).max_element = (data.list_item[i + 1]).max_element;
		(data.list_item[i]).current_number_element = (data.list_item[i + 1]).current_number_element;
		(data.list_item[i]).warring_number_element = (data.list_item[i + 1]).warring_number_element;
	      }
	      // Write back to config file
	      if( NULL == (config_f = fopen("config.cfg", "w"))){
		printf("Has some error!!!\nPlease close program and retry!\n\n");
	      } else {
		data.number_item -= 1;
		fwrite( &data, sizeof(private_data_of_client), 1, config_f);		
		fclose(config_f);
		printf("Done! Removed a item!\n");
	      }
	    } else {
	      printf("Oke! Cancel action delete a item!\n");
	    }
	    flag = 0;
	    break;
	  }
	}
	if( i >= data.number_item){
	  if( flag ){
	    printf("Invalite item!\n");
	  }
	}

      }
      printf("Press enter to back to main menu!");
      getchar();
      break;
    }
    case '6':{
      flag = 1;
      while( flag ){
	system("clear");
	printf("What is name of item? ");
	gets(name_item);
	for(i = 0; i < data.number_item; i++){
	  if( !strcmp( name_item, (data.list_item[i]).name)){
	    printf("Infomation of item:\n\tName: %s\n", (data.list_item[i]).name);
	    printf("\tCost per one element: %g vnd\n", data.list_item[i].cost);
	    printf("\tNumber elements in inventory: %d\n", data.list_item[i].current_number_element);
	    printf("\tMax element in inventory: %d\n", data.list_item[i].max_element);
	    printf("\tWarring on: %d element\n\n\n", data.list_item[i].warring_number_element);
	    adjust_element = -1;
	    while( (adjust_element < 0) || (adjust_element > data.list_item[i].max_element) ){
	    printf("How many element of item?: ");
	    scanf("%d", &adjust_element);
	    while( getchar() != '\n');
	    }
	    data.list_item[i].current_number_element = adjust_element;
	    // Write back to config file
	    if( NULL == (config_f = fopen("config.cfg", "w"))){
	      printf("Has some error!!!\nPlease close program and retry!\n\n");
	    } else {
	      fwrite( &data, sizeof(private_data_of_client), 1, config_f);		
	      fclose(config_f);
	      printf("Done! Adjust item!\n");
	    }
	    flag = 0;
	    break;
	  }
	}
	if( i >= data.number_item){
	  if( flag ){
	    printf("Invalite item!\n");
	  }
	}

      }
      printf("Press enter to back to main menu!");
      getchar();      
      break;
    }
    case '7': {
      system("clear");
      printf("ID client: %d\n", data.id_client);
      printf("Name client: %s\n", data.name_client);
      printf("Server IP: %s\n", data.server_address_ip);
      printf("UpPort: %hu\n", data.port);
      printf("DownPort: %hu\n", data.port - 1);
      printf("Number item: %d\n", data.number_item);
      for(i = 0; i < data.number_item; i++){
	printf("%d)\n\tName: %s\n", i + 1, data.list_item[i].name);
	printf("\tCost per one element: %g vnd\n", data.list_item[i].cost);
	printf("\tNumber elements in inventory: %d\n", data.list_item[i].current_number_element);
	printf("\tMax element in inventory: %d\n", data.list_item[i].max_element);
	printf("\tWarring on: %d element\n", data.list_item[i].warring_number_element);
      }
      
      printf("Press Enter to back to main menu!\n");
      getchar();
      break;
    }      
    case 'q': {
      system("clear");
      printf("Bye! See you again!\n\n\n\n");
      break;
    }      
    }
  }

  return;
}
