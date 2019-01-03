/**
 * Embedded Graphics Library
 */ 


#include "egl.h"
#include "sys_printf.h"

extern const uint8_t character_map[];

/*--------------------------------------------------------------------------
                     PROTOTYPE DES FONCTIONS INTERNES
--------------------------------------------------------------------------*/
uint8_t EGL_IntToStr( char *s_nbr, FIXPOINT_NUMBER *num_struct );

/*--------------------------------------------------------------------------
                              VARIABLES GLOBALES
--------------------------------------------------------------------------*/
#define FIXPOINT_BUFFER_SIZE  21 // taille max pour faire tenir deux 32bits + le signe
#define VMEM_SIZE ((DISPLAY_WIDTH*DISPLAY_HEIGHT)/32)

static uint32_t Vmem[VMEM_SIZE]; // Video Memory -> où l'on dessine

/*--------------------------------------------------------------------------
 * Fonction    :  EGL_ClearScreen
 *------------------------------------------------------------------------*/
/** \brief Efface entièrement l'écran
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
void EGL_ClearScreen(void)
{
	for (uint32_t i = 0; i < VMEM_SIZE; i++ )
	{
		Vmem[i] = 0;
	}
   //memset(Vmem,0,sizeof(Vmem));
}



/*--------------------------------------------------------------------------
 * Fonction    :  EGL_FillScreen
 *------------------------------------------------------------------------*/
/** \brief Rempli l'écran avec une image
 *
 * \param uint32_t *data : pointeur vers l'image
 *
 * \retval -
 *
 * \remarks Il faut que l'image soit de la bonne taille
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_FillScreen(uint32_t *data)
{
   uint16_t i;
   uint32_t *ptr = Vmem;

   for (i=0; i<VMEM_SIZE; i++ ) {
     *ptr++ = *data++;
   }
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_CopyImage16
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
 * \remarks 1) L'image doit être un multiple de 16 pixels
 *          2) l'abscisse doit être un multiple de 16
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_CopyImage16(const uint16_t *data, uint16_t x, uint16_t y, uint16_t cols, uint16_t lines )
{
   uint16_t *addr=(uint16_t*)Vmem+(DISPLAY_WIDTH/16)*y + x;
   uint16_t i;
   uint16_t *ptr = (uint16_t*)data;

   for(;lines--;addr+=(DISPLAY_WIDTH/16)) {
      for( i=0; i<cols; i++ ) {
         *(addr+i) = *ptr++;
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
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_Sprite6( const uint8_t *sprite, uint16_t x, uint16_t y, uint16_t h )
{
#ifndef _LITTLE_ENDIAN
   uint16_t *addr=(uint16_t*)Vmem+(DISPLAY_WIDTH/16)*y+(x>>4);
   int16_t cnt1;
   uint16_t cnt2;
   uint16_t data;

   cnt1 = 8-(x&15);
   cnt2 = 16-((-1)*cnt1);

   for(;h--;addr+=(DISPLAY_WIDTH/16)) {
      data = *sprite++;
      *addr &= ~((0xFCUL)<<(cnt1)) ; // on efface
      *addr |= (data&0xFC)<<(cnt1);
      // protection effet de bord
      if ( x < (DISPLAY_WIDTH-16) ) {
         *(addr+1) &= ~((0xFCUL)<<(cnt2)); // on efface
         *(addr+1) |= (data&0xFC)<<(cnt2);
      }
   }
#else
  uint32_t *addr=Vmem+(DISPLAY_WIDTH/32)*y+(x/32);

   int16_t cnt1;
   uint16_t cnt2;
   uint8_t data;

   cnt1 = 24-(x&31);
   cnt2 = 32-((-1)*cnt1);
   
   for(;h--;addr+=(DISPLAY_WIDTH/32)) {
      data = *sprite++;
      
      if( cnt1 >= 0 ) {
        *addr &= ~((0xFCUL)<<(cnt1)); // on efface
        *addr |= (data&0xFC)<<(cnt1);
      } else {
        *addr &= ~((0xFCUL)>>(-1*cnt1)); // on efface
        *addr |= (data&0xFC)>>(-1*cnt1);
      }    
      // protection effet de bord
      if ( x < (DISPLAY_WIDTH-32) && cnt1<0 ) {
         *(addr+1) &= ~((0xFCUL)<<(cnt2)); // on efface
         *(addr+1) |= (data&0xFC)<<(cnt2);           
      }
   }
   
#endif
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_Sprite16
 *------------------------------------------------------------------------*/
