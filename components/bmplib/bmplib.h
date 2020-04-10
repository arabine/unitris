#ifndef BMP_LIB_H
#define BMP_LIB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#define PACKED
#pragma pack(push,1)

#else
#define PACKED __attribute__ ((packed))

#endif

typedef struct PACKED {
   uint16_t type;                 /* Magic identifier            */
   uint32_t size;                       /* File size in bytes          */
   uint16_t reserved1;
   uint16_t reserved2;
   uint32_t offset;                     /* Offset to image data, bytes */
} bmp_header_t ;

typedef struct PACKED {
   uint32_t size;               /* Header size in bytes      */
   
   /* Width and height of image */
   int32_t width;
   int32_t height;                
   uint16_t planes;       /* Number of colour planes   */
   uint16_t bits;         /* Bits per pixel            */
   uint32_t compression;        /* Compression type          */
   uint32_t imagesize;          /* Image size in bytes       */
   
   /* Pixels per meter          */
   int32_t xresolution;
   int32_t yresolution;    
   uint32_t ncolours;           /* Number of colours         */
   uint32_t importantcolours;   /* Important colours         */
   uint32_t rgb;
   uint32_t rgb2;
} bmp_infoheader_t ;

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#undef PACKED

#define BMP_FILE_SIZE(w, h)   (sizeof(bmp_header_t) + sizeof(bmp_infoheader_t) + (w*h/8))

int BMP_Generate(uint32_t *vmem, uint32_t width, uint32_t height, char *out);

#ifdef __cplusplus
}
#endif

#endif // BMP_LIB_H
