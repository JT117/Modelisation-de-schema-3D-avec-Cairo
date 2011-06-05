#ifndef EXPORTWINDOW_H
#define EXPORTWINDOW_H

#include <gtk/gtk.h>
#include <cairo.h>
#include "Scene.h"
#include <cairo-svg.h>
#include<cairo-pdf.h>

typedef struct ExportWindow
{
    Scene* scene;

    /*Combobox*/
    GtkWidget* comboBox;

}ExportWindow;

/**
 * Initialise la fenêtre d'exportation.
 */
void ExportWindow_init( ExportWindow* ew, Scene* scene);

#endif /* EXPORTWINDOW */
