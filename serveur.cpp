/**************************************************************************/
/**************************************************************************/
/*                                                                        */
/* Ce programme permet d'implanter un processus serveur                   */
/*                                                                        */
/*                                                                        */
/**************************************************************************/
/**************************************************************************/
#include <unistd.h>
#include "../libc++/msglib.h"
#include <stdio.h>

 
main()
{ 
   struct request 
   {
     //??
   } demande;

	struct response 
   {
     //??
   } reponse;   

  
   key_t key_request = 442152; 
   Port RequestPort(key_request
   
   key_t key_response = 448841; 
   Port ResponsePort(key_response);

   while(true)
   { 
		//attente d'une demande d'un client
		RequestPort.Recoit(&demande,sizeof(demande));
		//Envoie de l'information vers le client
		ResponsePort.Envoie(&reponse,sizeof(response));
   }  
}