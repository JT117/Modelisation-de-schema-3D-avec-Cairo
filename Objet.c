#include <math.h>

#include "Objet.h"
#include "Groupe.h"
#include "Rectangle.h"
#include "Pyramid.h"

/** Fonction qui libère un objet selon son type
 * @param objet, l'objet à liberer
 * @warning pour certain type d'objet rien n'est à liberer
 **/
void Objet_detruire( Objet* objet )
{
    if( objet->eType == CUBE )
    {
        // Pas d'alloc dynamique
    }
    //g_free( objet->texte );
}

/** Fonction qui initialise un objet de type Cube
 * @param objet, un pointeur sur l'objet à initialiser
 * @param cube, le cube avec lequel initialiser l'objet
 * @warning l'utilisateur doit liberer la memoire allouer
 **/
void Objet_est_un_Cube( Objet* objet, Cube* cube )
{
	objet->eType = CUBE;
    //objet->typeObjet = (char*)malloc( 5 * sizeof(char) );
    objet->type.cube = cube;
    //strcpy( objet->typeObjet, "Cube" );
    objet->doitEtreDeselectionner = TRUE;
    objet->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
}

void Objet_est_un_Triangle( Objet* objet, Triangle* pTri )
{
	objet->eType = TRIANGLE;
    //objet->typeObjet = (char*)malloc( 9 * sizeof(char) );
    objet->type.triangle = pTri;
   //strcpy( objet->typeObjet, "Triangle" );
    objet->doitEtreDeselectionner = TRUE;
    objet->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
}

void Objet_est_un_Quadrilateral( Objet* objet, Quadrilateral* pQuadri )
{
	objet->eType = QUADRILATERAL;
    objet->type.quadrilateral = pQuadri;
    objet->doitEtreDeselectionner = TRUE;
    objet->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
}

void Objet_est_une_Pyramide( Objet* objet, Pyramid* pPyr )
{
    //objet->typeObjet = (char*)malloc( 5 * sizeof(char) );
    objet->type.pyramid = pPyr;
    //strcpy( objet->typeObjet, "Pyramide" );
    objet->doitEtreDeselectionner = TRUE;
    objet->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
}

void Objet_est_un_Rectangle( Objet* pObj, Rectangle* pRect )
{
    pObj->eType = RECTANGLE;
    pObj->type.rectangle = pRect; /* sauvegarde pointeur sur objet */
    pObj->doitEtreDeselectionner = TRUE;

    /* Allocation des tableaux dynamiques contenant les transformations à appliquer sur l'objet */
    Matrix_initIdentityMatrix(pObj->tTransfoMatrix);

    pObj->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
}

void Objet_est_un_Segment( Objet* pObj, Segment* pSeg )
{
	pObj->eType = SEGMENT;
	pObj->type.segment = pSeg; /* sauvegarde pointeur sur objet */
	pObj->doitEtreDeselectionner = TRUE;
	pObj->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
}

void Objet_est_une_Sphere( Objet* pObj, Sphere* pSph )
{
	pObj->eType = SPHERE;
	pObj->type.sphere = pSph; /* sauvegarde pointeur sur objet */
	pObj->doitEtreDeselectionner = TRUE;
	pObj->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
}

/** Fonction qui dessine l'objet
 * @param objet, un pointeur sur l'objet à dessiner
 * @param cr, un pointeur sur le contexte cairo servant à dessiner sur la zoneDeDessin
 **/
