//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(serv_port);
	server_addr.sin_addr.s_addr = inet_addr(serv_ip);
	
	//Step 3: Request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}
		
	//Step 4: Communicate with server			
	while(1){
		bytes_transfered = 0;
		// input file link
		memset(buff,'\0', BUFF_SIZE);
		fgets(buff, BUFF_SIZE, stdin);
		buff[strlen(buff) - 1] = '\0';
		if (strlen(buff) == 0) break;
		strcpy(filelink, buff);

		if((fp = fopen(filelink, "rb")) == NULL){
			printf("Error: File not found\n");
			continue;
		}else{
			// Step 5: Send filename to server
			// extract filename from link
			strcpy(filename, rindex(filelink, '/') + 1);
			// send filename to server
			if(send_msg(client_sock, filename, strlen(filename)) == -1){
				continue;
			}
			// Step 6: Check error message from server
			// receive error message from the server
			data = recv_msg(client_sock);
			errnum = atoi(data);
			if(errnum == 1){		// if file is existent on server
				printf("Error: File is existent on server\n");
			}else if(errnum == 0){		// if there is no error
				while(errnum == 0){		// until there is an error, keep reading from file
					memset(buff,'\0', BUFF_SIZE);
					if(fread(buff, BUFF_SIZE, 1, fp) == 1){
						// send the block to server
						if(send_msg(client_sock, buff, sizeof(buff)) == -1){
							break;
						}
						printf("Transfered: %.2lf MB\n", (bytes_transfered += sizeof(buff)) / (1024*1024));
					}else{
						// tell server that EOF has been reached
						if(send_msg(client_sock, buff, sizeof(buff)) == -1){
							break;
						}
						printf("Transfered: %.2lf MB\n", (bytes_transfered += sizeof(buff)) / (1024*1024));
						
						// check if there is any error
						data = recv_msg(client_sock);
						errnum = atoi(data);

						if(send_eof_msg(client_sock) == -1){
							break;
						}
					}
					// check if there is any error
					data = recv_msg(client_sock);
					errnum = atoi(data);
				}
				if(errnum == -1){
					printf("\nSuccessful transfering\n");
				}else{
					printf("\nError: File transfering is interupted\n");
				}
			}
		}
		fclose(fp);
	}
	
	//Step 4: Close socket
	close(client_sock);