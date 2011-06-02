#ifndef EXPORTWINDOW_H
#define EXPORTWINDOW_H

#include <gtk/gtk.h>
#include "Scene.h"

typedef struct ExportWindow
{
    GtkWidget* fenetre;
    Scene* scene;
    GtkWidget* longueur;
    GtkWidget* largeur;
    GtkWidget* wName;

    /*Combobox*/
    GtkWidget* comboBox;

    /* Boutons de la fenêtre*/
    GtkWidget* wOKButton;
    GtkWidget* wCancelButton;

    GtkWidget* barreObjet;
    GtkWidget* barreBouton;
    GtkWidget* layout;
}ExportWindow;

/**
 * Initialise la fenêtre d'exportation.
 */
void ExportWindow_init( ExportWindow* ew, Scene* scene);

#endif /* EXPORTWINDOW */
