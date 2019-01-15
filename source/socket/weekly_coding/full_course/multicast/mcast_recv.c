#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GROUP_ADDR "239.255.1.1"
#define LOCAL_ADDR "192.168.4.21"
#define PORT 5500

int main (int argc, char *argv[])
{
  struct sockaddr_in    localSock;
  struct ip_mreq        group;
  int                   recvfd;
  int                   datalen, ret;
  char                  databuf[32];

  /*
   * Create a datagram socket on which to receive.
   */
  recvfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (recvfd < 0) {
    perror("opening datagram socket");
    return 0;
  }

  /*
   * Enable SO_REUSEADDR to allow multiple instances of this
   * application to receive copies of the multicast datagrams.
   */
  {
    int reuse=1;

    if (setsockopt(recvfd, SOL_SOCKET, SO_REUSEADDR,
                   (char *)&reuse, sizeof(reuse)) < 0) {
      perror("setting SO_REUSEADDR");
      close(recvfd);
      return 0;
    }
  }

  /*
   * Bind to the proper port number with the IP address
   * specified as INADDR_ANY.
   */
  memset((char *) &localSock, 0, sizeof(localSock));
  localSock.sin_family = AF_INET;
  localSock.sin_port = htons(PORT);;
  localSock.sin_addr.s_addr  = INADDR_ANY;

  if (bind(recvfd, (struct sockaddr*)&localSock, sizeof(localSock))) {
    perror("binding datagram socket");
    close(recvfd);
    return 0;
  }


  /*
   * Join the multicast group GROUP_ADDR on the local LOCAL_ADDR
   * interface.  Note that this IP_ADD_MEMBERSHIP option must be
   * called for each local interface over which the multicast
   * datagrams are to be received.
   */
  group.imr_multiaddr.s_addr = inet_addr(GROUP_ADDR);
  group.imr_interface.s_addr = inet_addr(LOCAL_ADDR);
  if (setsockopt(recvfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                 (char *)&group, sizeof(group)) < 0) {
    perror("adding multicast group");
    close(recvfd);
    return 0;
  }

  /*
   * Read from the socket.
   */
  datalen = sizeof(databuf);
  if ((ret = read(recvfd, databuf, datalen)) < 0) {
    perror("reading datagram message");
  }
  else{
	databuf[ret] = '\0';
	printf("\nReceived multicast message: %s\n", databuf);
  }
  
  close(recvfd);
  return 0;	
}