/** \brief Affiche un Sprite de largeur 16px et de hauteur h pixels
 *
 * \param const uint16_t *sprite : pointeur vers le sprite à afficher
 * \param uint16_t x : abscisse du coin supérieur gauche
 * \param uint16_t y : ordonnée du coin supérieur gauche
 * \param uint16_t h : nombre de lignes du sprite
 *
 * \retval -
 *
 * \remarks Utilisé pour les pictos en bas de l'Enerium
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_Sprite16( const uint16_t *sprite, uint16_t x, uint16_t y, uint16_t h )
{
   uint16_t *addr=(uint16_t*)Vmem+(DISPLAY_WIDTH/16)*y+(x>>4);
   uint16_t cnt1, cnt2;
   uint16_t data;

   cnt1 = (x&15);
   cnt2 = 16-cnt1;

   for(;h--;addr+=(DISPLAY_WIDTH/16)) {
      data = *sprite++;
      *addr &= (~(0xFFFFUL))>>(cnt1) ; // on efface
      *addr |= (data)>>(cnt1);
      *(addr+1) &= (~(0xFFFFUL))<<(cnt2); // on efface
      *(addr+1) |= (data)<<(cnt2);
   }
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_Sprite40
 *------------------------------------------------------------------------*/
/** \brief Affiche un Sprite de largeur 40px et de hauteur h pixels
 *
 * \param const uint16_t *sprite : pointeur vers le sprite à afficher
 * \param uint16_t x : abscisse du coin supérieur gauche
 * \param uint16_t y : ordonnée du coin supérieur gauche
 * \param uint16_t h : nombre de lignes du sprite
 *
 * \retval -
 *
 * \remarks Utilisé pour les pictos du menu principal de l'Enerium
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_Sprite40( const uint16_t *sprite, uint16_t x, uint16_t y, uint16_t h )
{
#ifndef _LITTLE_ENDIAN
   uint16_t *addr;
   uint16_t cnt1, cnt2;

   addr= (uint16_t*)Vmem+(DISPLAY_WIDTH/16)*y+(x>>4);
   cnt1 = 16-(x&15);
   cnt2 = 16-cnt1;

   for(;h--; addr+= (DISPLAY_WIDTH/16), sprite+=3 ) {
      *addr = ((uint16_t)sprite[0])>>(cnt2);
      *(addr+1) = ((uint16_t)sprite[0])<<(cnt1) | ((uint16_t)sprite[1])>>(cnt2);
      *(addr+2) = ((uint16_t)sprite[1])<<(cnt1) | ((uint16_t)sprite[2])>>(cnt2);
      *(addr+3) = ((uint16_t)sprite[2])<<(cnt1);
   }
#else
   uint32_t *addr = Vmem+(DISPLAY_WIDTH/32)*y+(x/32);
   uint32_t cnt1, cnt2;
   uint32_t mot1;
  
   cnt1 = 32-(x&31);
   cnt2 = 32-cnt1;

   for(;h--; addr+= (DISPLAY_WIDTH/32), sprite+=3 ) {
      mot1 = ((uint32_t)sprite[0])<<(16) | ((uint32_t)sprite[1]);
      *(addr) |= mot1>>cnt2;
      *(addr+1) = (mot1<<cnt1) | (((uint32_t)sprite[2])<<16)>>cnt2;  
   }
#endif
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
void EGL_SetPixel( uint16_t x, uint16_t y, uint16_t value )
{
   uint32_t *addr;
   uint32_t bitmask;

   // protection
   x = (x>=DISPLAY_WIDTH) ? DISPLAY_WIDTH-1 : x;
   y = (y>=DISPLAY_HEIGHT) ? DISPLAY_HEIGHT-1 : y;

   addr=Vmem+(DISPLAY_WIDTH/32)*y+(x>>5);
   bitmask = 0x80000000UL>>(x%32);
   if (value) {
      *addr |= bitmask; // set
   } else {
      *addr &= ~bitmask; // clear
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
uint16_t EGL_GetPixel( uint16_t x, uint16_t y )
{
   uint32_t *addr;
   uint16_t bit = (x%32);
   uint32_t bitmask;

   // protection
   x = (x>=DISPLAY_WIDTH) ? DISPLAY_WIDTH-1 : x;
   y = (y>=DISPLAY_HEIGHT) ? DISPLAY_HEIGHT-1 : y;

   addr=Vmem+(DISPLAY_WIDTH/32)*y+(x>>5);
   bitmask = 0x80000000UL>>(bit);
   return( (uint16_t)(((*addr)&bitmask)>>(31-bit)));
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_DrawLine
 *------------------------------------------------------------------------*/
