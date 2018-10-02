CC= gcc
FLAGS= -g -lpthread

project: server.exec client.exec server_manager.exec clean


server.exec: server.o service_client.o thread_handler.o
	$(CC) $(FLAGS) -pthread -o server.exec server.o service_client.o thread_handler.o
client.exec: client.o sale.o config_module.o thread_down_handler.o service_down.o
	$(CC) $(FLAGS) -pthread -o client.exec client.o sale.o config_module.o thread_down_handler.o service_down.o
server_manager.exec: server_manager.o
	$(CC) $(FLAGS) -o server_manager.exec server_manager.o


server_manager.o: server_manager.c client.h
	$(CC) $(FLAGS) -c server_manager.c
server.o: server.c server.h
	$(CC) $(FLAGS) -c server.c
thread_handler.o: thread_handler.c server.h
	$(CC) $(FLAGS) -c thread_handler.c
service_client.o: service_client.c server.h
	$(CC) $(FLAGS) -c service_client.c
client.o: client.c client.h
	$(CC) $(FLAGS) -c client.c
service_down.o: service_down.c client.h
	$(CC) $(FLAGS) -c service_down.c	
thread_down_handler.o: thread_down_handler.o client.h
	$(CC) $(FLAGS) -c thread_down_handler.c
config_module.o: config_module.c client.h
	$(CC) $(FLAGS) -c config_module.c
sale.o: sale.c client.h
	$(CC) $(FLAGS) -c sale.c

reset_project: clean cleanexec
	./reset_project.sh && ./myls.sh
clean:
	./rm_all_obj_and_oldversion_file.sh && ./myls.sh
cleanexec:
	./rm_all_exec_file.sh && ./myls.sh
