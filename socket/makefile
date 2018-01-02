CC=gcc
CFLAGS=-w -std=c99

all: client server

client: client.c common.c
	$(CC) -g -o $@ $^ $(CFLAGS)

server: server.c common.c
	$(CC) -g -o $@ $^ $(CFLAGS)



clean: *
	rm -rf client server
