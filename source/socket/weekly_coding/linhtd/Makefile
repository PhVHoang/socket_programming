CC=gcc
CFLAGS=-w -std=c99

all: client server

client: echoClient.c 
	$(CC) -g -o $@ $^ $(CFLAGS)

server: echoServer.c
	$(CC) -g -o $@ $^ $(CFLAGS)



clean: *
	rm -rf client server
