#ifndef CLAVIER_H
#define CLAVIER_H

#include <gtk/gtk.h>

typedef struct Clavier Clavier;
struct Clavier
{
    GArray* tTouche;
    int nbTouche;
};


#endif  //CLAVIER_H
