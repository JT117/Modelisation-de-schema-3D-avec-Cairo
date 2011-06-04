#ifndef SELECTION_H
#define SELECTION_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

struct Scene;
struct Objet;

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

void Selection_initialiser( Selection* selection );

void Selection_selectionner_objet( struct Scene* scene, double x, double y );

void Selection_deselectionner_tout( struct Scene* scene );

void Selection_selectionner_tout( struct Scene* scene );

void Selection_selectionner_click_drag( struct Scene* scene );

void Selection_dessiner_rectangle( Selection* selection, cairo_t* cr );

void Selection_detruire( Selection* selection );

gboolean Selection_est_deja_selectionner( Selection* selection,struct Objet* objet );

void Selection_deselectionner( struct Scene* scene,struct Objet* objet );

void Selection_selectionner( struct Scene* scene,struct Objet* objet );

/**
 * Calcul si un clique de souris est contune dans le quadrilatere convexe passé en paramètre.
 */
gboolean Selection_inFace(tCoord2D tP1,tCoord2D tP2,tCoord2D tP3, tCoord2D tP4, double dXClick, double dYClick );

#endif  //SELECTION_H







