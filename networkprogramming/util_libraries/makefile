CC=gcc
CFLAGS=-w -std=c99

all: client server

client: client.c client_upload.c client_download.c
	$(CC) -g -o $@ $^ $(CFLAGS)

server: server.c server_receive.c server_send.c common.c
	$(CC) -g -o $@ $^ $(CFLAGS)



clean: *
	rm -rf client server