/** \brief Affiche une ligne quelconque en utilisant l'algorithme de Bresenham
 *
 * \param uint16_t x1,y1 : coordonnées x,y du point de départ de la ligne
 * \param uint16_t x2,y2 : coordonnéex x,y du point final de la ligne
 * \param uint16_t pattern :  0=clear 1=set
 *
 * \retval -
 *
 * \remarks Eviter d'utiliser cet algorithme pour dessiner des traits
 *          verticaux ou horizontaux. Voir plutôt LCD_Hline() et LCD_Vline().
 *
 *          Pour une pente positive, le point de départ est le point le plus bas
 *          Pour une pente négative, le point de départ est le point le plus haut
 *
 * Exemple :   LCD_DrawLine( 0, 0 , 159, 127, 1  ); // trace une diagonale sur un écran 160x128
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_DrawLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t pattern )
{
   int16_t dy;
   int16_t dx;
   int16_t stepx, stepy;

   // protection
   x1 = (x1>=DISPLAY_WIDTH) ? DISPLAY_WIDTH-1 : x1;
   x2 = (x2>=DISPLAY_WIDTH) ? DISPLAY_WIDTH-1 : x2;
   y1 = (y1>=DISPLAY_HEIGHT) ? DISPLAY_HEIGHT-1 : y1;
   y2 = (y2>=DISPLAY_HEIGHT) ? DISPLAY_HEIGHT-1 : y2;

   dy = y2 - y1;
   dx = x2 - x1;

   if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
   if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
   dy <<= 1;      // dy is now 2*dy
   dx <<= 1;      // dx is now 2*dx

   EGL_SetPixel( x1, y1, pattern );

   if (dx > dy) {
      int16_t fraction = dy - (dx >> 1); // same as 2*dy - dx
      while (x1 != x2) {
          if (fraction >= 0) {
              y1 += stepy;
              fraction -= dx;  // same as fraction -= 2*dx
          }
          x1 += stepx;
          fraction += dy;  // same as fraction -= 2*dy

         EGL_SetPixel( x1, y1, pattern );
      }
   } else {
      int16_t fraction = dx - (dy >> 1);
      while (y1 != y2) {
          if (fraction >= 0) {
              x1 += stepx;
              fraction -= dy;
          }
          y1 += stepy;
          fraction += dx;

          EGL_SetPixel( x1, y1, pattern );
      }
   }
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_DrawCircle
 *------------------------------------------------------------------------*/
/** \brief Dessine un cercle
 *
 * \param uint8_t xCenter, uint8_t yCenter : coordonnées du centre du cercle
 * \param uint8_t r : rayon du cercle en pixels
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_DrawCircle( uint8_t xCenter, uint8_t yCenter, uint8_t r )
{
	int16_t x = 0, y = r;
	int16_t d = 3-(2*r);

    while(x<=y){
		EGL_SetPixel(xCenter+x,yCenter+y,1);
		EGL_SetPixel(xCenter+y,yCenter+x,1);
		EGL_SetPixel(xCenter-x,yCenter+y,1);
		EGL_SetPixel(xCenter+y,yCenter-x,1);
		EGL_SetPixel(xCenter-x,yCenter-y,1);
		EGL_SetPixel(xCenter-y,yCenter-x,1);
		EGL_SetPixel(xCenter+x,yCenter-y,1);
		EGL_SetPixel(xCenter-y,yCenter+x,1);

		if (d<0) { 
			d += (4*x)+6;
		} else {
			d += (4*(x-y))+10;
			y -= 1;
		}
		x++;
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
void EGL_DrawRectangle( uint16_t x, uint16_t y, uint16_t h, uint16_t l )
{
   EGL_HLine( x, y, l, 1 );
   EGL_HLine( x, y-h+1, l, 1 );
   EGL_VLine( x, y-h+1, h, 1 );
   EGL_VLine( x+l-1, y-h+1, h, 1 );
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
void EGL_HLine( uint16_t x, uint16_t y, uint16_t len, uint16_t value )
{
   uint16_t i;
   uint16_t color = value;

   for( i=x; i<(x+len); i++ ) {
      if (value == 2 ) {
         color = (~EGL_GetPixel(i,y))&0x1;
      }
      EGL_SetPixel(i,y,color);
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
void EGL_VLine( uint16_t x, uint16_t y, uint16_t len, uint16_t value )
{
   uint16_t i;
   uint16_t color = value;

   for( i=y; i<(y+len); i++ ) {
      if (value == 2 ) {
         color = (~EGL_GetPixel(x,i))&0x1;
      }
      EGL_SetPixel(x,i,color);
   }
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_RefreshBargraph
 *------------------------------------------------------------------------*/