void Objet_dessiner_objet( Objet* objet, cairo_t* cr, InfoCamera* cam)
{
    if( objet->eType == CUBE )
    {
        Cube_drawCube( objet, cr, cam );
    }
    else if( objet->eType == RECTANGLE )
	{
		Rectangle_drawRectangle( objet, cr, cam);
	}
    else if( objet->eType == SEGMENT )
	{
		Segment_drawSegment( objet, cr, cam);
	}
	else if( objet->eType == SPHERE )
	{
	    Sphere_drawSphere( objet, cr, cam );
	}
	else if( objet->eType == TRIANGLE )
	{
		Triangle_drawTriangle( objet, cr, cam );
	}
	else if( objet->eType == PYRAMID )
	{
		Pyramid_drawPyramid( objet, cr, cam );
	}
	else if( objet->eType == QUADRILATERAL )
	{
		Quadrilateral_drawQuadrilateral( objet, cr, cam );
	}
	else if( objet->eType == TEXTE )
	{
	    cairo_set_source_rgb(cr, 0.9, 0.9, 0.0 );
	    cairo_set_font_size( cr, 50 );
	    cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD );
	    cairo_move_to(cr, 50, 50 );
        cairo_show_text(cr, objet->texte );
	}
}

/** Fonction qui indique si un point est contenu par un objet
 * @param objet, l'objet que l'on cherche à tester
 * @param x, la coordonnées x du point à tester
 * @param y, la coordonnées y du point à tester
 **/
gboolean Objet_contient_point( Objet* objet, double x, double y, InfoCamera* pCam)
{
    if( objet->eType == CUBE )
    {
        return Cube_Contient_Point( objet->type.cube, x, y, pCam );
    }
    else if( objet->eType == RECTANGLE)
    {
    	return Rectangle_Contient_Point( objet->type.rectangle, x, y,pCam);
    }
    else if( objet->eType == SPHERE )
    {
        return Sphere_Contient_Point( objet->type.sphere, x, y,pCam);
    }
    else if( objet->eType == TRIANGLE )
	{
    	return Triangle_Contient_Point( objet->type.triangle, x, y,pCam);
	}
    else if( objet->eType == QUADRILATERAL )
	{
    	return Quadrilateral_Contient_Point( objet->type.quadrilateral, x, y,pCam);
	}
    else if( objet->eType == PYRAMID )
	{
    	return Pyramid_Contient_Point( objet->type.pyramid, x, y,pCam);
	}
    return TRUE;
}

/** Fonction qui mets le flag de selection de l'objet à TRUE
 * @param objet, l'objet à selectionner
 **/
void Objet_selection( Objet* objet )
{
    if( objet->eType == CUBE )
    {
        objet->type.cube->estSelectionne = TRUE;
    }
    else if( objet->eType == RECTANGLE )
    {
        objet->type.rectangle->estSelectionne = TRUE;
    }
    else if( objet->eType == SEGMENT )
    {
        objet->type.segment->estSelectionne = TRUE;
    }
    else if( objet->eType == SPHERE )
    {
        objet->type.sphere->estSelectionne = TRUE;
    }
    else if( objet->eType == TRIANGLE )
    {
	   objet->type.triangle->estSelectionne = TRUE;
	}
    else if( objet->eType == QUADRILATERAL )
    {
	   objet->type.quadrilateral->estSelectionne = TRUE;
	}
    else if( objet->eType == PYRAMID )
    {
	   objet->type.pyramid->estSelectionne = TRUE;
	}
}

/** Fonction qui mets le flag de selection de l'objet à FALSE
 * @param scene, un pointeur sur une scene initialisée
 **/
void Objet_deselection( Objet* objet )
{
    if( objet->eType == CUBE )
    {
        objet->type.cube->estSelectionne = FALSE;
    }
    else if( objet->eType == RECTANGLE )
    {
        objet->type.rectangle->estSelectionne = FALSE;
    }
    else if( objet->eType == SEGMENT )
    {
        objet->type.segment->estSelectionne = FALSE;
    }
    else if( objet->eType == SPHERE )
    {
        objet->type.sphere->estSelectionne = FALSE;
    }
    else if( objet->eType == TRIANGLE )
	{
		objet->type.triangle->estSelectionne = FALSE;
	}
    else if( objet->eType == QUADRILATERAL )
	{
		objet->type.quadrilateral->estSelectionne = FALSE;
	}
    else if( objet->eType == PYRAMID )
	{
		objet->type.pyramid->estSelectionne = FALSE;
	}
}

