#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "egl.h"

/*******************************************************************************
 * **********
 * _________ _______ _________ _______ _________ _______ 
 * \__   __/(  ____ \\__   __/(  ____ )\__   __/(  ____ \
 *   ) (   | (    \/   ) (   | (    )|   ) (   | (    \/
 *   | |   | (__       | |   | (____)|   | |   | (_____ 
 *   | |   |  __)      | |   |     __)   | |   (_____  )
 *   | |   | (         | |   | (\ (      | |         ) |
 *   | |   | (____/\   | |   | ) \ \_____) (___/\____) |
 *   )_(   (_______/   )_(   |/   \__/\_______/\_______)
 * 
 *   Tetris caché ! Pour y accéder depuis l'écran de l'heure : 3 fois "OK" puis 
 * "OK + gauche"
 
******************************************************************************** 
************/


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

/**
 * une grille de Tetris fait 20 lignes sur 10 colonnes
 * avec une ligne fantôme (en bas) et deux colonnes afin de simuler les bords.

Aspect de la grille vide :
   1     1
   1     1
   1     1
   1     1
   1111111
 */
static int grille[21][12];
static int grille_mem[21][12]; // grille précédente
extern const uint16_t tetris[];
static long score;
static int lines, level, stop, pause, game_cnt;
static int niveaux[] = {
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

typedef struct
{
   int x,y; // coordonnées dans la matrice
   int type; // type de pièce (carré, barre ...)
   int rot; // numéro de la rotation [0..3]
} PIECE;

// L'origine commence en haut à gauche
#define REAL_X_BASE  110
#define REAL_Y_BASE  22
#define HAUTEUR_GRILLE  80
#define LARGEUR_GRILLE  40

static int seq = 0;
static PIECE p_curr;
static PIECE p_next;

// Chaque pièce tient dans une matrice de 4x4
// On stocke toutes les rotations avec un offset
int pieces[16*4*7] = {
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
//};
//int barre[16*4] = {
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
//};
//int t[16*4] = {
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
//};
//int l[16*4] = {
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
//};
//int l_inv[16*4] = {
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
//};
//int s[16*4] = {
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
//};
//int z[16*4] = {
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

// Ajoute ou retire une pièce de la grille
void AFF_Piece( PIECE *p, bool add )
{
   int x, y, val;
   int *ptr = &pieces[p->type*16*4 + 16*p->rot];
   
   // affichage de la pièce courante
   for( y=0; y<4; y++ ) {
      for( x=0; x<4; x++ ) {
         if( ptr[4*y+x] ) {
            if( add == true ) {
               val = 1;
            } else {
               val = 0;
            }
            grille[p->y+y][p->x+x] = val;
         }
      }
   }
}

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
// optimisation : on ne modifie que les carrés différents depuis la précédente 
update
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


/**
 * Retourne true si la pièce peut être bougée.
 * L'algo est simple : on essaye de voir si la nouvelle pièce peut tenir dans 
 * la grille à cet emplacement
 */ 
bool AFF_CanMovePiece(PIECE *p, int x, int y, int rot)
{
   int i, j;
   int *ptr = &pieces[p->type*16*4 + 16*rot]; // pointeur sur la pièce + 
rotation

   // On efface la pièce de la grille
   AFF_Piece(p,false);

   // tentative de placement aux nouvelles coordonnées
   for (i=0; i<4; i++) {
      for (j=0; j<4; j++) {
         if ( (grille[p->y+i+y][p->x+j+x] + ptr[4*i+j]) == 2 ) {
            AFF_Piece(p,true); // laisse la pièce où elle était
            return false;
         }
      }
   }
   return true;
}


void AFF_NewPiece(PIECE *p)
{
   p->x = 4;
   p->y = 0;
   p->rot = 0;
   p->type = rand()%7;
}

void AFF_DetectLine(void)
{
   int i, j, k, cnt;

   // détecte les lignes réalisées
   for (i=19; i>0; i--) {
      cnt = 0;
      for (j=1; j<11; j++) {
         if (grille[i][j]) {
            cnt++;
         }
      }
      if (cnt==10) {
         lines++;
         score += (level+1)*100;
         // ligne complète, on décale tout ce qu'il y a au dessus
         for (k=(i-1); k>0; k--) {
            for (j=1; j<11; j++) {
               grille[k+1][j] = grille[k][j];
            }
         }

         // dernière ligne toujours à zéro
         for (j=1; j<11; j++) {
            grille[0][j] = 0;
         }
         i++; // on reprend la détection à la ligne courante
      }
   }
}


bool AFF_UpdateTetris(uint32_t event)
{
   int rot;
   static int tab[2] = {0,0};

   if (get_key_hit()&KEY_OK) {
      if (++pause>=2) {
         e->type = IHM_SERVICES;
         AFF_EcranServices(2);
         return(true);
      } else {
         EGL_PutString("Tetris - PAUSE -         ",0,0);
         return(false);
      }
   }
 
   if (stop==1) {
      return(false);
   }
   if (pause) {
      if (get_key_hit()&(KEY_LEFT|KEY_RIGHT)) {
         EGL_PutString("Tetris !                 ",0,0);
         pause = 0;
      } else {
         return(false);
      }
   }

   // gestion de l'accélération latérale de la pièce si appui long
   if ( get_key_held()&KEY_LEFT ) {
      tab[0]++;
   } else if( get_key_released()&KEY_LEFT ) {
      tab[0] = 0;
   } else if( get_key_held()&KEY_RIGHT ) {
      tab[1]++;
   } else if( get_key_released()&KEY_RIGHT ) {
      tab[1] = 0;
   }

   if ( (get_key_held()&KEY_RIGHT) || (tab[1]>=6) ) {
      if( AFF_CanMovePiece(&p_curr, 1, 0, p_curr.rot) == true ) {
         p_curr.x += 1;
         AFF_Piece(&p_curr, true);
      }
   } else if ( (get_key_hit()&KEY_LEFT) || (tab[0]>=6) ) {
      if( AFF_CanMovePiece(&p_curr, -1, 0, p_curr.rot) == true ) {
         p_curr.x -= 1;
         AFF_Piece(&p_curr, true);
      }
   } else if ( get_key_hit()&KEY_UP ) {
      rot = (p_curr.rot+1)%4;
      if( AFF_CanMovePiece(&p_curr, 0, 0, rot) == true ) {
         p_curr.rot = rot;
         AFF_Piece(&p_curr, true);
      }
   } else if ( get_key_held()&KEY_DOWN ) {
      if( AFF_CanMovePiece(&p_curr, 0, 1, p_curr.rot) == true ) {
         p_curr.y++;
         AFF_Piece(&p_curr, true);
      }
   }

   if( ++seq >= (10-level) ) {
      // 500ms
      seq = 0;
      // On tombe la pièce
      if( AFF_CanMovePiece(&p_curr, 0, 1, p_curr.rot) == true ) {
         p_curr.y++;
         AFF_Piece(&p_curr, true);
      } else {
         AFF_DetectLine();
         game_cnt++;
         // passage au niveau suivant ?
         if( game_cnt>=niveaux[level] ) {
            if( level == 9 ) {
               EGL_PutString("YOUPI!", 110, 64);
               stop = 1; // arret du jeu
            }
            level++;
         }
         p_curr = p_next;
         AFF_NewPiece(&p_next); // tirer une autre pièce au hasard
         AFF_Piece(&p_curr, true);
         AFF_NextPiece(&p_next);
         if( AFF_CanMovePiece(&p_curr, 0, 1, p_curr.rot) == false ) {
            EGL_PutString("Perdu!", 110, 64);
            stop = 1; // perdu !
         }
      }
      EGL_PutNumber(lines, 9, 92, "%ld");
      EGL_PutNumber(level+1, 58, 92, "%ld");
      EGL_PutNumber(score, 4, 69, "%ld");
   }
   AFF_Update();
   return(true);
}



// Init des graphismes et du jeu
void AFF_GrilleTetris(void)
{
   int i, j;

   EGL_ClearCentralZone();
   EGL_PutString("Tetris !                 ",0,0);
   EGL_DrawRectangle(REAL_X_BASE-1, REAL_Y_BASE+HAUTEUR_GRILLE, 
HAUTEUR_GRILLE+2, LARGEUR_GRILLE+2);
   
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

   score = 0;
   level = 0;
   lines = 0;
   stop = 0;
   game_cnt = 0;
   pause = 0;

   AFF_NewPiece(&p_curr);
   AFF_NewPiece(&p_next);
   AFF_Piece(&p_curr, true);
   AFF_NextPiece(&p_next);
   AFF_Update();
}