/** \brief Affiche un Bargraph vertical
 *
 * \param BARGRAPH_STRUCT *b : structure de type bargraph
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_RefreshBargraph( BARGRAPH_STRUCT *b )
{
   uint16_t i;
   
   for ( i=(b->x+1); i<(b->x+b->l-1); i++ ) {
      EGL_VLine( i, b->y-b->h+2, b->h-2, 0 ); // on efface l'intérieur du rectangle
      EGL_VLine( i, b->val+1, b->y-b->val-1, 1 ); // on remplit le rectangle
   } 
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_RefreshSlider
 *------------------------------------------------------------------------*/
/** \brief Rafraichi le slider
 *
 * \param uint16_t index : index de la ligne où afficher le slider
 * \param uint16_t val : valeur courante du slider [0..50]
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_RefreshSlider(uint16_t index, uint16_t val)
{
   if (val < 51) {
      EGL_HLine( 103, (8*index)+27, val, 1 );
      EGL_HLine( 103, (8*index)+28, val, 1 );
      // efface la fin
      EGL_HLine( 103+val, (8*index)+27, 51-val, 0 );
      EGL_HLine( 103+val, (8*index)+28, 51-val, 0 );
   }
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_Slider
 *------------------------------------------------------------------------*/
/** \brief Affiche un bargraph horizontal (slider)
 *
 * \param uint16_t index : index de la ligne où afficher le slider
 * \param uint16_t val : valeur courante du slider [0..50]
 * \param bool arrow : true pour afficher des petites flèches 
 *
 * \retval -
 *
 * \remarks Aspect général :
 *     _______________
 *  <-| =========     |->
 *     ---------------
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_Slider(uint16_t index, uint16_t val, bool arrow)
{
   uint16_t onoff;

   EGL_HLine( 102, (8*index)+25, 53, 1 );
   EGL_HLine( 102, (8*index)+30, 53, 1 );
   EGL_VLine( 101, (8*index)+26, 4, 1 );
   EGL_VLine( 155, (8*index)+26, 4, 1 );

   EGL_RefreshSlider(index, val);

   if( arrow == true ) {
      onoff = 1;
   } else {
      onoff = 0;
   }
   EGL_VLine( 99, (8*index)+25, 6, onoff );
   EGL_VLine( 98, (8*index)+26, 4, onoff );
   EGL_VLine( 97, (8*index)+27, 2, onoff );
   EGL_VLine( 157, (8*index)+25, 6, onoff );
   EGL_VLine( 158, (8*index)+26, 4, onoff );
   EGL_VLine( 159, (8*index)+27, 2, onoff );
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
void EGL_PutChar( uint8_t c, uint16_t x, uint16_t y, CHAR_TYPE c_type )
{
   const uint8_t *ptr;

   if ( x > (DISPLAY_WIDTH-6) ) {
      return;
   } 
   if ( y > (DISPLAY_HEIGHT-8) ) {
      return;
   }
   
   c &= 0xFF;
   if ( c_type == CHAR_ASCII ) {
      c -= 0x20;
   }
   ptr = character_map+8*c;
   EGL_Sprite6( ptr, x, y, 8 );
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
void EGL_PutString( const uint8_t *s, uint16_t x, uint16_t y )
{
   uint8_t c;
   while ( *s ) {
      c = (uint8_t)*s++;
      // petite protection pour éviter d'aller écrire d'importe où
      if ( x>154 || y>120 ) {
         return;
      } 
      EGL_PutChar(c,x,y,CHAR_ASCII);
      x+=6;
   }
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_PutBigChar
 *------------------------------------------------------------------------*/