void Objet_updateCoordWorld(Objet* objet)
{
	if( objet->eType == CUBE )
	{
		Cube_updateCoordWolrd(objet);
	}
	else if( objet->eType == RECTANGLE )
	{
		Rectangle_updateCoordWolrd(objet);
	}
	else if( objet->eType == SPHERE )
	{
		//Sphere_transfo(objet->type.sphere, tdTransfo);
	}
	else if( objet->eType == TRIANGLE )
	{
		Triangle_updateCoordWolrd(objet);
	}
	else if( objet->eType == QUADRILATERAL )
	{
		Quadrilateral_updateCoordWolrd(objet);
	}
	else if( objet->eType == PYRAMID )
	{
		Pyramid_updateCoordWolrd(objet);
	}
}

void Objet_transfo(Objet* objet, tdMatrix tdTransfo)
{
	if( objet->eType == CUBE )
	{
		Cube_transfo(objet->type.cube, tdTransfo);
	}
	else if( objet->eType == RECTANGLE )
	{
		Rectangle_transfo(objet->type.rectangle, tdTransfo);
	}
	else if( objet->eType == SPHERE)
	{
		Sphere_transfo(objet->type.sphere, tdTransfo);
	}
	else if( objet->eType == TRIANGLE )
	{
		Triangle_transfo(objet->type.triangle, tdTransfo);
	}
	else if( objet->eType == QUADRILATERAL )
	{
		Quadrilateral_transfo(objet->type.quadrilateral, tdTransfo);
	}
	else if( objet->eType == PYRAMID )
	{
		Pyramid_transfo(objet->type.pyramid, tdTransfo);
	}

}

void Objet_transfoCenter(Objet* objet, tdMatrix tdTransfo)
{
	if( objet->eType == CUBE )
	{
		Cube_transfoCenter(objet->type.cube, tdTransfo);
	}
	else if( objet->eType == RECTANGLE )
	{
		Rectangle_transfoCenter(objet->type.rectangle, tdTransfo);
	}
	else if( objet->eType == SPHERE )
	{
		Sphere_transfoCenter(objet->type.sphere, tdTransfo);
	}
	else if( objet->eType == TRIANGLE )
	{
		Triangle_transfoCenter(objet->type.triangle, tdTransfo);
	}
	else if( objet->eType == QUADRILATERAL )
	{
		Quadrilateral_transfoCenter(objet->type.quadrilateral, tdTransfo);
	}
	else if( objet->eType == PYRAMID )
	{
		Pyramid_transfoCenter(objet->type.pyramid, tdTransfo);
	}
}

void Objet_homothetie( Objet* objet, int ratio )
{
    if( objet->eType == CUBE )
    {
        Cube_modSize( objet->type.cube, ratio );
    }
}

void Objet_get_Color( Objet* objet, Color tColor )
{
    if( objet->eType == CUBE )
    {
        tColor[0] = objet->type.cube->tColor[0];
        tColor[1] = objet->type.cube->tColor[1];
        tColor[2] = objet->type.cube->tColor[2];
        tColor[3] = objet->type.cube->tColor[3];
    }
    else if( objet->eType == RECTANGLE )
    {
        /* TODO pareil que ci dessus*/
    }
    else if( objet->eType == SEGMENT )
    {
        /* TODO pareil que ci dessus*/
    }
    else if( objet->eType == SPHERE )
    {
        /* TODO pareil que ci dessus*/
    }
}

void Objet_set_Color( Objet* objet, double r, double g, double b, double a )
{
    if( objet->eType == CUBE )
    {
        Color_setColor( objet->type.cube->tColor,(r/255),(g/255),(b/255),a);
    }
    else if( objet->eType == RECTANGLE )
    {
        /* TODO pareil que ci dessus*/
    }
    else if( objet->eType == SEGMENT )
    {
        /* TODO pareil que ci dessus*/
    }
    else if( objet->eType == SPHERE )
    {
        /* TODO pareil que ci dessus*/
    }
}

