#ifndef GROUPE_H
#define GROUPE_H

#include <stdlib.h>
#include <gtk/gtk.h>

/** Structure representant un groupe sur lequel s'applique des transformations */
typedef struct Groupe Groupe;
struct Groupe
{
    int id;
    Groupe* groupePere;
};

Groupe_initialiser( Groupe* groupe, Groupe* pere, int nb );

#endif //GROUPE_H
