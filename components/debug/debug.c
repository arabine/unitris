
#include "debug.h"
#include <stdio.h>

void DBG_DumpGrid(const unitris_t *ctx)
{
    // init variables
    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            printf("%d ", ctx->grid[i][j]);
        }
        printf("\n");
    }
}



 
