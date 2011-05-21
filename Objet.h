#ifndef OBJET_H
#define OBJET_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "Rectangle.h"
#include "Cube.h"
#include "Segment.h"
#include "Sphere.h"

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
    /** iteration de l'arbre pour placement dans l'arbre d'objet */
    GtkTreeIter* iter;
    /** Union contenant le pointeur sur le bon type de l'objet */
    union{
            Cube* cube;
            Rectangle* rectangle;
            Segment* segment;
            Sphere* sphere;
         }type;
};

/** Libere la memoire allouer dynamiquement pour un objet */
void Objet_detruire( Objet* objet );


/** Initialise un Objet Cube */
void Objet_est_un_Cube( Objet* objet, Cube* cube );

/** Initialise un Objet Rectangle */
void Objet_est_un_Rectangle( Objet* pObj, Rectangle* pRect );
void Objet_est_un_Segment( Objet* pObj, Segment* pSeg );
void Objet_est_une_Sphere( Objet* pObj, Sphere* pSph );

/** Fonction capable de dessiner n'importe quel objet */
void Objet_dessiner_objet( Objet* objet, cairo_t* cr, InfoCamera* cam);


gboolean Objet_contient_point( Objet* objet, double x, double y, InfoCamera* pCam);

/** Selectionne l'objet */
void Objet_selection( Objet* objet );

/** Deselection l'objet */
void Objet_deselection( Objet* objet );

void Objet_rotation( Objet* objet, double x, double y );
void Objet_homothetie( Objet* objet, int ratio );

#endif  // OBJET_H
