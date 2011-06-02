#ifndef EXPORTWINDOW_H
#define EXPORTWINDOW_H

#include <gtk/gtk.h>
#include "Scene.h"

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
