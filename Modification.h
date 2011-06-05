#ifndef MODIFICATION_H
#define MODIFICATION_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

struct Scene;

/** Structure gérant les fonctions annuler/refaire, détectant les modifications effectuer, et permettant de naviguer dans l'historique de modification */
typedef struct Modification Modification;
struct Modification
{
    /** Le numero du fichier de sauvegarde utilisé en ce moment */
    int actuel;
    /** Nombre maximum de fichiers utilisé pour l'historique des modifications */
    int nbMax;
    /** Pointeur sur l'item annuler dans le menu edition */
    GtkWidget* annuler;
    /** Pointeur sur l'item refaire dans le menu edition */
    GtkWidget* refaire;
};


void Modification_modification_effectuer( struct Scene* scene );

void Modification_initialiser( Modification* modif );

void Modification_annuler( struct Scene* scene );

void Modification_refaire( struct Scene* scene );

void Modification_detruire_temporaire( Modification* modif );
#endif  //MODIFICATION_H




