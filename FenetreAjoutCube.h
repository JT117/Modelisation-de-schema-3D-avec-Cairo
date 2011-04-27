#ifndef FENETRE_AJOUT_CUBE_H
#define FENETRE_AJOUT_CUBE_H

#include <gtk/gtk.h>
#include "Scene.h"

typedef struct FenetreAjoutCube FenetreAjoutCube;
struct FenetreAjoutCube
{
    GtkWidget* fenetre;
    Scene* scene;
    GtkWidget* longueur;
    GtkWidget* largeur;
    GtkWidget* entry1; // X
    GtkWidget* entry2; // Y
    GtkWidget* entry3; // Z
    GtkWidget* boutonOk;
    GtkWidget* boutonAnnuler;
    GtkWidget* barreSelection;
    GtkWidget* barrePosition;
    GtkWidget* barreBouton;
    GtkWidget* layout;
    GtkWidget* hbox_cube;
    GtkWidget* hbox_rect;
    char* dernierLayout;

    double x, y, z;
    double L;
    double l;
};

void initialier_FenetreAjoutCube( FenetreAjoutCube* fao, Scene* scene );



#endif //FENETRE_AJOUT_CUBE_H
