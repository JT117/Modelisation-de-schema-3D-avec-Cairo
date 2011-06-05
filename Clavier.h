#ifndef CLAVIER_H
#define CLAVIER_H

#include <gtk/gtk.h>

struct Scene;

/** Structure permettant de gérant le clavier */
typedef struct Clavier Clavier;
struct Clavier
{
    /** Tableau representant les touches actuellement appuyées */
    GArray* tTouche;
    /** Nombre de touche contenu dans le tableau */
    int nbTouche;
};


gboolean Clavier_est_appuyer( struct Scene* scene, char* touche );

void Clavier_initialiser( Clavier* clavier );

void Clavier_detruire( Clavier* clavier );

void Clavier_touche_appuyer( struct Scene* scene, char* nomTouche );

void Clavier_touche_relacher( struct Scene* scene, char* nomTouche );
#endif  //CLAVIER_H
