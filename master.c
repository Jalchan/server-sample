
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>
#include <net/if.h>
#include <sys/ioctl.h>

/* Master program*/
main(int argc,char *argv[])
{
  int sock, length;
  struct sockaddr_in source, si_me;
  int msgsock;
  char buf[1024],port[128];
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
  printf("I am MASTER\n\n",ntohs(si_me.sin_port));

  while(1) {
      bzero(buf,sizeof(buf));
      srclen = sizeof(source);
      while ((rval = recvfrom(sock,buf,1024,0,(struct sockaddr *)&source,&srclen))<0){
        perror("receiver recvfrom");
      }      
	  printf("Client want to do <%s>\n", buf); 
      bzero(port,sizeof(port));
      if(strcmp(buf,"1")==0){
    	strcpy(port,"3301");	
	  } else if (strcmp(buf,"2")==0){
    	strcpy(port,"3302");
	  } else if (strcmp(buf,"3")==0){
    	strcpy(port,"3303");	
	  } else {
	  	strcpy(port,"3304");
	  }
	  /* send it to client*/
	  if ((sendto(sock,port,sizeof(port),0,(struct sockaddr *)&source,sizeof(source))) < 0) {
         perror("writing on datagram socket");
      }
  } 
  close(sock);
}


