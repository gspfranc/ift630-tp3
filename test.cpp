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
#include <netdb.h>
//#include <unistd.h>

#define PORT_NUM 5000
#define NUM_CLIENTS	5


void doprocessing (int sock)
{
    int n;
    char buffer[256];

    memset(buffer,0,256);

    n = read(sock,buffer,255);
    if (n < 0)
    {
        printf("printf reading from socket");
        exit(1);
    }
    printf("[SERVER] Request Received: %s\n",buffer);
    n = write(sock,"[[file.txt]]",12);
    if (n < 0) 
    {
        printf("printf writing to socket");
        exit(1);
    }
	//pthread_exit(NULL);
}


void *Server(void *threadid){
 int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        printf("ERROR opening socket \n");
     memset((char *) &serv_addr,0, sizeof(serv_addr));
     portno = PORT_NUM;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              printf("ERROR on binding \n");
     listen(sockfd,5);
	 
    clilen = sizeof(cli_addr);
    while (1) 
    {
        newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
        {
            printf("ERROR on accept");
            exit(1);
        }
        /* Create child process */
        pid_t pid = fork();
        if (pid < 0)
        {
            printf("ERROR on fork");
	    exit(1);
        }
        if (pid == 0)  
        {
            /* This is the client process */
            close(sockfd);
            doprocessing(newsockfd);
            exit(0);
        }
        else
        {
            close(newsockfd);
        }
    } /* end of while */
}

 
 
 
  void *Client(void *threadid){
     int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
   
    portno = PORT_NUM;
	/** INIT **/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        printf("printf opening socket");
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"printf, no such host\n");
        exit(0);
    }
    memset((char *) &serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)&serv_addr.sin_addr.s_addr,
	       (char *)server->h_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    
	/** CONNECT **/
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        printf("printf connecting");
    memset(buffer,0,256);
	
	/** WRITE **/
	
    n = write(sockfd,"REQUEST FILE",12);
    if (n < 0) 
         printf("printf sending request");

    n = read(sockfd,buffer,255);
    if (n < 0) 
         printf("printf reading from socket");
    printf("[CLIENT] I received: %s\n",buffer);
    close(sockfd);
    return 0;
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
         printf(" CANNOT START SERVER return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
	  printf("Server is started \n");


   for(t=0; t<NUM_CLIENTS; t++){
      rc = pthread_create(&clients[t], NULL, Client, (void *)t);
      if (rc){
         printf(" return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
	  printf("client is started: %ld\n", t+1);
}
  pthread_exit(NULL);
 
 
 
 }