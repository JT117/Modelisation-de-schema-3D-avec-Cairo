#ifndef SELECTION_H
#define SELECTION_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "Objet.h"
#include "Point.h"


typedef struct Selection Selection;
struct Selection
{
    GArray* tSelection;
    int nbSelection;
    gboolean selection_en_cours;
    Point departSelection;
    Point finSelection;
};


#endif  //SELECTION_H







