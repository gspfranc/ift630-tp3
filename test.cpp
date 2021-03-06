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
#include <iostream>
#include <signal.h>
#include <iostream>
#include <fstream>


#define PORT_NUM 5000
#define NUM_CLIENTS	50

typedef void Sigfunc(int);
//Sigfunc *signal(int,Sigfunc *);

void doprocessing (int sock)
{
    int n;
    char buffer[256];

    memset(buffer,0,256);

    n = read(sock,buffer,255);
    if (n < 0)
    {
        exit(1);
    }
    printf("[SERVER] Request Received: %s\n",buffer);
    std::ifstream myfile (buffer);
    std::string line;

    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            n = write(sock,line.c_str(),line.size());
        }
        myfile.close();
    }

    n = write(sock,"[[file.txt]]",12);
    if (n < 0)
    {
        printf("printf writing to socket");
        exit(1);
    }
}

int sock_server, new_sock_server;
void *Server(void *threadid)
{
	int no_de_port;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in adresse_serveur, adresse_client;
    int n;

    sock_server = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_server < 0)
        printf("Impossible d'ouvrir la socket \n");

    //Remplir de zero
    memset((char *) &adresse_serveur,0, sizeof(adresse_serveur));

    no_de_port = PORT_NUM;

    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_addr.s_addr = INADDR_ANY;
    adresse_serveur.sin_port = htons(no_de_port);

    //Binder la socket
    if (bind(sock_server, (struct sockaddr *) &adresse_serveur,
             sizeof(adresse_serveur)) < 0)
        printf("Impossible de binder la socket \n");
    listen(sock_server,5);

    clilen = sizeof(adresse_client);
    while (1) //Pour toujours
    {
        //ACCEPT
        new_sock_server = accept(sock_server,
                           (struct sockaddr *) &adresse_client, &clilen);
        if (new_sock_server < 0)
        {
            printf("ERROR on accept");
            exit(1);
        }
        /* Créer un enfant */
        pid_t pid = fork();
        if (pid < 0)
        {
            printf("ERROR on fork");
            exit(1);
        }
        if (pid == 0)
        {
            /* C'est le client */
            close(sock_server);
            doprocessing(new_sock_server);
            exit(0);
        }
        else
        {
            //fermeture des sockets
            close(new_sock_server);
        }
    } /* end of while */
}


int sockfd_client;
void *Client(void *threadid)
{
int no_de_port,n;
    struct sockaddr_in adresse_serveur;
    struct hostent *server;
    std::ofstream myfile;

    char buffer[256];

    no_de_port = PORT_NUM;
    /** INIT **/
    sockfd_client = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_client < 0)
        printf("Impossible de créer la socket");

    server = gethostbyname("localhost");
    if (server == NULL)
    {
        fprintf(stderr,"impossible de contacter l'host \n");
        exit(0);
    }
    memset((char *) &adresse_serveur,0, sizeof(adresse_serveur));
    adresse_serveur.sin_family = AF_INET;
    memcpy((char *)&adresse_serveur.sin_addr.s_addr,
           (char *)server->h_addr,
           server->h_length);
    adresse_serveur.sin_port = htons(no_de_port);

    /** CONNECT **/
    if (connect(sockfd_client,(struct sockaddr *) &adresse_serveur,sizeof(adresse_serveur)) < 0)
        printf("printf connecting");
    memset(buffer,0,256);


    /** WRITE **/
    n = write(sockfd_client,"file.txt",8);
    if (n < 0)
        printf("Impossible d'envoyer la request");

    n = read(sockfd_client,buffer,255);
    if (n < 0)
        printf("Impossible de lire la reponse de la socket");
    printf("[CLIENT] I received: %s\n",buffer);

    myfile.open("received.txt");
    //Écriture dans le fichier
    myfile << buffer;
    myfile.close();
    close(sockfd_client);
    return 0;
    pthread_exit(NULL);

}

void cleanExit(int code)
{
    printf(" \n Kill signal received... closing sockets \n");
    close(sock_server);
    close(new_sock_server);
	//close(sockfd_client);
    exit(0);
}

main()
{
//***** Création des thread ****//
    pthread_t serveur;
    pthread_t clients[NUM_CLIENTS];


//***Enregistrement des handler de signals***//
   signal(SIGINT,cleanExit);



    int rc;
    int t;

    rc = pthread_create(&serveur, NULL, Server, (void *)t);
    if (rc)
    {
        printf(" Impossible de démarer le serveur: %d\n", rc);
        exit(-1);
    }
    printf("Server is started \n");


    for(t=0; t<NUM_CLIENTS; t++)
    {
        rc = pthread_create(&clients[t], NULL, Client, (void *)t);
        if (rc)
        {
            printf(" Impossible de créer le client:  %d\n", rc);
            exit(-1);
        }
        printf("Client is started: %ld\n", t+1);
    }
    pthread_exit(NULL);



}
