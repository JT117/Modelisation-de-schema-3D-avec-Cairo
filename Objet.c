#include "Objet.h"

void Objet_Cube( Objet* objet, Cube* cube )
{
    objet->cube = cube;
    strcpy( objet->type, "Cube" );
}

void dessiner_Objet( Objet* objet, cairo_t* cr )
{
    if( strcmp( objet->type, "Cube" ) == 0 )
    {
        dessiner_Cube( objet->cube, cr );
    }
}

gboolean Objet_contient_Point( Objet* objet, double x, double y )
{
    if( strcmp( objet->type, "Cube" ) == 0 )
    {
        return Cube_Contient_Point( objet->cube, x, y );
    }
}

void Objet_Selection( Objet* objet )
{
    if( strcmp( objet->type, "Cube" ) == 0 )
    {
        objet->cube->estSelectionne = TRUE;
    }
}

void Objet_Deselection( Objet* objet )
{
    if( strcmp( objet->type, "Cube" ) == 0 )
    {
        objet->cube->estSelectionne = FALSE;
    }
}

gboolean Objet_est_contenu( Objet* objet, double x1, double y1, double x2, double y2 )
{
    if( strcmp( objet->type, "Cube" ) == 0 )
    {
        return Cube_est_contenu( objet->cube, x1, y1, x2, y2 );
    }
}
