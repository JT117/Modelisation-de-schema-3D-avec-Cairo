#ifndef FENETRE_AJOUT_H
#define FENETRE_AJOUT_H

#include <gtk/gtk.h>

#include "Scene.h"
#include "Color.h"

typedef struct FenetreAjout FenetreAjout;
struct FenetreAjout
{
    GtkWidget* fenetre;
    Scene* scene;
    GtkWidget* longueur;
    GtkWidget* largeur;
    GtkWidget* entry1; // X
    GtkWidget* entry2; // Y
    GtkWidget* entry3; // Z

    GtkWidget* entryR; // R
    GtkWidget* entryG; // G
    GtkWidget* entryB; // B
    GtkWidget* entryA; // Alpha

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


    /* Infos relatives au triangle et au quadrilatère*/
    GtkWidget* hbox_point1;
    GtkWidget* hbox_point2;
    GtkWidget* hbox_point3;
    GtkWidget* hbox_point4;

    GtkWidget* xCoord[4]; // X
	GtkWidget* yCoord[4]; // Y
	GtkWidget* zCoord[4]; // Z

    double x, y, z;
    double L;
    double l;
};

void initialiser_FenetreAjout( FenetreAjout* fao, Scene* scene );


#endif //FENETRE_AJOUT_H
