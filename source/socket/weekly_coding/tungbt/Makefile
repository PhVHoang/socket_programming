CC=gcc
CFLAGS=-w -std=c99

all: client server

client: tcp_client.c 
	$(CC) -g -o $@ $^ $(CFLAGS)

server: tcp_server.c
	$(CC) -g -o $@ $^ $(CFLAGS)



clean: *
	rm -rf client server
