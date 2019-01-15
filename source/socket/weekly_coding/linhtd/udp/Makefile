CC=gcc
CFLAGS=-w -std=c99

all: client server recv send

server: udpServer.c
	$(CC) -g -o $@ $^ $(CFLAGS)
client: udpClient.c
	$(CC) -g -o $@ $^ $(CFLAGS)
recv: udp-recv.c 
	$(CC) -g -o $@ $^ $(CFLAGS)
send: udp-send.c
	$(CC) -g -o $@ $^ $(CFLAGS)



clean: *
	rm -rf server client recv send
