#define the C compiler to use
CC = gcc

# define any conpile-time flags
CFLAGS = -g -Wall

#define the C source files
SRCS = tcp_server.c

#define the C object files
OBJS = $(SRCS:.c=.o)

#define the executable files
MAIN = server

all: $(MAIN)
	@echo server has been compiled

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(MAIN)