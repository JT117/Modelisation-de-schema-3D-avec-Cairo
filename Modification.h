#ifndef MODIFICATION_H
#define MODIFICATION_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>


typedef struct Modification Modification;
struct Modification
{
    int actuel;
    int nbMax;
    GtkWidget* annuler;
    GtkWidget* refaire;
};


#endif  //MODIFICATION_H




