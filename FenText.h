#ifndef FEN_TEXT_H
#define FEN_TEXT_H

#include <gtk/gtk.h>
#include "Scene.h"

typedef struct FenText FenText;
struct FenText
{
    GtkWidget* fenetre;
    Scene* scene;
    GtkWidget* entry1;
    GtkWidget* fontSelection;
    char* texte;
    char* font;

};

void FenText_init( FenText* ft, Scene* scene );

void FenText_validation( GtkButton* button, gpointer data );

void FenText_propriete( GtkButton* button, gpointer data );

void FenText_changement( GtkButton* button, gpointer data );

#endif //FEN_TEXT_H

