#ifndef OBJET_H
#define OBJET_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "Cube.h"

typedef union Objet Objet;
union Objet
{
    Cube* cube;               // !!!!!!!!!!!!!!!!!!!!!!! Union
    char* type;
};

void Objet_Cube( Objet* objet, Cube* cube );

void dessiner_Objet( Objet* objet, cairo_t* cr );

gboolean Objet_contient_Point( Objet* objet, double x, double y );

void Objet_Selection( Objet* objet );

void Objet_Deselection( Objet* objet );


#endif  // OBJET_H
