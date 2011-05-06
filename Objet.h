#ifndef OBJET_H
#define OBJET_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "Rectangle.h"
#include "Cube.h"

/** Structure representant un objet dans l'interface graphique, servant à masquer l'implementation des differents objets à la GUI */
typedef struct Objet Objet;
struct Objet
{
    /** Un string contenant le type d'objet */
    char* typeObjet;
    /** Un flag indiquant si l'objet est à selectionner */
    gboolean doitEtreDeselectionner;
    /** Un int indiquant l'id du dernier groupe dont l'objet fait partie */
    int numeroGroupe;
    /** Union contenant le pointeur sur le bon type de l'objet */
    union{
            Cube* cube;
            Rectangle* rectangle;
         }type;

};

/** Libere la memoire allouer dynamiquement pour un objet */
void Objet_detruire( Objet* objet );

/** Initialise un Objet Cube */
void Objet_est_un_Cube( Objet* objet, Cube* cube );

/** Initialise un Objet Rectangle */
void Objet_est_un_Rectangle( Objet* pObj, Rectangle* pRect );

/** Fonction capable de dessiner n'importe quel objet */
void Objet_dessiner_objet( Objet* objet, cairo_t* cr, InfoCamera* cam);

/** Fonction indiquand si un point est contenu dans l'objet */
gboolean Objet_contient_point( Objet* objet, double x, double y );

/** Selectionne l'objet */
void Objet_selection( Objet* objet );

/** Deselection l'objet */
void Objet_deselection( Objet* objet );


#endif  // OBJET_H
