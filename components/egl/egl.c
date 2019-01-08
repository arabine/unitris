/**
 * Embedded Graphics Library
 */ 


#include "egl.h"
#include "sys_printf.h"

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
 * Fonction    :  EGL_GetPixel
 *------------------------------------------------------------------------*/
/** \brief Récupère la couleur d'un pixel
 *
 * \param uint16_t x,y : coordonnées du point
 *
 * \retval uint16_t : 0=clear 1=set
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
uint16_t EGL_GetPixel(egl_t *ctx, uint16_t x, uint16_t y)
{
   uint32_t *addr;
   uint16_t bit = (x & 31); // == %32
   uint32_t bitmask;
   uint16_t pixel = 0U;

   // protection
   if ((x < ctx->display_width) && (y < ctx->display_height))
   {
        // >> 5 is equivalent to divide by 32
       addr=ctx->Vmem+(ctx->display_width >> 5)*y+(x>>5);
       bitmask = 0x80000000UL>>(bit);
       pixel =(uint16_t)(((*addr)&bitmask)>>(31-bit));
   }
   return pixel;
}

/*--------------------------------------------------------------------------
 * Fonction    :  EGL_DrawByte
 *------------------------------------------------------------------------*/
/** \brief Affiche un octet, avec possibilité de limiter le nombre de bits
 *
 * \param const uint16_t *sprite : pointeur vers le sprite à afficher
 * \param uint16_t x : abscisse du coin supérieur gauche
 * \param uint16_t y : ordonnée du coin supérieur gauche
 * \param uint16_t h : nombre de lignes du sprite
 *
 * \retval -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_DrawByte(egl_t *ctx, uint8_t byte, uint16_t x, uint16_t y, uint8_t nb_bits)
{
    uint8_t mask = 0x80U;
    for (uint8_t i = 0U; i < nb_bits; i++)
    {
        EGL_SetPixel(ctx, x+i, y, byte & mask);
        mask >>= 1;
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
    for(;h--;)
    {
        uint8_t data = *sprite++;
        EGL_DrawByte(ctx, data, x, y, 6);
        y++; // Jump memory line
    }
}

/*--------------------------------------------------------------------------
 * Fonction    :  EGL_CopyImage
 *------------------------------------------------------------------------*/
/** \brief Copie brute d'une image, écrase tout
 *
 * \param const uint8_t *data : image
 * \param uint16_t x : colonne où afficher l'image (modulo 16)
 * \param uint16_t y : ligne où afficher l'image
 * \param uint16_t cols : nombre de colonnes que prend l'image
 * \param uint16_t lines : nombre de lignes de l'image
 *
 * \retval -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_CopyImage(egl_t *ctx, const uint8_t *data, uint16_t x, uint16_t y, uint16_t cols, uint16_t lines )
{
   for (;lines--;)
   {
       uint16_t current_x = x;
      for(uint16_t i=0; i<cols; i++ )
      {
         EGL_DrawByte(ctx, *data++, current_x, y, 8);
         current_x += 8;
      }
      y++;
   }
}

/*--------------------------------------------------------------------------
 * Fonction    :  EGL_DrawRectangle
 *------------------------------------------------------------------------*/
/** \brief Dessine un rectangle
 *
 * \param uint16_t x,y : coordonnées en bas à gauche du rectangle
 * \param h, l : hauteur, largeur
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_DrawRectangle(egl_t *ctx, uint16_t x, uint16_t y, uint16_t h, uint16_t l)
{
   EGL_HLine(ctx, x, y, l, 1 );
   EGL_HLine(ctx, x, y-h+1, l, 1 );
   EGL_VLine(ctx, x, y-h+1, h, 1 );
   EGL_VLine(ctx, x+l-1, y-h+1, h, 1 );
}


/*--------------------------------------------------------------------------
 * Fonction    :  LCD_HLine
 *------------------------------------------------------------------------*/
/** \brief Affiche une ligne horizontale de gauche à droite
 *
 * \param uint16_t x,y : coordonnées du départ de la ligne
 * \param len : longueur de la ligne
 * \param uint16_t value :  0=clear 1=set 2=inverse
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_HLine(egl_t *ctx, uint16_t x, uint16_t y, uint16_t len, uint16_t value)
{
   uint16_t i;
   uint16_t color = value;

   for( i=x; i<(x+len); i++ ) {
      if (value == 2 ) {
         color = (~EGL_GetPixel(ctx, i,y))&0x1;
      }
      EGL_SetPixel(ctx, i,y,color);
   }
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_VLine
 *------------------------------------------------------------------------*/
/** \brief Affiche une ligne verticale de haut en bas
 *
 * \param uint16_t x,y : coordonnées du départ de la ligne
 * \param len : longueur de la ligne
 * \param uint16_t value :  0=clear 1=set 2=inverse
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_VLine(egl_t *ctx, uint16_t x, uint16_t y, uint16_t len, uint16_t value)
{
   uint16_t i;
   uint16_t color = value;

   for( i=y; i<(y+len); i++ ) {
      if (value == 2 ) {
         color = (~EGL_GetPixel(ctx, x,i))&0x1;
      }
      EGL_SetPixel(ctx, x,i,color);
   }
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
 * Fonction    :  EGL_PutNumber
 *------------------------------------------------------------------------*/
/** \brief Affiche une chaîne de caractères terminée par un '\0'
 *
 * \param int32_t num : le nombre à afficher
 * \param x, y : colonne, ligne du début de la chaîne (en pixels)
 * \param char *conv_specs : chaîne de formatage à la printf
 *
 * \retval -
 *
 * \remarks Exemple : EGL_PutNumber(Jbus.Params_Brm.instrument_model,100,96,"%3ld");
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
uint16_t EGL_PutNumber(egl_t *ctx, int32_t num, uint16_t x, uint16_t y, char *conv_specs )
{
   uint16_t len;

   len = (uint16_t)sys_snprintf(ctx->scratch, EGL_MAX_NUMBER_BUF, conv_specs, num);
   EGL_PutString(ctx, ctx->scratch, x, y);
   return(len);
}


/*--------------------------------------------------------------------------
                             FIN FICHIER
--------------------------------------------------------------------------*/
/** @} */

