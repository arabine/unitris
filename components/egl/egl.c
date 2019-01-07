/**
 * Embedded Graphics Library
 */ 


#include "egl.h"
//#include "sys_printf.h"

extern const uint8_t character_map[];

/*--------------------------------------------------------------------------
                     PROTOTYPE DES FONCTIONS INTERNES
--------------------------------------------------------------------------*/
uint8_t EGL_IntToStr( char *s_nbr, egl_fixpoint_number_t *num_struct );

/*--------------------------------------------------------------------------
                              VARIABLES GLOBALES
--------------------------------------------------------------------------*/
#define FIXPOINT_BUFFER_SIZE  21 // taille max pour faire tenir deux 32bits + le signe


void EGL_Initialize(egl_t *ctx, uint16_t width, uint16_t height, uint32_t *mem)
{
     ctx->mem_size = (width * height)/32;
     ctx->display_width = width;
     ctx->display_height = height;
     ctx->Vmem = mem;
}

/*--------------------------------------------------------------------------
 * Fonction    :  EGL_ClearScreen
 *------------------------------------------------------------------------*/
/** \brief Efface entičrement l'écran
 *
 * \param -
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_ClearScreen(egl_t *ctx)
{
    for (uint32_t i = 0; i < ctx->mem_size; i++ )
	{
		ctx->Vmem[i] = 0;
	}
   //memset(Vmem,0,sizeof(Vmem));
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_SetPixel
 *------------------------------------------------------------------------*/
/** \brief Allume ou éteint un pixel
 *
 * \param uint16_t x,y : coordonnées du point
 * \param uint16_t value :  0=clear 1=set
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_SetPixel(egl_t *ctx, uint16_t x, uint16_t y, uint16_t value)
{
   uint32_t *addr;
   uint32_t bitmask;

   // protection
   if ((x < ctx->display_width) && (y < ctx->display_height))
   {
       // >> 5 is equivalent to divide by 32
       addr = ctx->Vmem+(ctx->display_width >> 5) * y + (x >> 5);
       bitmask = 0x80000000UL >> ( x & 31); // modulo 32
       if (value)
       {
          *addr |= bitmask; // set
       }
       else
       {
          *addr &= ~bitmask; // clear
       }
    }
}

/*--------------------------------------------------------------------------
 * Fonction    :  EGL_Sprite6
 *------------------------------------------------------------------------*/
/** \brief Affiche un Sprite de largeur 6px et de hauteur h pixels
 *
 * \param const uint16_t *sprite : pointeur vers le sprite à afficher
 * \param uint16_t x : abscisse du coin supérieur gauche
 * \param uint16_t y : ordonnée du coin supérieur gauche
 * \param uint16_t h : nombre de lignes du sprite
 *
 * \retval -
 *
 * \remarks Utilisé pour les caractères
 *          La table de caractère a les deux bits de poids faibles à zéro (6 bits utiles)
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_Sprite6(egl_t *ctx, const uint8_t *sprite, uint16_t x, uint16_t y, uint16_t h)
{

#ifndef _LITTLE_ENDIAN
   for(;h--;)
   {
        uint8_t data = *sprite++;
        uint8_t mask = 0x80U;
        for (uint8_t i = 0U; i < 6; i++)
        {
            EGL_SetPixel(ctx, x+i, y, data & mask);
            mask >>= 1;
        }
        // Jump memory line
        y++;
   }

/*
   cnt1 = 8 - (x & 15);
   cnt2 = 16 - (-1*cnt1);

   for(;h--;addr+=(ctx->display_width/16))
   {
      data = *sprite++;
      *addr &= ~((0xFCUL)<<(cnt1)) ; // on efface
      *addr |= (data&0xFC)<<(cnt1);
      // protection effet de bord
      if ( x < (ctx->display_width-16) ) {
         *(addr+1) &= ~((0xFCUL)<<(cnt2)); // on efface
         *(addr+1) |= (data&0xFC)<<(cnt2);
      }
   }
   */
#else

  uint32_t *addr=ctx->Vmem+(ctx->display_width/32)*y+(x/32);

   int16_t cnt1;
   uint16_t cnt2;
   uint8_t data;

   cnt1 = 24-(x&31);
   cnt2 = 32-((-1)*cnt1);

   for(;h--;addr+=(ctx->display_width/32)) {
      data = *sprite++;

      if( cnt1 >= 0 ) {
        *addr &= ~((0xFCUL)<<(cnt1)); // on efface
        *addr |= (data&0xFC)<<(cnt1);
      } else {
        *addr &= ~((0xFCUL)>>(-1*cnt1)); // on efface
        *addr |= (data&0xFC)>>(-1*cnt1);
      }
      // protection effet de bord
      if ( x < (ctx->display_width-32) && cnt1<0 ) {
         *(addr+1) &= ~((0xFCUL)<<(cnt2)); // on efface
         *(addr+1) |= (data&0xFC)<<(cnt2);
      }
   }

#endif
}

/*--------------------------------------------------------------------------
 * Fonction    :  EGL_PutChar
 *------------------------------------------------------------------------*/
/** \brief Affiche un caractère
 *
 * \param uint8_t c : le caractère en ASCII (ou le code correspondant)
 * \param x, y : colonne, ligne du début de la chaîne
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_PutChar(egl_t *ctx, uint8_t c, uint16_t x, uint16_t y, egl_char_type_t c_type)
{
   const uint8_t *ptr;

   if ( x > (ctx->display_width - 6) )
   {
      return;
   }
   if ( y > (ctx->display_height - 8) )
   {
      return;
   }

   c &= 0xFF;
   if ( c_type == CHAR_ASCII )
   {
      c -= 0x20;
   }
   ptr = character_map+8*c;
   EGL_Sprite6(ctx, ptr, x, y, 8 );
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_PutString
 *------------------------------------------------------------------------*/
/** \brief Affiche une chaîne de caractères terminée par un '\0'
 *
 * \param const uint8_t *string : la chaîne de caractères
 * \param x, y : colonne, ligne du début de la chaîne
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_PutString(egl_t *ctx, const char *s, uint16_t x, uint16_t y)
{
   uint8_t c;
   while ( *s )
   {
      c = (uint8_t)*s++;
      // petite protection pour éviter d'aller écrire d'importe où
      if ((x > 154) || (y > 120))
      {
         return;
      }
      EGL_PutChar(ctx, c, x, y, CHAR_ASCII);
      x += 6;
   }
}

/*--------------------------------------------------------------------------
                             FIN FICHIER
--------------------------------------------------------------------------*/
/** @} */

