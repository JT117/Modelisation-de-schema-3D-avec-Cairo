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
#include "Clavier.h"

/** Structure representant la scene */
typedef struct Scene Scene;
struct Scene
{
    /** Pointeur sur le module des modifications */
    Modification* modification;
    /** Pointeur sur le module de selection */
    Selection* selection;
    /** Tableau contenant tous les objets de la scene */
    GArray* tObjet;
    /** Int contenant le nombre d'objet dans le tableau tObjet */
    int nbObjet;
    /** Pointeur sur la module clavier */
    Clavier* clavier;
    /** Un point contenant les informations necessaire à l'ajout d'un nouvel element */
    Point* creation;
    float tailleCreation;
    /** Pointeur sur la zone de dessin */
    GtkWidget* zoneDeDessin;
};

void Scene_initialiser_scene( Scene* scene, GtkWidget* window );

void Scene_ajouter_cube( Scene* scene, Cube* cCube );

void Scene_dessiner_scene( Scene* scene, cairo_t* cr );

gboolean Scene_selection_Multiple( Scene* scene );

void Scene_creation_objet( Scene* scene, double x, double y );

void Scene_clear_scene( Scene* scene, cairo_t* cr );

void Scene_detruire( Scene* scene );

void Scene_reset( Scene* scene, GtkWidget* window );

void Scene_reconstruire( Scene* scene, GtkWidget* window );


#endif // SCENE_H
