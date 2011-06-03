#ifndef FEN_TRANS_H
#define FEN_TRANS_H

#include <gtk/gtk.h>
#include "Scene.h"

typedef struct FenTrans FenTrans;
struct FenTrans
{
    GtkWidget* fenetre;
    Scene* scene;
    GtkWidget* entry1;
    GtkWidget* entry2;
    GtkWidget* entry3;
    GtkWidget* radio1;
    GtkWidget* radio2;
    GtkWidget* radio3;
    Groupe* groupeSelectionne;
    gboolean unGroupeEstSelectionner;
};

void FenTrans_init( FenTrans* ft, Scene* scene );

void FenTrans_validation( GtkButton* button, gpointer data );

#endif //FEN_TRANS_H
