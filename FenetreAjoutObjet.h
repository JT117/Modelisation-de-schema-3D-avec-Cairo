#ifndef FENETRE_AJOUT_CUBE_H
#define FENETRE_AJOUT_CUBE_H

#include <gtk/gtk.h>

typedef struct FenetreAjoutCube FenetreAjoutCube;
struct FenetreAjoutCube
{
    GtkWidget* fenetre;
    GtkWidget* comboBox;
    GtkWidget* text;
    GtkWidget* vBox;
    GtkWidget* hBox;
};

void initialier_FenetreAjoutCube( FenetreAjoutCube* fao );

static gboolean selection_change( GtkWidget *widget, gpointer data );

void detruire_FenetreAjoutCube( FenetreAjoutCube* fao );



#endif //FENETRE_AJOUT_CUBE_H
