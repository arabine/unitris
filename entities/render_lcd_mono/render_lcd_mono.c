#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "egl.h"
#include "render_lcd_mono.h"

const uint16_t tetris[] = {
   0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 
   0xc000, 0000, 0000, 0000, 0000, 0x0003, 
   0xbfff, 0xffff, 0xffff, 0xffff, 0xffff, 0xfffd, 
   0xb000, 0000, 0000, 0000, 0000, 0x000d, 
   0xb000, 0000, 0000, 0000, 0000, 0x000d, 
   0xb000, 0000, 0000, 0000, 0000, 0x000d, 
   0xb000, 0000, 0000, 0000, 0000, 0x000d, 
   0xb3df, 0x7bef, 0xef7d, 0xefb8, 0x3e2f, 0xeead, 
   0xb253, 0x5a28, 0x694d, 0x68a6, 0x2228, 0x64ed, 
   0xb293, 0x1a24, 0x6a4c, 0x68a2, 0x22b4, 0x64ad, 
   0xb293, 0x3a22, 0x6a4d, 0xe8a1, 0x2292, 0x600d, 
   0xb31f, 0x3be1, 0xec7c, 0xefbf, 0x3efb, 0xe00d, 
   0xb31f, 0x1be0, 0x6c7c, 0x6fbf, 0x3efd, 0xe00d, 
   0xb21f, 0x1be1, 0x287c, 0x6fbf, 0x3e7c, 0xe00d, 
   0xb01f, 0x0be3, 0x007c, 0x2fbf, 0x3e7e, 0x600d, 
   0xb01f, 0x03e7, 0x007c, 0x0fbe, 0x3e7f, 0x600d, 
   0xb013, 0x0229, 0x004c, 0x089c, 0x2221, 0xa00d, 
   0xb013, 0x022b, 0x004c, 0x08b0, 0x2210, 0x800d, 
   0xb013, 0x0225, 0x004c, 0x08a0, 0x2248, 0x600d, 
   0xb013, 0x0223, 0x004c, 0x0990, 0x2268, 0x600d, 
   0xb013, 0x0221, 0x604c, 0x0888, 0x2254, 0x200d, 
   0xb01f, 0x03e0, 0xe07c, 0x0fb8, 0x3e73, 0xf00d, 
   0xb01f, 0x03e1, 0xe07c, 0x0fbc, 0x3e7b, 0xf00d, 
   0xb01f, 0x03e3, 0xe07c, 0x0fbe, 0x3e7d, 0xf00d, 
   0xb01f, 0x03e7, 0xe07c, 0x0fbe, 0x3e7c, 0xf00d, 
   0xb01f, 0x03ef, 0xe07c, 0x0fbf, 0xbe7e, 0x600d, 
   0xb01f, 0x03ef, 0xe07c, 0x0fbf, 0xbe7e, 0xc00d, 
   0xb000, 0000, 0000, 0000, 0000, 0x000d, 
   0xb000, 0000, 0000, 0000, 0000, 0x000d, 
   0xb000, 0000, 0000, 0000, 0000, 0x000d, 
   0xbfff, 0xffff, 0xffff, 0xffff, 0xffff, 0xfffd, 
   0x8000, 0000, 0000, 0000, 0000, 0x0001, 
   0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 
   0000, 0000, 0000, 0000, 0000, 0000, 
   0000, 0000, 0000, 0000, 0000, 0000, 
   0x1fff, 0xffff, 0xffff, 0xffff, 0xe000, 0000, 
   0x1fff, 0xffff, 0xffff, 0xffff, 0xe000, 0000, 
   0x7000, 0000, 0000, 0000, 0x3000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x6000, 0000, 0000, 0000, 0x1000, 0000, 
   0x7000, 0000, 0000, 0000, 0x3000, 0000, 
   0x1fff, 0xffff, 0xffff, 0xffff, 0xe000, 0000, 
   0000, 0000, 0000, 0000, 0000, 0000, 
   0000, 0000, 0000, 0000, 0000, 0000, 
   0x1fff, 0xffff, 0xfff8, 0x1fff, 0xffff, 0xfffc, 
   0x1fff, 0xffff, 0xfff8, 0x1fff, 0xffff, 0xfffc, 
   0x7000, 0000, 0x000c, 0x7000, 0000, 0x0006, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x6000, 0000, 0x0004, 0x6000, 0000, 0x0002, 
   0x7000, 0000, 0x000c, 0x7000, 0000, 0x0006, 
   0x1fff, 0xffff, 0xfff8, 0x1fff, 0xffff, 0xfffc, 
   0000, 0000, 0000, 0000, 0000, 0000, 
};
/*
// Affiche un carré, la base de toutes les pièces
void AFF_PrintBase(int x, int y, int val)
{
   int i;

   // transformation de coordonnée matricielle en coordonnée LCD
   x = REAL_X_BASE + (x-1)*4;
   y = REAL_Y_BASE + y*4;
   
   for( i=0; i<4; i++ ) {
      EGL_SetPixel(x+i,y,val);
      EGL_SetPixel(x,y+i,val);
      EGL_SetPixel(x+i,y+3,val);
      EGL_SetPixel(x+3,y+i,val);
   }
}


// Affiche la pièce suivante
void AFF_NextPiece(PIECE *p)
{
   int i, x, y, val, k, n;
   int *ptr = &pieces[p->type*16*4];

   for( k=0; k<4; k++ ) {
      for( n=0; n<4; n++ ) {
         if(ptr[4*k+n]) {
            val = 1;
         } else {
            val = 0;
         }
         // transformation de coordonnée matricielle en coordonnée LCD
         x = 75 + n*4;
         y = 60 + k*4;
         
         // Affichage du carré (ou effacement)
         for( i=0; i<4; i++ ) {
            EGL_SetPixel(x+i,y,val);
            EGL_SetPixel(x,y+i,val);
            EGL_SetPixel(x+i,y+3,val);
            EGL_SetPixel(x+3,y+i,val);
         }
      }
   }
}

// Met à jour l'affichage
// optimisation : on ne modifie que les carrés différents depuis la précédente update
void AFF_Update(void)
{
   int x, y, val;

   for(y=0; y<20; y++ ) {
      for(x=1; x<11; x++ ) {
         if( grille[y][x] != grille_mem[y][x] ) {
            grille_mem[y][x] = grille[y][x];
            if( grille_mem[y][x] ) {
               val = 1;
            } else {
               val = 0;
            }
            AFF_PrintBase(x,y,val);
         }
      }
   }
}



bool AFF_UpdateTetris(uint32_t event)
{

	EGL_PutString("Tetris - PAUSE -         ",0,0);
	EGL_PutString("Tetris !                 ",0,0);

            if( level == 9 ) {
               EGL_PutString("YOUPI!", 110, 64);
			}
        AFF_NextPiece(&p_next);

        EGL_PutString("Perdu!", 110, 64);

      EGL_PutNumber(lines, 9, 92, "%ld");
      EGL_PutNumber(level+1, 58, 92, "%ld");
      EGL_PutNumber(score, 4, 69, "%ld");
   }
   AFF_Update();
   return(true);
}

*/

