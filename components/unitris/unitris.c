#include <stdio.h>
#include <stdlib.h>

#include "unitris.h"


static const int levels[] = {
    10,
    20,
    40,
    60,
    100,
    150,
    200,
    300,
    400,
    500
};

// Chaque pièce tient dans une matrice de 4x4
// On stocke toutes les rotations avec un offset
static const int pieces[16*4*7] = {
        
    // Carré
    0,0,0,0,
    0,1,1,0,
    0,1,1,0,
    0,0,0,0,

    0,0,0,0,
    0,1,1,0,
    0,1,1,0,
    0,0,0,0,

    0,0,0,0,
    0,1,1,0,
    0,1,1,0,
    0,0,0,0,

    0,0,0,0,
    0,1,1,0,
    0,1,1,0,
    0,0,0,0,

    // barre
    0,1,0,0,
    0,1,0,0,
    0,1,0,0,
    0,1,0,0,

    0,0,0,0,
    1,1,1,1,
    0,0,0,0,
    0,0,0,0,

    0,1,0,0,
    0,1,0,0,
    0,1,0,0,
    0,1,0,0,

    0,0,0,0,
    1,1,1,1,
    0,0,0,0,
    0,0,0,0,

    // Le T
    0,0,0,0,
    1,1,1,0,
    0,1,0,0,
    0,0,0,0,

    0,1,0,0,
    0,1,1,0,
    0,1,0,0,
    0,0,0,0,

    0,1,0,0,
    1,1,1,0,
    0,0,0,0,
    0,0,0,0,

    0,1,0,0,
    1,1,0,0,
    0,1,0,0,
    0,0,0,0,

    // Le L
    0,0,1,0,
    1,1,1,0,
    0,0,0,0,
    0,0,0,0,

    1,1,0,0,
    0,1,0,0,
    0,1,0,0,
    0,0,0,0,

    0,0,0,0,
    1,1,1,0,
    1,0,0,0,
    0,0,0,0,

    0,1,0,0,
    0,1,0,0,
    0,1,1,0,
    0,0,0,0,

    // le L inversé
    0,0,0,0,
    1,1,1,0,
    0,0,1,0,
    0,0,0,0,

    0,1,1,0,
    0,1,0,0,
    0,1,0,0,
    0,0,0,0,

    1,0,0,0,
    1,1,1,0,
    0,0,0,0,
    0,0,0,0,

    0,1,0,0,
    0,1,0,0,
    1,1,0,0,
    0,0,0,0,

    // Le S
    0,1,1,0,
    1,1,0,0,
    0,0,0,0,
    0,0,0,0,

    1,0,0,0,
    1,1,0,0,
    0,1,0,0,
    0,0,0,0,

    0,0,0,0,
    0,1,1,0,
    1,1,0,0,
    0,0,0,0,

    0,1,0,0,
    0,1,1,0,
    0,0,1,0,
    0,0,0,0,

    // Le Z
    1,1,0,0,
    0,1,1,0,
    0,0,0,0,
    0,0,0,0,

    0,1,0,0,
    1,1,0,0,
    1,0,0,0,
    0,0,0,0,

    0,0,0,0,
    1,1,0,0,
    0,1,1,0,
    0,0,0,0,

    0,0,1,0,
    0,1,1,0,
    0,1,0,0,
    0,0,0,0
};

    // Ajoute ou retire une pièce de la grid
static void UNI_DisplayTetromino(unitris_t *ctx, bool add )
{
    int x, y, val;
    tetromino_t *p = &ctx->p_curr;
    const int *ptr = &pieces[p->type*16*4 + 16*p->rot];

    // affichage de la pièce courante
    for( y=0; y<4; y++ ) {
        for( x=0; x<4; x++ ) {
            if( ptr[4*y+x] ) {
                if( add == true ) {
                val = 1;
                } else {
                val = 0;
                }
                ctx->grid[p->y+y][p->x+x] = val;
            }
        }
    }
}


    /**
    * Retourne true si la pièce peut être bougée.
    * L'algo est simple : on essaye de voir si la nouvelle pièce peut tenir dans 
    * la grid à cet emplacement
    */ 
static bool UNI_CanMoveTetromino(unitris_t *ctx, int x, int y, int rot)
{
    int i, j;
    tetromino_t *p = &ctx->p_curr;
    const int *ptr = &pieces[p->type*16*4 + 16*rot]; // pointeur sur la pièce + rotation

    // On efface la pièce de la grid
    UNI_DisplayTetromino(ctx, false);

    // tentative de placement aux nouvelles coordonnées
    for (i=0; i<4; i++) {
        for (j=0; j<4; j++) {
            if ( (ctx->grid[p->y+i+y][p->x+j+x] + ptr[4*i+j]) == 2 ) {
                UNI_DisplayTetromino(ctx, true); // laisse la pièce où elle était
                return false;
            }
        }
    }
    return true;
}


