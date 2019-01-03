/**
 * Embedded Graphics Library
 */ 

 
#ifndef _EGL_H
#define _EGL_H

/*--------------------------------------------------------------------------
                    IMPORTATION DES EN-TETES DU MODULE
--------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*--------------------------------------------------------------------------
                              DEFINITIONS
--------------------------------------------------------------------------*/
#define BYTES_PER_ROW   20
#define CHAR_WIDTH      8
#define CHAR_HEIGHT     8
#define DISPLAY_WIDTH   160
#define DISPLAY_HEIGHT  128


// Offset du character map
#define ID_VIRGULE   0x82
#define ID_AL_ON     0x81
#define ID_AL_OFF    0x80
#define ID_MOINS     0xDD
#define ID_PHI       0xDE
#define ID_DEGRE     0x5F

typedef enum {
   CHAR_ASCII,
   CHAR_BYTE
} CHAR_TYPE;

typedef enum {
   NO_PADDING,     // affiche juste les digits disponibles
   BLANK_PADDING,  // affiche le maximum de digits supportés et complète avec des espaces
   ZERO_PADDING    // affiche le maximum de digits supportés et complète avec des espaces
} FIXPOINT_MODE;


typedef struct {
	uint16_t x, y;   // coordonnées du bargraph (en bas à gauche
   uint16_t l, h;   // largeur/hauteur du bargraph
	uint16_t val;		// valeur courante (coordonnée y en pixel)

} BARGRAPH_STRUCT;

typedef enum {
   CAL_SANS = 0,
   CAL_KILO = 1,
   CAL_MEGA = 2,
   CAL_GIGA = 3
} CALIBRE;


typedef struct {
   uint32_t val;
   bool  signed_val;
   uint8_t prec;
   uint8_t digits_max;
   // Coordonnées de droite car les nombres sont alignés à droite
   uint16_t x;
   uint16_t y;
   FIXPOINT_MODE mode;
} FIXPOINT_NUMBER;


/*--------------------------------------------------------------------------
                          VARIABLES EXPORTEES
--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------
                          FONCTIONS EXPORTEES
--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

extern void EGL_ClearScreen(void);
extern void EGL_Sprite6( const uint8_t *sprite, uint16_t x, uint16_t y, uint16_t h );
extern void EGL_Sprite16( const uint16_t *sprite, uint16_t x, uint16_t y, uint16_t h );
extern void EGL_Sprite40( const uint16_t *sprite, uint16_t x, uint16_t y, uint16_t h );
extern void EGL_SetPixel( uint16_t x, uint16_t y, uint16_t value );
extern uint16_t EGL_GetPixel( uint16_t x, uint16_t y );
extern void EGL_DrawLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t pettern );
extern void EGL_DrawCircle( uint8_t xCenter, uint8_t yCenter, uint8_t r );
extern void EGL_DrawRectangle( uint16_t x, uint16_t y, uint16_t h, uint16_t l );
extern void EGL_HLine( uint16_t x, uint16_t y, uint16_t len, uint16_t value );
extern void EGL_VLine( uint16_t x, uint16_t y, uint16_t len, uint16_t value );
extern void EGL_PutChar( uint8_t c, uint16_t x, uint16_t y, CHAR_TYPE c_type );
extern void EGL_PutString( const uint8_t *s, uint16_t x, uint16_t y );
extern uint16_t EGL_PutNumber( int32_t num, uint16_t x, uint16_t y, char *conv_specs );
extern CALIBRE EGL_PutBigNumber( FIXPOINT_NUMBER *num_sruct );
extern CALIBRE EGL_PutFixPoint( FIXPOINT_NUMBER *num_struct );
extern void EGL_PutBigChar( uint8_t c, uint8_t x, uint8_t y );
extern uint8_t EGL_StrLen( const uint8_t *str, uint8_t taille );
extern void EGL_Surbrillance( uint16_t index, uint16_t index_prec, uint8_t *tailles, uint16_t x );
extern void EGL_Curseur( uint16_t x_debut, uint16_t ligne, uint16_t digit, uint16_t digit_prec );
extern void EGL_RefreshBargraph( BARGRAPH_STRUCT *b );
extern void EGL_FillScreen(uint32_t *data);
extern void EGL_Slider(uint16_t index, uint16_t val, bool arrow);
extern void EGL_RefreshSlider(uint16_t index, uint16_t val);
extern void EGL_CopyImage16(const uint16_t *data, uint16_t x, uint16_t y, uint16_t cols, uint16_t lines );

#ifdef __cplusplus
}
#endif


#endif /* _EGL_H */

/*--------------------------------------------------------------------------
                              FIN FICHIER
--------------------------------------------------------------------------*/
/** @} */

