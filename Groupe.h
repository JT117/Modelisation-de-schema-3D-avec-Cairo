#ifndef GROUPE_H
#define GROUPE_H

#include <stdlib.h>
#include <gtk/gtk.h>

#include "Objet.h"

/** Structure representant un groupe sur lequel s'applique des transformations */
typedef struct Groupe Groupe;
struct Groupe
{
    int id;
    Groupe* pere;
    GArray* tObjet;
    int nbObjet;
    GArray* tFils;
    int nbFils;
};

Groupe_initialiser( Groupe* groupe, Groupe* pere, int nb );

Groupe_enlever_fils( Groupe* groupe, Groupe* fils );

Groupe_enlever_objet( Groupe* groupe, Objet* objet );

Groupe_ajouter_Fils( Groupe* groupe, Groupe* fils );

Groupe_ajouter_Objet( Groupe* groupe, Objet* objet );

#endif //GROUPE_H
