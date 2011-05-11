#ifndef SELECTION_H
#define SELECTION_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "Objet.h"
#include "Point.h"

/** Structure Selection gérant la selection des objets dans la scene */
typedef struct Selection Selection;
struct Selection
{
    /** Un tableau de pointeur sur les Objets selectionner dans la scene */
    GArray* tSelection;
    /** nombre d'elements selectionner dans la scene */
    int nbSelection;
    /** Flag indiquant si une selection est en cours utile pour le rectangle de selection */
    gboolean selection_en_cours;
    /** Flag indiquant si une selection multiple est en cours i.e. dans la plupart des cas si la touche Shift_L est enfoncé */
    gboolean selection_multiple;
    /** Un point contenant les coordonnées du premier clic de selection, servant pour dessiner le rectangle de selection */
    Point departSelection;
    /** Un point contenant les coordonnées de la position de la souris quand elle se deplace, servant aussi pour le rectangle de selection */
    Point finSelection;
};


#endif  //SELECTION_H







