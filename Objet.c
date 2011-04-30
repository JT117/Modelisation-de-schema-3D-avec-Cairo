#include "Objet.h"

void Objet_detruire( Objet* objet )
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        // pas d'allocation dynamique pour le cube de base
    }
}

void Objet_est_un_Cube( Objet* objet, Cube* cube )
{
    objet->typeObjet = (char*)malloc( 5 * sizeof(char) );
    objet->type.cube = cube;
    strcpy( objet->typeObjet, "Cube" );
    objet->doitEtreDeselectionner = TRUE;
}

void Objet_est_un_Rectangle( Objet* pObj, Rectangle* pRect )
{
    pObj->typeObjet = (char*)malloc( 10 * sizeof(char) ); /* allocation chaine de type */
    pObj->type.rectangle = pRect; /* sauvegarde pointeur sur objet */
    strcpy( pObj->typeObjet, "Rectangle" );
    pObj->doitEtreDeselectionner = TRUE;
}

void Objet_dessiner_objet( Objet* objet, cairo_t* cr, InfoCamera* cam)
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        dessiner_Cube( objet->type.cube, cr, cam);
    }
    else if( strcmp( objet->typeObjet, "Rectangle" ) == 0 )
	{
		dessiner_Cube( objet->type.cube, cr, cam);
	}
}

gboolean Objet_contient_point( Objet* objet, double x, double y )
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        return Cube_Contient_Point( objet->type.cube, x, y );
    }
    else if(strcmp( objet->typeObjet, "Rectangle" ) == 0 )
    {
    	return Rectangle_Contient_Point( objet->type.rectangle, x, y );
    }
    return TRUE;
}

void Objet_selection( Objet* objet )
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        objet->type.cube->estSelectionne = TRUE;
    }
}

void Objet_deselection( Objet* objet )
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        objet->type.cube->estSelectionne = FALSE;
    }
}
