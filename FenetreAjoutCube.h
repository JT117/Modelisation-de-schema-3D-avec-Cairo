#ifndef FENETRE_AJOUT_CUBE_H
#define FENETRE_AJOUT_CUBE_H

#include <gtk/gtk.h>
#include "Scene.h"

typedef struct FenetreAjoutCube FenetreAjoutCube;
struct FenetreAjoutCube
{
    GtkWidget* fenetre;
    Scene* scene;
};

void initialier_FenetreAjoutCube( FenetreAjoutCube* fao, Scene* scene );


static gboolean nouvel_ajout( GtkWidget *widget, GdkEvent *event, gpointer data );

#endif //FENETRE_AJOUT_CUBE_H
