/**************************************************************************/
/**************************************************************************/
/*                                                                        */
/* Ce programme permet de creer un processus serveur et des clients       */
/*                                                                        */
/*                                                                        */
/**************************************************************************/
/**************************************************************************/

#include "../libc++/pcslib.h"
#include "../libc++/msglib.h"

main()
{  
   key_t key_porte = 99887766;

   Port Porte(key_porte);

   Pcs PcsProduct("ProductMsg");
   Pcs PcsConsom ("ConsomMsg");

   PcsProduct.Join();
   PcsConsom.Join();

   Porte.Detruit();
 }