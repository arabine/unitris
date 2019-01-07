
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>

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

void DBG_BufferToFile(const char *buffer, uint32_t size, const char *filename)
{
    FILE *fout = NULL;

    fout = fopen(filename,"wb");

    if (fout != NULL)
    {
       (void) fwrite(buffer, sizeof(char), size, fout);
       fclose(fout);
    }
}



 