/** \brief Affiche un chiffre avec la police 18x24
 *
 * \param uint8_t c : le chiffre en codage spécial
 * \param x, y : colonne, ligne du début de la chaîne
 *
 * \retval -
 *
 * \remarks 0 < c <= 9 : affiche le digit correspondant
 *          c == 10 : affiche un espace
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_PutBigChar( uint8_t c, uint8_t x, uint8_t y )
{
   uint8_t offset, i, j;

   if ( c > 10 ) { 
      return;
   }

   if( c == 10 ) {
      c = 0;         // espace
      offset = 0;
   } else {
      c = 0x83+c*9;  // 3x3
      offset = 1;
   }

   for( i=0; i<24; i+=8 )  {
      for( j=0; j<18; j+=6 ) {
         EGL_PutChar( c, x+j, y+i, CHAR_BYTE);
         c += offset;
      }
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
uint16_t EGL_PutNumber( int32_t num, uint16_t x, uint16_t y, char *conv_specs )
{
   char buffer[DISPLAY_WIDTH/6+1]; // +1 pour le '\0'
   uint16_t len;

   len = sys_snprintf(buffer,sizeof(buffer),conv_specs,num);
   EGL_PutString((uint8_t*)buffer,x,y);
   return(len);
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_PutFixPoint
 *------------------------------------------------------------------------*/
/** \brief Affiche un nombre à virgule fixe en police 6x8
 *
 * \param FIXPOINT_NUMBER *num_struct : paramètres du nombre à afficher
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
CALIBRE EGL_PutFixPoint( FIXPOINT_NUMBER *num_struct )
{
   char buffer[FIXPOINT_BUFFER_SIZE];
   CALIBRE cal;
   uint16_t x, y, i, digits, debut;
   uint8_t prec = num_struct->prec;

   digits = EGL_IntToStr( buffer, num_struct );

   cal = CAL_SANS;
   debut = 0;
   if( digits > num_struct->digits_max ) {
      i = digits - prec;
      if ( i > 9 ) {
         cal = CAL_GIGA;
      } else if( i > 6 ) {
         cal = CAL_MEGA;
      } else if( i > 3 ) {
         cal = CAL_KILO;
      } else {
         cal = CAL_SANS;
      }
      if( cal ) {
         prec = prec+3*cal;
      }
      debut = digits-num_struct->digits_max;

   } else if( prec >= num_struct->digits_max ) {
      debut = prec-num_struct->digits_max+1;
   }

   x = num_struct->x;
   y = num_struct->y;

   // si la virgule est affichée, on la compte comme un digit
   if ( prec>debut && digits>=num_struct->digits_max ) {
      debut++;
   } 
   
   for( i=debut; i<(debut+num_struct->digits_max); i++ ) {
      if ( i == prec && i!=debut ){
         EGL_PutChar( '.', x, y, CHAR_ASCII );
         x -= 6;
      }
      // On affiche le chiffre
      if ( buffer[i] == 10 ) {
         EGL_PutChar( 0, x, y, CHAR_BYTE );
      } else {
         EGL_PutChar( buffer[i]+0x10, x, y, CHAR_BYTE );
      } 
      x -= 6;
   }
   return(cal);
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_PutBigNumber
 *------------------------------------------------------------------------*/
