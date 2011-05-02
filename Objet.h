#ifndef OBJET_H
#define OBJET_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "Cube.h"

/** Structure representant un objet dans l'interface graphique, servant à masquer l'implementation des differents objets à la GUI */
typedef struct Objet Objet;
struct Objet
{
    /** Un string contenant le type d'objet */
    char* typeObjet;
    /** Un flag indiquant si l'objet est à selectionner */
    gboolean doitEtreDeselectionner;
    /** Un tableau d'int contenant l'id des groupe dont l'objet fait partie */
    GArray* tGroupe;
    /** Un int indiquant le nombre de groupe dont l'objet fait partie */
    int nbGroupe;

    /** Union contenant le pointeur sur le bon type de l'objet */
    union{
            Cube* cube;
         }type;

};

/** Libere la memoire allouer dynamiquement pour un objet */
void Objet_detruire( Objet* objet );

/** Initialise un Objet Cube */
void Objet_est_un_Cube( Objet* objet, Cube* cube );

/** Fonction capable de dessiner n'importe quel objet */
void Objet_dessiner_objet( Objet* objet, cairo_t* cr );

/** Fonction indiquand si un point est contenu dans l'objet */
gboolean Objet_contient_point( Objet* objet, double x, double y );

/** Selectionne l'objet */
void Objet_selection( Objet* objet );

/** Deselection l'objet */
void Objet_deselection( Objet* objet );


#endif  // OBJET_H
