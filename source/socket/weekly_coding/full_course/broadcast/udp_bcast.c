#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include	<signal.h>

#define SERV_PORT 5500
#define BROADCAST_ADDR "255.255.255.255"
#define BUFF_SIZE 1024

static void	recvfrom_alarm(int);

int main()
{
	int	sockfd, n;
	struct sockaddr_in	servaddr, *preply_addr;
	const int		on = 1;
	char			sendbuff[BUFF_SIZE], recvbuff[BUFF_SIZE + 1];
	socklen_t		len, servlen;
	
	if(sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("\n create socket error: ");
		return 0;
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, BROADCAST_ADDR, &servaddr.sin_addr);

	servlen = sizeof(servaddr);
	preply_addr = malloc(servlen);
	
	if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on))){
		perror("\n set broadcast error: ");
		return 0;
	}
		
	signal(SIGALRM, recvfrom_alarm);
	while(1){
		printf("\nInsert string to send:");
		fgets(sendbuff, BUFF_SIZE, stdin);		
		msg_len = strlen(sendbuff);
		if (msg_len == 0) break;
		
		if(sendto(sockfd, sendbuff, msg_len, 0, (struct sockaddr *)&servaddr, servlen) < 0){
			perror("\n send error: ");
			return 0;
		}
		
		alarm(5); //call recvfrom in a loop and print all the replies received within 5 seconds
		while (1) {
			len = servlen;
			n = recvfrom(sockfd, recvbuff, BUFF_SIZE, 0, (struct sockaddr *)preply_addr, &len);
			if (n < 0) {
				//  after 5 seconds SIGALRM is generated, our signal handler is called, and recvfrom returns the error EINTR
				if (errno == EINTR) 
					break;		/* waited long enough for replies */
				else
					perror("recvfrom error");
			} else {
				recvline[n] = 0;	/* null terminate */
				printf("from %s: %s", recvline);
			}
		}
	}
	close(sockfd);
	return 0;
}

static void recvfrom_alarm(int signo)
{
	return;		/* just interrupt the recvfrom() */
}