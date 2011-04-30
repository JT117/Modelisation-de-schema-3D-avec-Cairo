#ifndef OBJET_H
#define OBJET_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "Cube.h"
#include "Rectangle.h"

typedef struct Objet Objet;
struct Objet
{
    char* typeObjet;
    gboolean doitEtreDeselectionner;

    union{
            Cube* cube;
            Rectangle* rectangle;
         }type;

};

void Objet_detruire( Objet* objet );

/***** Fonctions d'initialisation des structures objets en fonction du type *****/
void Objet_est_un_Cube( Objet* objet, Cube* cube );
void Objet_est_un_Rectangle( Objet* pObj, Rectangle* pRect );

void Objet_dessiner_objet( Objet* objet, cairo_t* cr, InfoCamera* cam);

gboolean Objet_contient_point( Objet* objet, double x, double y );

void Objet_selection( Objet* objet );

void Objet_deselection( Objet* objet );


#endif  // OBJET_H
