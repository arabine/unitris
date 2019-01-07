
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "bmplib.h"

#define BF_TYPE 0x4D42             /* "MB" */


int BMP_Generate(uint32_t *vmem, uint32_t width, uint32_t height, char *out)
{
	bmp_header_t header;
	bmp_infoheader_t infoheader;
	
    uint32_t taille = width * height / 8;
    uint32_t offset;
    unsigned char *scratch;
	
    header.type = BF_TYPE;
    header.size = 0xa3e;
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.offset = 62;
   
    infoheader.size = 40;
    infoheader.width = (int32_t)width;
    infoheader.height = (int32_t)height;
    infoheader.planes = 1;
    infoheader.bits = 1;
    infoheader.compression = 0;
    infoheader.imagesize = taille;
    infoheader.xresolution = 0;
    infoheader.yresolution = 0;
    infoheader.ncolours = 2;
    infoheader.importantcolours = 2;
    infoheader.rgb = 0;
    infoheader.rgb2 = 0xffffff;

    scratch = (unsigned char*)malloc( taille*sizeof(unsigned char) );
    if( scratch == NULL )
    {
        printf("Memory allocation error. Exiting...\n");
        return 2;
    }

    for(uint32_t i=0; i<height; i++ )
    {
        for(uint32_t j=0; j<(width / 32); j++ )
        {
            offset = (taille / 4) + j - (5 * i) - 5;
            scratch[20*i+4*j] = ~(unsigned char)(vmem[offset]>>24);
            scratch[20*i+4*j+1] = ~(unsigned char)(vmem[offset]>>16);
            scratch[20*i+4*j+2] = ~(unsigned char)(vmem[offset]>>8);
            scratch[20*i+4*j+3] = ~(unsigned char)(vmem[offset]&0xff);
        }
    }

    memcpy(out, &header, sizeof(bmp_header_t));
    memcpy(out + sizeof(bmp_header_t), &infoheader, sizeof(bmp_infoheader_t));
    memcpy(out + sizeof(bmp_header_t) + sizeof(bmp_infoheader_t), &scratch[0], taille);

    free( scratch );
    return (0);
}