static void UNI_NewTetromino(tetromino_t *p)
{
    p->x = 4;
    p->y = 0;
    p->rot = 0;
    p->type = rand()%7;
}

static void UNI_DetectLine(unitris_t *ctx)
{
    int i, j, k, cnt;

    // détecte les lignes réalisées
    for (i=19; i>0; i--) {
        cnt = 0;
        for (j=1; j<11; j++) {
            if (ctx->grid[i][j]) {
                cnt++;
            }
        }
        if (cnt==10) {
            ctx->lines++;
            ctx->score += (ctx->level+1)*100;
            // ligne complète, on décale tout ce qu'il y a au dessus
            for (k=(i-1); k>0; k--) {
                for (j=1; j<11; j++) {
                ctx->grid[k+1][j] = ctx->grid[k][j];
                }
            }

            // dernière ligne toujours à zéro
            for (j=1; j<11; j++) {
                ctx->grid[0][j] = 0;
            }
            i++; // on reprend la détection à la ligne courante
        }
    }
}


bool UNI_Tick(unitris_t *ctx, uint32_t key_evt, uint32_t key_held, uint32_t key_released)
{
    int rot;
    static int tab[2] = {0,0};
        
    if (ctx->pause || ctx->stop) {
        return(false);
    }

    // gestion de l'accélération latérale de la pièce si appui long
    if ( key_held & KEY_LEFT ) {
        tab[0]++;
    } else if( key_released&KEY_LEFT ) {
        tab[0] = 0;
    } else if( key_held & KEY_RIGHT ) {
        tab[1]++;
    } else if( key_released&KEY_RIGHT ) {
        tab[1] = 0;
    }

    if ( (key_held & KEY_RIGHT) || (tab[1]>=6) )
    {
        if( UNI_CanMoveTetromino(ctx, 1, 0, ctx->p_curr.rot) == true )
        {
            ctx->p_curr.x += 1;
                UNI_DisplayTetromino(ctx, true);
        }
    } 
    else if ( (key_evt & KEY_LEFT) || (tab[0]>=6) )
    {
        if( UNI_CanMoveTetromino(ctx, -1, 0, ctx->p_curr.rot) == true )
        {
            ctx->p_curr.x -= 1;
            UNI_DisplayTetromino(ctx, true);
        }
    }
    else if ( key_evt & KEY_UP )
    {
        rot = (ctx->p_curr.rot+1)%4;
        if( UNI_CanMoveTetromino(ctx, 0, 0, rot) == true )
        {
            ctx->p_curr.rot = rot;
            UNI_DisplayTetromino(ctx, true);
        }
    }
    else if ( key_held & KEY_DOWN )
    {
        if( UNI_CanMoveTetromino(ctx, 0, 1, ctx->p_curr.rot) == true )
        {
            ctx->p_curr.y++;
            UNI_DisplayTetromino(ctx, true);
        }
    }

    if( ++ctx->seq >= (10-ctx->level) ) {
        // 500ms
        ctx->seq = 0;
        // On tombe la pièce
        if( UNI_CanMoveTetromino(ctx, 0, 1, ctx->p_curr.rot) == true )
        {
            ctx->p_curr.y++;
            UNI_DisplayTetromino(ctx, true);
        }
        else
        {
            UNI_DetectLine(ctx);
            ctx->game_cnt++;
            // passage au niveau suivant ?
            if( ctx->game_cnt >= levels[ctx->level] ) {
                if( ctx->level == 9 ) {
                    ctx->stop = 1; // arret du jeu
                }
                ctx->level++;
            }
            ctx->p_curr = ctx->p_next;
            UNI_NewTetromino(&ctx->p_next); // tirer une autre pièce au hasard
            UNI_DisplayTetromino(ctx, true);

            if( UNI_CanMoveTetromino(ctx, 0, 1, ctx->p_curr.rot) == false )
            {
                ctx->stop = 1; // perdu !
            }
        }

    }

    return(true);
}



// Init des graphismes et du jeu
void UNI_Initialize(unitris_t *ctx)
{
    int i, j;

    // init variables
    for (i=0; i<21; i++) {
        for (j=0; j<12; j++) {
            if( j==0 || j==11 || i==20 ) {
                ctx->grid[i][j] = 1;
            } else {
                ctx->grid[i][j] = 0;
            }
        }
    }

    ctx->score = 0;
    ctx->level = 0;
    ctx->lines = 0;
    ctx->stop = 0;
    ctx->game_cnt = 0;
    ctx->pause = 0;
    ctx->seq = 0;
}
