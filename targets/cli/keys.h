/**
 * \addtogroup IHM
 * @{
 */

/**
 * \file
 * Fonctions de gestion du clavier
 * \author Anthony RABINE <anthony.rabine@chauvin-arnoux.com>
 */

/*--------------------------------------------------------------------------
|Copyright(C) 2006 ENERDIS
----------------------------------------------------------------------------
|Projet/Logiciel  :  N0075/ENERIUM50
|Responsable      :  J. OGIS
----------------------------------------------------------------------------
|Fichier          :  keys.h
|Chemin d'acces   :
|Auteur           :
|Module           :
|Date de creation :  17/07/2006
|version          :
|Description      :
|
----------------------------------------------------------------------------
|                            H I S T O R I Q U E
----------------------------------------------------------------------------
|Version                : $Revision: 1.1 $
|Auteur                 : $Author: anthony $
|Date de modification   : $Date: 2007-01-02 08:37:52 $
|Nature de la (des) modification(s)  :
|
--------------------------------------------------------------------------*/
#ifndef _KEYS_H
#define _KEYS_H


/*--------------------------------------------------------------------------
                    IMPORTATION DES EN-TETES DU MODULE
--------------------------------------------------------------------------*/
#include <c5502.h>


/*--------------------------------------------------------------------------
                               DEFINITIONS
--------------------------------------------------------------------------*/
#define KEY_LEFT     0x1
#define KEY_RIGHT    0x2
#define KEY_UP       0x4
#define KEY_DOWN     0x8
#define KEY_OK       0x10

/*--------------------------------------------------------------------------
                             VARIABLES EXPORTEES
--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------
                               FONCTIONS EXPORTEES
--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void keys_init(void);
void scan_keys(void);
uint16 get_key_held(void);
uint16 get_key_hit(void);
uint16 get_key_released(void);

#ifdef __cplusplus
}
#endif


#endif /* _KEYS_H */

/*--------------------------------------------------------------------------
                              FIN FICHIER
--------------------------------------------------------------------------*/
/** @} */

