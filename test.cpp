/**************************************************************************/
/**************************************************************************/
/*                                                                        */
/* Ce programme permet de creer un processus serveur et des clients       */
/*                                                                        */
/*                                                                        */
/**************************************************************************/
/**************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_NUM 5000
#define NUM_CLIENTS	1
void error(const char *msg)
{
    perror(msg);
    exit(1);
}


void *Server(void *threadid){
      int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
	printf("Strting to init socket \n");
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     memset((char *) &serv_addr,0, sizeof(serv_addr));
     portno = PORT_NUM;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     memset(buffer,0,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);
 pthread_exit(NULL);
 
 }
 
 
 
  void *Client(void *threadid){
    
  pthread_exit(NULL);

  }




main()
{  
//***** Création des thread ****//
pthread_t serveur;
pthread_t clients[NUM_CLIENTS];



int rc;
int t;

	rc = pthread_create(&serveur, NULL, Server, (void *)t);
      if (rc){
         printf("ERROR; CANNOT START SERVER return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
	  printf("Server is started \n");


   for(t=0; t<NUM_CLIENTS; t++){
      rc = pthread_create(&clients[t], NULL, Client, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
	  printf("client is started: %ld\n", t+1);
}
  pthread_exit(NULL);
 
 
 
 }