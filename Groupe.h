#ifndef GROUPE_H
#define GROUPE_H

#include <stdlib.h>
#include <gtk/gtk.h>

#include "Objet.h"
struct Scene;

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
    GtkTreeIter* iter;
    char* nom;
};

void Groupe_initialiser( Groupe* groupe, Groupe* pere, int nb );

void Groupe_enlever_fils( Groupe* groupe, Groupe* fils );

void Groupe_enlever_objet( Groupe* groupe, Objet* objet );

void Groupe_ajouter_Fils( Groupe* groupe, Groupe* fils );

void Groupe_ajouter_Objet( Groupe* groupe, Objet* objet );

void Groupe_detruire( Groupe* groupe );

Groupe* Groupe_trouver( struct Scene* scene, char* nom );

#endif //GROUPE_H
