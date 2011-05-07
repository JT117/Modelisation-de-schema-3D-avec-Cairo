#ifndef CLAVIER_H
#define CLAVIER_H

#include <gtk/gtk.h>

/** Structure permettant de gérant le clavier */
typedef struct Clavier Clavier;
struct Clavier
{
    /** Tableau representant les touches actuellement appuyées */
    GArray* tTouche;
    /** Nombre de touche contenu dans le tableau */
    int nbTouche;
};


#endif  //CLAVIER_H
