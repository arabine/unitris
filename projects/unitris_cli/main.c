
#include "unitris.h"

#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
 
int main(int argc, char **argv)
{
    
    // initilize seed
    srand(time(NULL));
    
    unitris_t ctx;
    
    UNI_Initialize(&ctx);
    
    
    return 0;
}
