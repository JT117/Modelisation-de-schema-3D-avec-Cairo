#include "Objet.h"

/** Fonction qui libère un objet selon son type
 * @param objet, l'objet à liberer
 * @warning pour certain type d'objet rien n'est à liberer
 **/
void Objet_detruire( Objet* objet )
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        // pas d'allocation dynamique pour le cube de base
    }
}

/** Fonction qui initialise un objet de type Cube
 * @param objet, un pointeur sur l'objet à initialiser
 * @param cube, le cube avec lequel initialiser l'objet
 * @warning l'utilisateur doit liberer la memoire allouer
 **/
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

void Objet_est_un_Segment( Objet* pObj, Segment* pSeg )
{
	pObj->typeObjet = (char*)malloc( 8 * sizeof(char) ); /* allocation chaine de type */
	pObj->type.segment = pSeg; /* sauvegarde pointeur sur objet */
	strcpy( pObj->typeObjet, "Segment" );
	pObj->doitEtreDeselectionner = TRUE;

}

/** Fonction qui dessine l'objet
 * @param objet, un pointeur sur l'objet à dessiner
 * @param cr, un pointeur sur le contexte cairo servant à dessiner sur la zoneDeDessin
 **/
void Objet_dessiner_objet( Objet* objet, cairo_t* cr, InfoCamera* cam)
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        //dessiner_Cube( objet->type.cube, cr, cam);
        Cube_drawCube( objet->type.cube, cr, cam );
    }
    else if( strcmp( objet->typeObjet, "Rectangle" ) == 0 )
	{
		Rectangle_drawRectangle( objet->type.rectangle, cr, cam);
	}
    else if( strcmp( objet->typeObjet, "Segment" ) == 0 )
	{
		Segment_drawSegment( objet->type.segment, cr, cam);
	}
}


/** Fonction qui indique si un point est contenu par un objet
 * @param objet, l'objet que l'on cherche à tester
 * @param x, la coordonnées x du point à tester
 * @param y, la coordonnées y du point à tester
 **/
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

/** Fonction qui mets le flag de selection de l'objet à TRUE
 * @param objet, l'objet à selectionner
 **/
void Objet_selection( Objet* objet )
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        objet->type.cube->estSelectionne = TRUE;
    }
}

/** Fonction qui mets le flag de selection de l'objet à FALSE
 * @param scene, un pointeur sur une scene initialisée
 **/
void Objet_deselection( Objet* objet )
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        objet->type.cube->estSelectionne = FALSE;
    }
}