void LCD_Tick(render_lcd_mono_t *lcd)
{
    UNI_Tick(&lcd->engine, 0, 0, 0);
}

// Init des graphismes et du jeu
void LCD_Initialize(render_lcd_mono_t *lcd)
{
   int i, j;

   UNI_Initialize(&lcd->engine);

   EGL_Initialize(&lcd->egl, DISPLAY_WIDTH, DISPLAY_HEIGHT, &lcd->Vmem[0]);
   EGL_ClearScreen(&lcd->egl);
   EGL_PutString(&lcd->egl, "Tetris !                 ", 0, 0);

   /*
   EGL_DrawRectangle(REAL_X_BASE-1, REAL_Y_BASE+HAUTEUR_GRILLE, HAUTEUR_GRILLE+2, LARGEUR_GRILLE+2);
   
   EGL_CopyImage16(tetris, 0, 23, 6, 80);
   EGL_PutString("Score", 4, 61);
   EGL_PutString("Lines", 9, 84);
   EGL_PutString("Level", 58, 84);


   // init variables
   for (i=0; i<21; i++) {
      for (j=0; j<12; j++) {
         if( j==0 || j==11 || i==20 ) {
            grille[i][j] = 1;
            grille_mem[i][j] = 1;
         } else {
            grille[i][j] = 0;
            grille_mem[i][j] = 0;
         }
      }
   }


   AFF_NextPiece(&p_next);
   AFF_Update();
    */
}

void LCD_RenderBitmap(render_lcd_mono_t *lcd)
{
    BMP_Generate(lcd->Vmem, lcd->egl.display_width, lcd->egl.display_height, lcd->bitmap);
}

