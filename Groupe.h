#ifndef GROUPE_H
#define GROUPE_H

#define GROUPE0 0

#include <stdlib.h>
#include <gtk/gtk.h>

#include "MatrixTools.h"
#include "Modification.h"

struct Objet;
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

    /**Coordonnées du groupe dans le groupe parent  **/
    Point tCenterGroup;  /* coordonées du centre du repere du groupe */
    /** Tableau des transformation à appliquer sur les objets du groupe concerné ainsi que tous les groupes fils **/
    GArray* aTransfo;
    /** Matrice de transformation comprenant toutes les transfo du tableau aTransfo  Tfinale = T1*T2*...*Tn **/
    tdMatrix tTransfoMatrix;

    gboolean bVisited;  /* Booléen utilisé lors du parcours de l'arbre dans le cas de transformation */
};

/**
 * Calcul une matrice de transformation à partir du tableau des matrices.
 */
void Groupe_transfoCalc(Groupe* pGroup );

/**
 * AJout d'une transformation devant s'appliquer à l'ensemble du groupe.
 */
void Groupe_addTransfo(Groupe* pGroup, tdMatrix tdNouvTransfo);

void Groupe_initialiser( Groupe* groupe, Groupe* pere, int nb, double dX, double dY, double dZ );

void Groupe_enlever_fils( Groupe* groupe, Groupe* fils );

void Groupe_enlever_objet( Groupe* groupe, struct Objet* objet );

void Groupe_ajouter_Fils( Groupe* groupe, Groupe* fils );

void Groupe_ajouter_Objet( Groupe* groupe, struct Objet* objet );

void Groupe_detruire( Groupe* groupe );

void Groupe_sauvegarde( Groupe* groupe, FILE* fichier );

void Groupe_restaure( FILE* fichier, struct Scene* scene );

Groupe* Groupe_trouver( struct Scene* scene, char* nom );

/**
 * Va mettre le gboolean d'un groupe à FALSE.
 *
 *  @param pGroup Le groupe concerné
 */
void Groupe_unvisit(Groupe* pGroup);

Groupe* Groupe_trouver_ById(struct Scene* scene, int id );

void Groupe_transfoCenter(Groupe* pGroup, tdMatrix tdTransfo);

void Groupe_transfo(Groupe* pGroup, tdMatrix tdTransfo);
#endif //GROUPE_H
