#ifndef UNITRIS_H
#define UNITRIS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Bit field
#define KEY_LEFT    1
#define KEY_RIGHT   2
#define KEY_UP      4
#define KEY_DOWN    8

typedef struct
{
    int x,y; // coordonnées dans la matrice
    int type; // type de pièce (carré, barre ...)
    int rot; // numéro de la rotation [0..3]
} tetromino_t;


typedef struct
{
    /**
     * une grid de Tetris fait 20 lignes sur 10 colonnes
     * avec une ligne fantôme (en bas) et deux colonnes afin de simuler les bords.
     * 
     * Aspect de la grid vide :
     *   1     1
     *   1     1
     *   1     1
     *   1     1
     *   1111111
     */
    int grid[21][12];
    
    long score;
    int lines;
    int level;
    int stop;
    int pause;
    int game_cnt;
    
    int seq;
    tetromino_t p_curr; // current tetromino
    tetromino_t p_next; // next tetromino
    
} unitris_t;

/**
 * @brief Main Unitris tick engine
 * 
 * Must be called every 50ms
 * 
 */
bool UNI_Tick(unitris_t *ctx, uint32_t key_evt, uint32_t key_held, uint32_t key_released);
void UNI_Initialize(unitris_t *ctx);

#ifdef __cplusplus
}
#endif

#endif // UNITRIS_H
