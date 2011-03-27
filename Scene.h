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
    int nbCube;
};

void initialiser_Scene( Scene* scene, GtkWidget* window );

void ajouter_cube( Scene* scene, Cube* cCube );

void dessiner_Scene( Scene* scene, cairo_t* cr );

void selectionner_objet( Scene* scene, double x, double y );


#endif // SCENE_H
