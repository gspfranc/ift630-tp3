/**************************************************************************/
/**************************************************************************/
/*                                                                        */
/* Ce programme permet d'implanter un processus client                    */
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

  
   key_t key_request = 442152; 
   Port RequestPort(key_request);

   //Envoie d'une demande
   RequestPort.Envoie(&demande, sizeof(demande));
   
   //Recevoir le message
   ResponsePort.Recevoir(&reponse,sizeof(reponse));
   
   
}