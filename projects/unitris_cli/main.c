
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


#include "unitris.h"
#include "debug.h"

 
int main(int argc, char **argv)
{
    
    // initilize seed
    srand(time(NULL));
    
    unitris_t ctx;
    
    UNI_Initialize(&ctx);
    
    
    for (int i = 0; i < 50; i++)
    {
        DBG_DumpGrid(&ctx);
        UNI_Tick(&ctx, 0, 0, 0);
        printf("\n");
    }
    
    
    return 0;
}
