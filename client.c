
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

/* client program*/
main(int argc,char *argv[])
{
  int sock, length, port;
  struct sockaddr_in source, si_me, dest,dest2;
  int msgsock;
  char buf[1024];
  int rval, srclen;
  int i, seqno;
  struct hostent *hp,*hp2, *gethostbyname();

/* create socket */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  si_me.sin_family = AF_INET;
  si_me.sin_addr.s_addr = INADDR_ANY;
  si_me.sin_port = 0;
  if(bind(sock, (struct sockaddr *)&si_me, sizeof(si_me))) {
    perror("binding socket name");
    exit(1);
  }

  dest.sin_family = AF_INET;
  hp = gethostbyname(argv[1]);//master server
  bcopy(hp->h_addr, &dest.sin_addr, hp->h_length);
  port = atoi(argv[2]);//master port
  dest.sin_port = htons(port); 

  length = sizeof(si_me);
  if(getsockname(sock, (struct sockaddr *)&si_me, &length)) {
    perror("getting socket name");
    exit(1);
  }
  printf("Hello! I am CLIENT\n\n",ntohs(si_me.sin_port));
  char input[128];
  printf("Select your choice from :[1]add,[2]substract,[3]multiply,[4]divide:\n");
  scanf("%s",input);
  if ((rval=sendto(sock,input,sizeof(input),0,(struct sockaddr *)&dest,sizeof(dest))) < 0) {
 	perror("writing on datagram socket");
  }
  
  while(1){	
      bzero(buf,sizeof(buf));
      srclen = sizeof(source);
      while ((rval = recvfrom(sock,buf,1024,0,(struct sockaddr *)&source,&srclen))<0){
        perror("receiver recvfrom");
      }
      char *buff =buf;
      if(ntohs(source.sin_port)== port){ 	  /* recieve from master*/
	      dest2.sin_family = AF_INET;
	 	  hp2 = gethostbyname(argv[3]);

	      bcopy(hp2->h_addr, &dest2.sin_addr, hp2->h_length);
	      dest2.sin_port = htons(atoi(buf)); 
	      
	      bzero(buf,sizeof(buf));
	      printf("Tell me your first number:\n");
	      scanf("%s",buf);
		  /* send it to master server*/
		  if ((sendto(sock,buf,sizeof(buf),0,(struct sockaddr *)&dest2,sizeof(dest2))) < 0) {
	         perror("writing on datagram socket");
	      }
	      
  		  bzero(buf,sizeof(buf));
	      printf("Tell me your second number:\n");
	      scanf("%s",buf);
		  /* send it to master server*/
		  if ((sendto(sock,buf,sizeof(buf),0,(struct sockaddr *)&dest2,sizeof(dest2))) < 0) {
	         perror("writing on datagram socket");
	      }
      } else{ /* recieve from calculation server*/
      	    printf("Em.. result is: %s\n\n", buf);
      	    bzero(input,sizeof(input));
   			printf("Select your choice from :[1]add,[2]substract,[3]multiply,[4]divide:\n");
           scanf("%s",input);
           if ((rval=sendto(sock,input,sizeof(input),0,(struct sockaddr *)&dest,sizeof(dest))) < 0) {
			   perror("writing on datagram socket");
  		   }
	  }
  }
}

