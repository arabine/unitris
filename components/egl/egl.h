/**
 * Embedded Graphics Library
 */ 

 
#ifndef EGL_H
#define EGL_H

/*--------------------------------------------------------------------------
                    IMPORTATION DES EN-TETES DU MODULE
--------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------------------------------------------------------------
                              DEFINITIONS
--------------------------------------------------------------------------*/
#define BYTES_PER_ROW   20
#define CHAR_WIDTH      8
#define CHAR_HEIGHT     8


// Offset du character map
#define ID_VIRGULE   0x82
#define ID_AL_ON     0x81
#define ID_AL_OFF    0x80
#define ID_MOINS     0xDD
#define ID_PHI       0xDE
#define ID_DEGRE     0x5F

#define EGL_MAX_NUMBER_BUF 12 // the following should be enough for 32 bit int

typedef enum {
   CHAR_ASCII,
   CHAR_BYTE
} egl_char_type_t;

typedef enum {
   NO_PADDING,     // affiche juste les digits disponibles
   BLANK_PADDING,  // affiche le maximum de digits supportés et complète avec des espaces
   ZEROFILL_PADDING    // affiche le maximum de digits supportés et complète avec des espaces
} egl_fixpoint_mode_t;

typedef struct {
   uint32_t val;
   bool  signed_val;
   uint8_t prec;
   uint8_t digits_max;
   // Coordonnées de droite car les nombres sont alignés à droite
   uint16_t x;
   uint16_t y;
   egl_fixpoint_mode_t mode;
} egl_fixpoint_number_t;

typedef struct
{
    uint32_t *Vmem;
    uint32_t mem_size;
    uint16_t display_width;
    uint16_t display_height;

    char scratch[EGL_MAX_NUMBER_BUF];
} egl_t;

void EGL_Initialize(egl_t *ctx, uint16_t width, uint16_t height, uint32_t *mem);
void EGL_ClearScreen(egl_t *ctx);
void EGL_SetPixel(egl_t *ctx, uint16_t x, uint16_t y, uint16_t value);
uint16_t EGL_GetPixel(egl_t *ctx, uint16_t x, uint16_t y);
void EGL_Sprite6(egl_t *ctx, const uint8_t *sprite, uint16_t x, uint16_t y, uint16_t h);
void EGL_CopyImage(egl_t *ctx, const uint8_t *data, uint16_t x, uint16_t y, uint16_t cols, uint16_t lines );
void EGL_PutString(egl_t *ctx, const char *s, uint16_t x, uint16_t y);
void EGL_HLine(egl_t *ctx, uint16_t x, uint16_t y, uint16_t len, uint16_t value);
void EGL_VLine(egl_t *ctx, uint16_t x, uint16_t y, uint16_t len, uint16_t value);
void EGL_DrawRectangle(egl_t *ctx, uint16_t x, uint16_t y, uint16_t h, uint16_t l);
uint16_t EGL_PutNumber(egl_t *ctx, int32_t num, uint16_t x, uint16_t y, char *conv_specs );

#ifdef __cplusplus
}
#endif


#endif /* EGL_H */

/*--------------------------------------------------------------------------
                              FIN FICHIER
--------------------------------------------------------------------------*/
/** @} */

