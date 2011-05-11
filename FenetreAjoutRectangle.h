#ifndef FENETRE_AJOUT_RECTANGLE_H
#define FENETRE_AJOUT_RECTANGLE_H

#include <gtk/gtk.h>
#include "Scene.h"

typedef struct FenetreAjoutRectangle
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
}FenetreAjoutRectangle;

void initialiser_FenetreAjoutRectangle( FenetreAjoutRectangle* fao, Scene* scene );

#endif //s
