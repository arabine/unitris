#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "egl.h"
#include "render_lcd_mono.h"

static const uint8_t tetris[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xc0, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x03,
   0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd,
   0xb0, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x0d,
   0xb0, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x0d,
   0xb0, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x0d,
   0xb0, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x0d,
   0xb3, 0xdf, 0x7b, 0xef, 0xef, 0x7d, 0xef, 0xb8, 0x3e, 0x2f, 0xee, 0xad,
   0xb2, 0x53, 0x5a, 0x28, 0x69, 0x4d, 0x68, 0xa6, 0x22, 0x28, 0x64, 0xed,
   0xb2, 0x93, 0x1a, 0x24, 0x6a, 0x4c, 0x68, 0xa2, 0x22, 0xb4, 0x64, 0xad,
   0xb2, 0x93, 0x3a, 0x22, 0x6a, 0x4d, 0xe8, 0xa1, 0x22, 0x92, 0x60, 0x0d,
   0xb3, 0x1f, 0x3b, 0xe1, 0xec, 0x7c, 0xef, 0xbf, 0x3e, 0xfb, 0xe0, 0x0d,
   0xb3, 0x1f, 0x1b, 0xe0, 0x6c, 0x7c, 0x6f, 0xbf, 0x3e, 0xfd, 0xe0, 0x0d,
   0xb2, 0x1f, 0x1b, 0xe1, 0x28, 0x7c, 0x6f, 0xbf, 0x3e, 0x7c, 0xe0, 0x0d,
   0xb0, 0x1f, 0x0b, 0xe3, 00, 0x7c, 0x2f, 0xbf, 0x3e, 0x7e, 0x60, 0x0d,
   0xb0, 0x1f, 0x03, 0xe7, 00, 0x7c, 0x0f, 0xbe, 0x3e, 0x7f, 0x60, 0x0d,
   0xb0, 0x13, 0x02, 0x29, 00, 0x4c, 0x08, 0x9c, 0x22, 0x21, 0xa0, 0x0d,
   0xb0, 0x13, 0x02, 0x2b, 00, 0x4c, 0x08, 0xb0, 0x22, 0x10, 0x80, 0x0d,
   0xb0, 0x13, 0x02, 0x25, 00, 0x4c, 0x08, 0xa0, 0x22, 0x48, 0x60, 0x0d,
   0xb0, 0x13, 0x02, 0x23, 00, 0x4c, 0x09, 0x90, 0x22, 0x68, 0x60, 0x0d,
   0xb0, 0x13, 0x02, 0x21, 0x60, 0x4c, 0x08, 0x88, 0x22, 0x54, 0x20, 0x0d,
   0xb0, 0x1f, 0x03, 0xe0, 0xe0, 0x7c, 0x0f, 0xb8, 0x3e, 0x73, 0xf0, 0x0d,
   0xb0, 0x1f, 0x03, 0xe1, 0xe0, 0x7c, 0x0f, 0xbc, 0x3e, 0x7b, 0xf0, 0x0d,
   0xb0, 0x1f, 0x03, 0xe3, 0xe0, 0x7c, 0x0f, 0xbe, 0x3e, 0x7d, 0xf0, 0x0d,
   0xb0, 0x1f, 0x03, 0xe7, 0xe0, 0x7c, 0x0f, 0xbe, 0x3e, 0x7c, 0xf0, 0x0d,
   0xb0, 0x1f, 0x03, 0xef, 0xe0, 0x7c, 0x0f, 0xbf, 0xbe, 0x7e, 0x60, 0x0d,
   0xb0, 0x1f, 0x03, 0xef, 0xe0, 0x7c, 0x0f, 0xbf, 0xbe, 0x7e, 0xc0, 0x0d,
   0xb0, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x0d,
   0xb0, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x0d,
   0xb0, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x0d,
   0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd,
   0x80, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x01,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
   00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
   0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 00, 00, 00,
   0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 00, 00, 00,
   0x70, 00, 00, 00, 00, 00, 00, 00, 0x30, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x60, 00, 00, 00, 00, 00, 00, 00, 0x10, 00, 00, 00,
   0x70, 00, 00, 00, 00, 00, 00, 00, 0x30, 00, 00, 00,
   0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 00, 00, 00,
   00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
   00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
   0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfc,
   0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfc,
   0x70, 00, 00, 00, 00, 0x0c, 0x70, 00, 00, 00, 00, 0x06,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x60, 00, 00, 00, 00, 0x04, 0x60, 00, 00, 00, 00, 0x02,
   0x70, 00, 00, 00, 00, 0x0c, 0x70, 00, 00, 00, 00, 0x06,
   0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfc,
   00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
};

