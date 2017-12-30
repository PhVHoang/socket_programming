#include "server_receive.h"
#include "server_send.h"

void sig_chld(int);

int main() {
    return 0;
}

void sig_chld(int signo) {
    pid_t pid;
	int stat;
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	/* for debugging only, i/o not recommended here */
	return;
}