void Objet_sauvegarde( Objet* objet, FILE* fichier )
{
    if( objet->eType == CUBE )
    {
        fprintf( fichier, "%s\n%d %f %f %f %f\n", "Cube"/*objet->eType*/, objet->numeroGroupe, objet->type.cube->Center.tdCoordGroup[0], objet->type.cube->Center.tdCoordGroup[1],
                                              objet->type.cube->Center.tdCoordGroup[3],  abs( objet->type.cube->tPoint[0].tdCoordGroup[0] - objet->type.cube->tPoint[1].tdCoordGroup[0]) );
        fprintf( fichier, "%f %f %f %f\n", objet->type.cube->tColor[0]*255, objet->type.cube->tColor[1]*255, objet->type.cube->tColor[2]*255, objet->type.cube->tColor[3] );
    }
    else if( objet->eType == RECTANGLE )
    {
        fprintf( fichier, "%s\n%d %f %f %f %f %f\n", "Rectangle"/*objet->eType*/, objet->numeroGroupe, objet->type.rectangle->Center.tdCoordGroup[0], objet->type.rectangle->Center.tdCoordGroup[1],
          /*TODO calculer largeur hauteur*/                                    objet->type.rectangle->Center.tdCoordGroup[3], 250.0, 250.0 );
        fprintf( fichier, "%f %f %f %f\n", objet->type.rectangle->tColor[0]*255, objet->type.rectangle->tColor[1]*255, objet->type.rectangle->tColor[2]*255, objet->type.rectangle->tColor[3] );
    }
    else if( objet->eType == SEGMENT )
    {
        /* TODO pareil que ci dessus*/
    }
    else if( objet->eType == SPHERE )
    {
        /* TODO pareil que ci dessus*/
    }
}

void Objet_restaure( FILE* fichier, struct Scene* scene )
{
    int idGroupe = 0;
    float r, g, b, a, dWidth, x, y, z = 0;
    tCoord tdCenter;

    char* typeObjet = (char*)malloc( 35 * sizeof( char ) );
    //int typeObjet;
    fscanf( fichier, "%e", typeObjet );
    //fscanf( fichier, "%d", typeObjet );

    if( strcmp(typeObjet,"Cube") == 0 )
    {
        fscanf( fichier, "%d %f %f %f %f", &idGroupe, &x, &y, &z, &dWidth  );
        fscanf( fichier, "%f %f %f %f", &r, &g, &b , &a );

        Point_initCoord( tdCenter, x, y, z);
        Cube* pNewCube = Cube_createCube(tdCenter, dWidth, dWidth, dWidth);

        Groupe* groupe = Groupe_trouver_ById( scene, idGroupe );

        Scene_ajouter_cube( scene, pNewCube, groupe->id );
        Color_setColor(pNewCube->tColor,(r/255),(g/255),(b/255),a);

        Modification_modification_effectuer( scene );
    }
    else if( strcmp(typeObjet,"Rectangle") == 0 )
    {
        float dHeight;
        fscanf( fichier, "%d %f %f %f %f %f", &idGroupe, &x, &y, &z, &dWidth, &dHeight  );
        fscanf( fichier, "%f %f %f %f", &r, &g, &b , &a );

        Rectangle* pNewRect;
        tCoord coord;
        tCoord coord1;

        /* On recupere le groupe concerné par la création */
        Groupe* groupe = Groupe_trouver_ById( scene, idGroupe ) ;

        Point_initCoord( tdCenter, x, y, z); /* Récupération des coordonées du centre (par rapport au groupe)*/
        /* Initialisation des points aux coordonées qui vont bien pour le repere objet */
        Point_initCoord( coord, -dWidth/2, dHeight/2, 0 );         //Point_initCoord( coord, dX-dWidth/2, dY+dHeight/2, dZ );
        Point_initCoord( coord1, dWidth/2, -dHeight/2, 0 );    // Point_initCoord( coord1, dX + dWidth/2, dY - dHeight/2, dZ );
        pNewRect = Rectangle_createRectangle( coord, coord1, tdCenter );
        Color_setColor(pNewRect->tColor,(r/255),(g/255),(b/255),a);

        Scene_ajouter_rectangle( scene, pNewRect, groupe->id );
        Modification_modification_effectuer( scene );
    }
}