// Affiche un carré, la base de toutes les pièces
static void LCD_PrintBaseSquare(egl_t *egl, uint16_t x, uint16_t y, uint16_t val)
{
    for (uint16_t i = 0; i < 4; i++)
    {
        EGL_SetPixel(egl, x + i, y, val);
        EGL_SetPixel(egl, x, y + i, val);
        EGL_SetPixel(egl, x + i, y + 3, val);
        EGL_SetPixel(egl, x + 3, y + i, val);
    }
}


// Affiche la pièce suivante
void LCD_DrawNextTetromino(render_lcd_mono_t *lcd)
{
    uint16_t x;
    uint16_t y;
    uint16_t val;
    const uint8_t *ptr = UNI_GetTetromino(&lcd->engine.p_next);

   for(uint16_t k=0; k<4; k++ )
   {
      for(uint16_t n=0; n<4; n++ )
      {
         if(ptr[4*k+n])
         {
            val = 1;
         }
         else
         {
            val = 0;
         }
         // transformation de coordonnée matricielle en coordonnée LCD
         x = 75 + n*4;
         y = 60 + k*4;
         
         LCD_PrintBaseSquare(&lcd->egl, x, y, val);
      }
   }
}

// Met à jour l'affichage
// optimisation : on ne modifie que les carrés différents depuis la précédente update
static void LCD_UpdateGrid(render_lcd_mono_t *lcd)
{
   uint16_t x, y, val;

   for(y=0; y<20; y++ ) {
      for(x=1; x<11; x++ ) {
         if( lcd->engine.grid[y][x] != lcd->grille_mem[y][x] ) {
            lcd->grille_mem[y][x] = lcd->engine.grid[y][x];
            if( lcd->grille_mem[y][x] ) {
               val = 1;
            } else {
               val = 0;
            }

            // transformation de coordonnée matricielle en coordonnée LCD
            uint16_t x_real = REAL_X_BASE + (x - 1) * 4;
            uint16_t y_real = REAL_Y_BASE + (y * 4);

            LCD_PrintBaseSquare(&lcd->egl, x_real, y_real, val);
         }
      }
   }
}

static void LCD_MemorizeGrid(render_lcd_mono_t *lcd)
{
    for (uint16_t i=0; i<21; i++)
    {
        for (uint16_t j=0; j<12; j++)
        {
            lcd->grille_mem[i][j] = lcd->engine.grid[i][j];
        }
    }
}


void LCD_Tick(render_lcd_mono_t *lcd, uint32_t key_event)
{
    UNI_Tick(&lcd->engine, key_event, 0, 0);

    LCD_UpdateGrid(lcd);

 //   EGL_PutString("Tetris - PAUSE -         ",0,0);
//	EGL_PutString("Tetris !                 ",0,0);

//            if( level == 9 ) {
//               EGL_PutString("YOUPI!", 110, 64);
//            }

    LCD_DrawNextTetromino(lcd);

    //    EGL_PutString("Perdu!", 110, 64);

    EGL_PutNumber(&lcd->egl, lcd->engine.lines, 9, 92, "%ld");
    EGL_PutNumber(&lcd->egl, lcd->engine.level+1, 58, 92, "%ld");
    EGL_PutNumber(&lcd->egl, lcd->engine.score, 4, 69, "%ld");

    LCD_MemorizeGrid(lcd);
}

// Init des graphismes et du jeu
void LCD_Initialize(render_lcd_mono_t *lcd)
{
    UNI_Initialize(&lcd->engine);

    EGL_Initialize(&lcd->egl, DISPLAY_WIDTH, DISPLAY_HEIGHT, &lcd->Vmem[0]);
    EGL_ClearScreen(&lcd->egl);
    EGL_PutString(&lcd->egl, "Tetris !                 ", 0, 0);

    EGL_DrawRectangle(&lcd->egl, REAL_X_BASE-1, REAL_Y_BASE+HAUTEUR_GRILLE, HAUTEUR_GRILLE+2, LARGEUR_GRILLE+2);

    EGL_CopyImage(&lcd->egl, tetris, 0, 23, 12, 80);
    EGL_PutString(&lcd->egl, "Score", 4, 61);
    EGL_PutString(&lcd->egl, "Lines", 9, 84);
    EGL_PutString(&lcd->egl, "Level", 58, 84);

    for (uint16_t i=0; i<21; i++)
    {
        for (uint16_t j=0; j<12; j++)
        {
            lcd->grille_mem[i][j] = 0;
        }
    }
    LCD_DrawNextTetromino(lcd);
}

void LCD_RenderBitmap(render_lcd_mono_t *lcd)
{
    BMP_Generate(lcd->Vmem, lcd->egl.display_width, lcd->egl.display_height, lcd->bitmap);
}

