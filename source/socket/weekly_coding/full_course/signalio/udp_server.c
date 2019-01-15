#include <stdio.h>          
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

#define	QSIZE	   8		/* size of input queue */
#define	MAXDG	4096		/* max datagram size */
#define SERV_PORT 5500

typedef struct {
  void		*dg_data;		/* ptr to actual datagram */
  size_t	dg_len;			/* length of datagram */
  struct sockaddr  *dg_sa;	/* ptr to sockaddr{} w/client's address */
  socklen_t	dg_salen;		/* length of sockaddr{} */
} DG;
static DG	dg[QSIZE];			/* queue of datagrams to process */

static int	iget;		/* next one for main loop to process */
static int	iput;		/* next one for signal handler to read into */
static int	nqueue;		/* # on queue for main loop to process */
static socklen_t clilen;/* max length of sockaddr{} */
static int		sockfd;

static void	sig_io(int);

int main()
{
	int			i;
	const int	on = 1;
	sigset_t	zeromask, newmask, oldmask;
	
	struct sockaddr_in	servaddr, cliaddr;
	
	if ((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) == -1 ){  /* calls socket() */
		perror("socket() error\n");
		return 0;
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);
	
	if(bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr))==-1){ /* calls bind() */
		perror("bind() error\n");
		return 0;
	}
		
	clilen = sizeof(cliaddr);
	
	for (i = 0; i < QSIZE; i++) {	/* init queue of buffers */
		dg[i].dg_data = malloc(MAXDG);
		dg[i].dg_sa = malloc(clilen);
		dg[i].dg_salen = clilen;
	}
	iget = iput = nqueue = 0;
	
	/* Signal handlers are established for SIGIO. The socket owner is
	 * set using fcntl and the signal-driven and non-blocking I/O flags are set using ioctl
	 */
	signal(SIGIO, sig_io);
	fcntl(sockfd, F_SETOWN, getpid());
	ioctl(sockfd, FIOASYNC, &on);
	ioctl(sockfd, FIONBIO, &on);
	
	/* Three signal sets are initialized: zeromask (which never changes),
	 * oldmask (which contains the old signal mask when we block SIGIO), and newmask.
	 */
	sigemptyset(&zeromask);		
	sigemptyset(&oldmask);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGIO);	/* signal we want to block */
	
	/* Stores the current signal mask of the process in oldmask and then
	 * logically ORs newmask into the current signal mask. This blocks SIGIO
	 * and returns the current signal mask. We need SIGIO blocked when we test
	 * nqueue at the top of the loop
	 */
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	
	for ( ; ; ) {
		while (nqueue == 0)
			sigsuspend(&zeromask);	/* wait for datagram to process */

		/* unblock SIGIO by calling sigprocmask to set the signal mask of
		 * the process to the value that was saved earlier (oldmask).
		 * The reply is then sent by sendto.
		 */
		sigprocmask(SIG_SETMASK, &oldmask, NULL);

		sendto(sockfd, dg[iget].dg_data, dg[iget].dg_len, 0,
			   dg[iget].dg_sa, dg[iget].dg_salen);

		if (++iget >= QSIZE)
			iget = 0;

		/* SIGIO is blocked and the value of nqueue is decremented.
		 * We must block the signal while modifying this variable since
		 * it is shared between the main loop and the signal handler.
		 */
		sigprocmask(SIG_BLOCK, &newmask, &oldmask);
		nqueue--;
	}
}

static void sig_io(int signo)
{
	ssize_t		len;
	DG			*ptr;

	for (; ; ) {
		if (nqueue >= QSIZE){
			perror("receive overflow");
			break;
		}

		ptr = &dg[iput];
		ptr->dg_salen = clilen;
		len = recvfrom(sockfd, ptr->dg_data, MAXDG, 0,
					   ptr->dg_sa, &ptr->dg_salen);
		if (len < 0) {
			if (errno == EWOULDBLOCK)
				break;		/* all done; no more queued to read */
			else{
				perror("recvfrom error");
				break;
			}
		}
		ptr->dg_len = len;

		nqueue++;
		if (++iput >= QSIZE)
			iput = 0;

	}	
}
