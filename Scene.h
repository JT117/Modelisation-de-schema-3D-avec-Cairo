#ifndef SCENE_H
#define SCENE_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include <string.h>

#include "Objet.h"
#include "Cube.h"
#include "Point.h"
#include "Selection.h"
#include "Modification.h"

typedef struct Scene Scene;
struct Scene
{
    Modification* modification;
    Selection* selection;
    GArray* tObjet;
    GArray* tTouche;
    int nbObjet;
    int nbTouche;
    Point* creation;
    float tailleCreation;
    GtkWidget* zoneDeDessin;
};

void Scene_initialiser_scene( Scene* scene, GtkWidget* window );

void Scene_ajouter_cube( Scene* scene, Cube* cCube );

void Scene_dessiner_scene( Scene* scene, cairo_t* cr );

void Scene_touche_appuyer( Scene* scene, char* nomTouche );

void Scene_touche_relacher( Scene* scene, char* nomTouche );

gboolean Scene_selection_Multiple( Scene* scene );

void Scene_creation_objet( Scene* scene, double x, double y );

void Scene_clear_scene( Scene* scene, cairo_t* cr );

void Scene_detruire( Scene* scene );

void Scene_reset( Scene* scene, GtkWidget* window );


#endif // SCENE_H
