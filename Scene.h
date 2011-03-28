#ifndef SCENE_H
#define SCENE_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include <string.h>
#include "Cube.h"

typedef struct Scene Scene;
struct Scene
{
    GArray* tCube;
    GArray* tCubeSelection;
    GArray* tTouche;
    int nbCube;
    int nbCubeSelection;
    int nbTouche;
};

void initialiser_Scene( Scene* scene, GtkWidget* window );

void ajouter_cube( Scene* scene, Cube* cCube );

void dessiner_Scene( Scene* scene, cairo_t* cr );

void selectionner_objet( Scene* scene, double x, double y );

void touche_appuyer( Scene* scene, char* nomTouche );

void touche_relacher( Scene* scene, char* nomTouche );

gboolean selection_Multiple( Scene* scene );

void deselectionner_tout( Scene* scene );


#endif // SCENE_H