/** \brief Affiche un nombre dans la police 18x24
 *
 * \param FIXPOINT_NUMBER *num_struct : paramètres du nombre à afficher
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
CALIBRE EGL_PutBigNumber( FIXPOINT_NUMBER *num_struct )
{
   char buffer[FIXPOINT_BUFFER_SIZE];
   CALIBRE cal;
   uint16_t x, y, i, digits, debut;
   uint8_t prec = num_struct->prec;

   digits = EGL_IntToStr( buffer, num_struct );

   cal = CAL_SANS;
   debut = 0;
   if( digits > num_struct->digits_max ) {
      i = digits - prec;
      if ( i > 9 ) {
         cal = CAL_GIGA;
      } else if( i > 6 ) {
         cal = CAL_MEGA;
      } else if( i > 3 ) {
         cal = CAL_KILO;
      } else {
         cal = CAL_SANS;
      }
      if( cal ) {
         prec = prec+3*cal;
      }
      debut = digits-num_struct->digits_max;

   } else if( prec >= num_struct->digits_max ) {
      debut = prec-num_struct->digits_max+1;
   }

   x = num_struct->x;
   y = num_struct->y;

   for( i=debut; i<(debut+num_struct->digits_max); i++ ) {
      if ( i == prec && i!=debut ){
         x -= 6;
         EGL_PutChar( ' ', x, y, CHAR_ASCII );
         EGL_PutChar( ' ', x, y+8, CHAR_ASCII );
         EGL_PutChar( ID_VIRGULE, x, y+16, CHAR_BYTE );
      }
      // le signe éventuel
      if ( (i==digits && num_struct->mode==BLANK_PADDING) || (i==num_struct->digits_max && num_struct->mode==ZERO_PADDING) ) {
         if( buffer[FIXPOINT_BUFFER_SIZE-1] == '-' ) {
            x -= 6;
            EGL_PutChar( ' ', x, y, CHAR_ASCII );
            EGL_PutChar( ID_MOINS, x, y+8, CHAR_BYTE );
            EGL_PutChar( ' ', x, y+16, CHAR_ASCII );
         }
      }
      x -= 18;
      // On affiche le chiffre
      EGL_PutBigChar( buffer[i], x, y );
   }
   
   // le signe éventuel
   if ( (i==digits && num_struct->mode==BLANK_PADDING) || (i==num_struct->digits_max && num_struct->mode==ZERO_PADDING) ) {
      if( buffer[FIXPOINT_BUFFER_SIZE-1] == '-' ) {
         x -= 6;
         EGL_PutChar( ' ', x, y, CHAR_ASCII );
         EGL_PutChar( ID_MOINS, x, y+8, CHAR_BYTE );
         EGL_PutChar( ' ', x, y+16, CHAR_ASCII );
      }
   }
   
   
   // Si aucune virgule n'est affichée, on efface quand même une colonne
   // (pour effacer une éventuelle trace du nombre précédemment affiché)
   if ( !prec ) {
      x -=6;
      EGL_PutChar( ' ', x, y, CHAR_ASCII );
      EGL_PutChar( ' ', x, y+8, CHAR_ASCII );
      EGL_PutChar( ' ', x, y+16, CHAR_ASCII );
   }
   // pareil mais pour le signe '+'
   if( buffer[FIXPOINT_BUFFER_SIZE-1] == '+' ) {
       x -= 6;
       EGL_PutChar( ' ', x, y, CHAR_ASCII );
       EGL_PutChar( ' ', x, y+8, CHAR_ASCII );
       EGL_PutChar( ' ', x, y+16, CHAR_ASCII );
   }
   return(cal);
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_IntToStr
 *------------------------------------------------------------------------*/
