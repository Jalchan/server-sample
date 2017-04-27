
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <signal.h>
#include <net/if.h>
#include <sys/ioctl.h>


/* Divide program. */
main(int argc,char *argv[])
{
  int sock, length;
  struct sockaddr_in source, si_me;
  int msgsock;
  char buf[1024];
  int rval, srclen;
  struct hostent *hp, *gethostbyname();

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  si_me.sin_family = AF_INET;
  si_me.sin_addr.s_addr = INADDR_ANY;
  si_me.sin_port = htons(atoi(argv[1]));
  if(bind(sock, (struct sockaddr *)&si_me, sizeof(si_me))) {
    perror("binding socket name");
    exit(1);
  }

  length = sizeof(si_me);
  if(getsockname(sock, (struct sockaddr *)&si_me, &length)) {
    perror("getting socket name");
    exit(1);
  }
  printf("I am DIVIDE server\n\n",ntohs(si_me.sin_port));
  int flag =0;
  int a,b;
  do {
      flag++;
      bzero(buf,sizeof(buf));
      srclen = sizeof(source);
	  /* recieve from client*/
      while ((rval = recvfrom(sock,buf,1024,0,(struct sockaddr *)&source,&srclen))<0){
        perror("receiver recvfrom");
      }
      if(flag%2==0){
		  sscanf(buf, "%d", &b);
		  sprintf(buf,"%d", a/b);	
		  printf("%d / %d = %s\n\n",a,b,buf);
		  /* send it to client*/
		  if ((sendto(sock,buf,sizeof(buf),0,(struct sockaddr *)&source,sizeof(source))) < 0) {
	         perror("writing on datagram socket");
	      }
	  } else{
 		  sscanf(buf, "%d", &a);
	  }      
  } while (1);
  close(sock);
}


