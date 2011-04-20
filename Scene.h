#ifndef SCENE_H
#define SCENE_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include <string.h>

#include "Objet.h"
#include "Cube.h"
#include "Point.h"

typedef struct Scene Scene;
struct Scene
{
    Scene* scene;
    GArray* tObjet;
    GArray* tSelection;
    GArray* tTouche;
    int nbObjet;
    int nbSelection;
    int nbTouche;
    gboolean selection_en_cours;
    Point departSelection;
    Point finSelection;
    Point creation;
    int tailleCreation;
};

void Scene_initialiser_scene( Scene* scene, GtkWidget* window );

void Scene_ajouter_cube( Scene* scene, Cube* cCube );

void Scene_dessiner_scene( Scene* scene, cairo_t* cr );

void Scene_selectionner_tout( Scene* scene );

void Scene_selectionner_objet( Scene* scene, double x, double y );

void Scene_selectionner_click_drag( Scene* scene );

void Scene_touche_appuyer( Scene* scene, char* nomTouche );

void Scene_touche_relacher( Scene* scene, char* nomTouche );

gboolean Scene_selection_Multiple( Scene* scene );

void Scene_deselectionner_tout( Scene* scene );

void Scene_creation_objet( Scene* scene, double x, double y );

void Scene_clear_scene( Scene* scene, cairo_t* cr );


#endif // SCENE_H
