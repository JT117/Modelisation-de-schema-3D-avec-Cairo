#ifndef FENETRE_PROPRIETE_H
#define FENETRE_PROPRIETE_H

#include <gtk/gtk.h>
#include "Scene.h"

typedef struct FenetrePropriete FenetrePropriete;
struct FenetrePropriete
{
    GtkWidget* fenetre;
    Scene* scene;
    GtkWidget* longueur;
    GtkWidget* largeur;
    GtkWidget* entry1; // X
    GtkWidget* entry2; // Y
    GtkWidget* entry3; // Z
    GtkWidget* boutonOk;
    GtkWidget* barreObjet;
    GtkWidget* barreGroupe;
    GtkWidget* barreBouton;
    GtkWidget* layout;
    GtkWidget* hbox_cube;
    GtkWidget* hbox_rect;
    char* dernierLayout;

    double x, y, z;
    double L;
    double l;
};

void FenetrePropriete_initialiser( FenetrePropriete* fp, Scene* scene );



#endif //FENETRE_PROPRIETE_H

