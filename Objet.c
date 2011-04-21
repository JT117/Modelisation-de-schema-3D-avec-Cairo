#include "Objet.h"

void Objet_detruire( Objet* objet )
{
    if( strcmp( objet->type, "Cube" ) == 0 )
    {
        // pas d'allocation dynamique pour le cube de base
    }
}

void Objet_est_un_Cube( Objet* objet, Cube* cube )
{
    objet->cube = cube;
    strcpy( objet->type, "Cube" );
}

void Objet_dessiner_objet( Objet* objet, cairo_t* cr )
{
    if( strcmp( objet->type, "Cube" ) == 0 )
    {
        dessiner_Cube( objet->cube, cr );
    }
}

gboolean Objet_contient_point( Objet* objet, double x, double y )
{
    if( strcmp( objet->type, "Cube" ) == 0 )
    {
        return Cube_Contient_Point( objet->cube, x, y );
    }
    return TRUE;
}

void Objet_selection( Objet* objet )
{
    if( strcmp( objet->type, "Cube" ) == 0 )
    {
        objet->cube->estSelectionne = TRUE;
    }
}

void Objet_deselection( Objet* objet )
{
    if( strcmp( objet->type, "Cube" ) == 0 )
    {
        objet->cube->estSelectionne = FALSE;
    }
}
