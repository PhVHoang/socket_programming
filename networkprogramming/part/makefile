CC=gcc
CFLAGS=-w -std=c99

all: tcpclient tcpserver sclient sserver

tcpclient: tcp_client.c
	$(CC) -o $@ $^ $(CFLAGS)

tcpserver: tcp_server.c
	$(CC) -o $@ $^ $(CFLAGS)

sclient: getfile_client.c
	$(CC) -o $@ $^ $(CFLAGS)
sserver: sendfile_server.c
	$(CC) -o $@ $^ $(CFLAGS)

clean: *
	rm -rf client server