/** \brief Convertion d'un entier en chaîne de caractères
 *
 * \param  char *s_nbr : buffer où stocker le nombre converti
 * \param FIXPOINT_NUMBER *num_struct : paramètres du nombre à afficher
 *
 * \retval uint8_t : nombre de digits "utiles" à l'affichage
 *
 * \remarks Le résultat est présenté comme cela :
 *
 * Nombre : -46266 (précision = 1/100, padding = espace)
 * index du buffer   0123456789
 * buffer            66264AAAA-
 *
 * Nombre : 46266 (précision = 1/100, padding = zéros)
 * index du buffer   0123456789
 * buffer            662640000+
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
uint8_t EGL_IntToStr( char *s_nbr, FIXPOINT_NUMBER *num_struct )
{
   uint32_t n;
   uint32_t n10;
   uint16_t i = 0;
   uint8_t raz;
   uint8_t digits = 0;
   uint8_t flag=0;

   s_nbr[FIXPOINT_BUFFER_SIZE-1] = '+';
   if ( num_struct->signed_val == true ) {
      if ( ((int32_t)num_struct->val)<0 ) {
         num_struct->val = -((int32_t)num_struct->val);
         s_nbr[FIXPOINT_BUFFER_SIZE-1] = '-';
      }
   }
   n = num_struct->val;
   n10 = n/10;

   if ( num_struct->mode == BLANK_PADDING ) {
      raz = 0xA;
   } else {
      raz = 0;
   }

   do {
     s_nbr[i++] = (n - ((n10<<3) + (n10<<1))), n = n10, n10 /= 10;
   } while(n > 0);
   digits = i;
   for( ; i<(FIXPOINT_BUFFER_SIZE-1); i++ ) {
      if( i<=num_struct->prec ) {
            s_nbr[i] = 0;
         digits++;
      } else {
         s_nbr[i] = raz;
      }
   }

   // Maintenant on arrondit le nombre si besoin (nombre tronqué par l'affichage)
   // on regarde la valeur du premier digit hors écran
   if ( digits > num_struct->digits_max ) {
     n = digits-num_struct->digits_max-1;
   } else  {
     return(digits);
   }

   i = s_nbr[n];
   n++;
   if ( i>4 ) {
      s_nbr[n]++;
   }

   // Propagation de la retenue ?
   for( i=n; i<digits; i++ ) {
      if (flag) {
        s_nbr[i]++;
      }

      if( s_nbr[i]==10 ) {
        s_nbr[i]=0;
        flag=1;
        if( i+1 == digits ) {// aïe, on arrive au bout, il faut donc augmenter le nombre de digits
          digits++;
          s_nbr[i+1] = 0;
        }
      } else {
        break; // on termine la propagation de la retenue
      }
   }

   return(digits);
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_StrLen
 *------------------------------------------------------------------------*/
/** \brief Calcule la longueur d'une chaîne sans les espaces au bout
 *
 * \param const uint8_t *str : pointeur sur la chaîne
 * \param uint8_t taille : taille totale de la chaîne
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
uint8_t EGL_StrLen( const uint8_t *str, uint8_t taille )
{
   while( str[--taille]==0x20 && taille );
   return (taille+1);
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_Surbrillance
 *------------------------------------------------------------------------*/
/** \brief Met la sélection courante en inverse vidéo
 *
 * \param uint16_t index : position actuelle de la sélection
 * \param uint16_t index_prec : position précédente de la sélection
 * \param uint8_t *tailles : tableau de tailles (en px) pour chaque ligne
 * \param uint16_t x : début abscisse de la surbrillance
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_Surbrillance( uint16_t index, uint16_t index_prec, uint8_t *tailles, uint16_t x )
{
   uint16_t y;

   // Si besoin, on désélectionne l'élément précédent du sous menu
   if ( index != index_prec ) {
      for ( y=(24+8*index_prec-1); y<(24+8*(index_prec+1)); y++ ) {
         EGL_HLine( x, y, tailles[index_prec],2 );
      }
   }
   // On met en surbrillance l'élément courant du sous menu
   for ( y=(24+8*index-1); y<(24+8*(index+1)); y++ ) {
      EGL_HLine( x, y, tailles[index],2 );
   }
}


/*--------------------------------------------------------------------------
 * Fonction    :  EGL_Curseur
 *------------------------------------------------------------------------*/
/** \brief Met un caractère (6 pixels de large) en inverse vidéo
 *
 * \param uint16_t x_debut : coordonnée abscisse du nombre affiché (digit le plus à gauche)
 * \param uint16_t ligne : ligne y du texte (ordonnée x8)
 * \param uint16_t digit : digit courant
 * \param uint16_t digit_prec : digit précédent
 *
 * \retval -
 *
 * \remarks -
 *
 * \see -
 */
/*------------------------------------------------------------------------*/
void EGL_Curseur( uint16_t x_debut, uint16_t ligne, uint16_t digit, uint16_t digit_prec )
{
   uint16_t y;

   // Si besoin, on efface la position précédente du curseur
   if ( digit != digit_prec ) {
      for ( y=(24+8*ligne-1); y<(24+8*(ligne+1)); y++ ) {
         EGL_HLine( x_debut+6*digit_prec, y, 7, 2 );
      }
   }
   // On met en surbrillance l'élément courant du sous menu
   for ( y=(24+8*ligne-1); y<(24+8*(ligne+1)); y++ ) {
      EGL_HLine( x_debut+6*digit, y, 7, 2 );
   }

}


/*--------------------------------------------------------------------------
                             FIN FICHIER
--------------------------------------------------------------------------*/
/** @} */

