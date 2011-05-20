#ifndef FENETRE_AJOUT_H
#define FENETRE_AJOUT_H

#include <gtk/gtk.h>

#include "Scene.h"
#include "Color.h"

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
    GtkWidget* entryR; // Z
    GtkWidget* entryG; // Z
    GtkWidget* entryB; // Z
    GtkWidget* entryA; // Z
    GtkWidget* boutonOk;
    GtkWidget* boutonAnnuler;
    GtkWidget* barreSelection;
    GtkWidget* barrePosition;
    GtkWidget* barreCouleur;
    GtkWidget* barreBouton;
    GtkWidget* layout;
    GtkWidget* hbox_cube;
    GtkWidget* hbox_rect;
    char* dernierLayout;
    GtkWidget* comboBoxGroupe;

    double x, y, z;
    double L;
    double l;
};

void initialiser_FenetreAjoutCube( FenetreAjoutCube* fao, Scene* scene );

#endif //FENETRE_AJOUT_H
