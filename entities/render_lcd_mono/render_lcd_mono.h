#ifndef RENDER_LCD_MONO
#define RENDER_LCD_MONO

#include "egl.h"
#include "unitris.h"
#include "bmplib.h"

#ifdef __cplusplus
extern "C" {
#endif


#define DISPLAY_WIDTH   160
#define DISPLAY_HEIGHT  128
#define VMEM_SIZE ((DISPLAY_WIDTH*DISPLAY_HEIGHT)/32)

// L'origine commence en haut à gauche
#define REAL_X_BASE  110
#define REAL_Y_BASE  22
#define HAUTEUR_GRILLE  80
#define LARGEUR_GRILLE  40

typedef struct {
    unitris_t engine;  // Moteur du jeu
    egl_t egl;  // Paramètres de l'écran où dessiner
	uint32_t Vmem[VMEM_SIZE]; // Video Memory -> où l'on dessine

    int grille_mem[21][12]; // grille précédente (on ne dessine que les pixels différents

    // Position de la grille Tetris dans l'écran virtuel
    uint8_t x_grid;
    uint8_t y_grid;
    uint8_t grid_width;
    uint8_t grid_height;

    char bitmap[BMP_FILE_SIZE(DISPLAY_WIDTH, DISPLAY_HEIGHT)];

} render_lcd_mono_t;

void LCD_Initialize(render_lcd_mono_t *lcd);
void LCD_Tick(render_lcd_mono_t *lcd);
void LCD_RenderBitmap(render_lcd_mono_t *lcd);

#ifdef __cplusplus
}
#endif

#endif
