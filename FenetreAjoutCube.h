#ifndef FENETRE_AJOUT_CUBE_H
#define FENETRE_AJOUT_CUBE_H

#include <gtk/gtk.h>
#include "Scene.h"

typedef struct FenetreAjoutCube FenetreAjoutCube;
struct FenetreAjoutCube
{
    GtkWidget* fenetre;
    Scene* scene;
    GtkWidget* entry;
    GtkWidget* entry1;
    GtkWidget* entry2;
    GtkWidget* entry3;
    GtkWidget* boutonOk;
    GtkWidget* boutonAnnuler;

};

void initialier_FenetreAjoutCube( FenetreAjoutCube* fao, Scene* scene );



#endif //FENETRE_AJOUT_CUBE_H
