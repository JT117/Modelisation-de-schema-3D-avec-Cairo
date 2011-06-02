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
    GtkWidget* boutonAnnuler;
    GtkWidget* barreObjet;
    GtkWidget* barreGroupe;
    GtkWidget* barreBouton;
    GtkWidget* barreCouleur;
    GtkWidget* layout;
    GtkWidget* hbox_cube;
    GtkWidget* hbox_rect;
    char* dernierLayout;
    GtkWidget* comboBoxGroupe;

    GtkWidget* entryR; // R
    GtkWidget* entryG; // G
    GtkWidget* entryB; // B
    GtkWidget* entryA; // Alpha
    Objet* objet;

    double x, y, z;
    double L;
    double l;
};

void FenetrePropriete_initialiser( FenetrePropriete* fp, Scene* scene );

void FenetrePropriete_validation( GtkButton* button, gpointer data );

#endif //FENETRE_PROPRIETE_H

