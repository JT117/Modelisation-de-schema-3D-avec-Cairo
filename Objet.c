#include <math.h>

#include "Objet.h"
#include "Groupe.h"
#include "Rectangle.h"



/** Fonction qui libère un objet selon son type
 * @param objet, l'objet à liberer
 * @warning pour certain type d'objet rien n'est à liberer
 **/
void Objet_detruire( Objet* objet )
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
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
    objet->typeObjet = (char*)malloc( 5 * sizeof(char) );
    objet->type.cube = cube;
    strcpy( objet->typeObjet, "Cube" );
    objet->doitEtreDeselectionner = TRUE;
    objet->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
}

void Objet_est_un_Rectangle( Objet* pObj, Rectangle* pRect )
{
    pObj->typeObjet = (char*)malloc( 10 * sizeof(char) ); /* allocation chaine de type */
    pObj->type.rectangle = pRect; /* sauvegarde pointeur sur objet */
    strcpy( pObj->typeObjet, "Rectangle" );
    pObj->doitEtreDeselectionner = TRUE;

    /* Allocation des tableaux dynamiques contenant les transformations à appliquer sur l'objet */
    Matrix_initIdentityMatrix(pObj->tTransfoMatrix);

    pObj->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
}

void Objet_est_un_Segment( Objet* pObj, Segment* pSeg )
{
	pObj->typeObjet = (char*)malloc( 8 * sizeof(char) ); /* allocation chaine de type */
	pObj->type.segment = pSeg; /* sauvegarde pointeur sur objet */
	strcpy( pObj->typeObjet, "Segment" );
	pObj->doitEtreDeselectionner = TRUE;
	pObj->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
}

void Objet_est_une_Sphere( Objet* pObj, Sphere* pSph )
{
	pObj->typeObjet = (char*)malloc( 6 * sizeof(char) ); /* allocation chaine de type */
	pObj->type.sphere = pSph; /* sauvegarde pointeur sur objet */
	strcpy( pObj->typeObjet, "Sphere" );
	pObj->doitEtreDeselectionner = TRUE;
	pObj->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
}

/** Fonction qui dessine l'objet
 * @param objet, un pointeur sur l'objet à dessiner
 * @param cr, un pointeur sur le contexte cairo servant à dessiner sur la zoneDeDessin
 **/
void Objet_dessiner_objet( Objet* objet, cairo_t* cr, InfoCamera* cam)
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        Cube_drawCube( objet, cr, cam );
    }
    else if( strcmp( objet->typeObjet, "Rectangle" ) == 0 )
	{
		Rectangle_drawRectangle( objet, cr, cam);
	}
    else if( strcmp( objet->typeObjet, "Segment" ) == 0 )
	{
		Segment_drawSegment( objet, cr, cam);
	}
	else if( strcmp( objet->typeObjet, "Sphere" ) == 0 )
	{
	    Sphere_drawSphere( objet, cr, cam );
	}
	else if( strcmp( objet->typeObjet, "Texte" ) == 0 )
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
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        return Cube_Contient_Point( objet->type.cube, x, y, pCam );
    }
    else if(strcmp( objet->typeObjet, "Rectangle" ) == 0 )
    {
    	return Rectangle_Contient_Point( objet->type.rectangle, x, y,pCam);
    }
    else if( strcmp( objet->typeObjet, "Sphere" ) == 0 )
    {
        return Sphere_Contient_Point( objet->type.sphere, x, y,pCam);
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
    else if( strcmp( objet->typeObjet, "Rectangle" ) == 0 )
    {
        objet->type.rectangle->estSelectionne = TRUE;
    }
    else if( strcmp( objet->typeObjet, "Segment" ) == 0 )
    {
        objet->type.segment->estSelectionne = TRUE;
    }
    else if( strcmp( objet->typeObjet, "Sphere" ) == 0 )
    {
        objet->type.sphere->estSelectionne = TRUE;
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
    else if( strcmp( objet->typeObjet, "Rectangle" ) == 0 )
    {
        objet->type.rectangle->estSelectionne = FALSE;
    }
    else if( strcmp( objet->typeObjet, "Segment" ) == 0 )
    {
        objet->type.segment->estSelectionne = FALSE;
    }
    else if( strcmp( objet->typeObjet, "Sphere" ) == 0 )
    {
        objet->type.sphere->estSelectionne = FALSE;
    }
}

void Objet_updateCoordWorld(Objet* pObj)
{
	if( strcmp( pObj->typeObjet, "Cube" ) == 0 )
	{
		Cube_updateCoordWolrd(pObj);
	}
	else if( strcmp( pObj->typeObjet, "Rectangle" ) == 0 )
	{
		Rectangle_updateCoordWolrd(pObj);
	}
	else if( strcmp( pObj->typeObjet, "Sphere" ) == 0 )
	{
		//Sphere_transfo(objet->type.sphere, tdTransfo);
	}
}

void Objet_transfo(Objet* objet, tdMatrix tdTransfo)
{
	if( strcmp( objet->typeObjet, "Cube" ) == 0 )
	{
		Cube_transfo(objet->type.cube, tdTransfo);
	}
	else if( strcmp( objet->typeObjet, "Rectangle" ) == 0 )
	{
		Rectangle_transfo(objet->type.rectangle, tdTransfo);
	}
	else if( strcmp( objet->typeObjet, "Sphere" ) == 0 )
	{
		Sphere_transfo(objet->type.sphere, tdTransfo);
	}
}

void Objet_transfoCenter(Objet* objet, tdMatrix tdTransfo)
{
	if( strcmp( objet->typeObjet, "Cube" ) == 0 )
	{
		Cube_transfoCenter(objet->type.cube, tdTransfo);
	}
	else if( strcmp( objet->typeObjet, "Rectangle" ) == 0 )
	{
		Rectangle_transfoCenter(objet->type.rectangle, tdTransfo);
	}
	else if( strcmp( objet->typeObjet, "Sphere" ) == 0 )
	{
		Sphere_transfoCenter(objet->type.sphere, tdTransfo);
	}
}

void Objet_rotation( Objet* objet, double x, double y )
{
	tdMatrix tdTransfoMat, tdNewTransfo;
	double dAngleX = y/200;
	double dAngleY = x/200;
	double dAngleZ = 0.0;

	Matrix_initIdentityMatrix(tdTransfoMat); /* Initialisation de la matrice de rotation */
	/* On recherche la matrice de rotation qui va bien */
	if(dAngleX != 0)
	{
		Transformation_getMatrixRotation(tdNewTransfo, dAngleX, AXEX);
		Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
	}

	if(dAngleY != 0)
	{
		Transformation_getMatrixRotation(tdNewTransfo, dAngleY, AXEY);
		Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
	}

	if(dAngleZ != 0)
	{
		Transformation_getMatrixRotation(tdNewTransfo, dAngleZ, AXEZ);
		Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
	}


    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        Cube_rotateCube( objet->type.cube, y/200, x/200, 0 );
    }
    else if( strcmp( objet->typeObjet, "Rectangle" ) == 0 )
    {
    	Rectangle_transfo(objet->type.rectangle, tdTransfoMat);
        //Rectangle_rotate( objet->type.rectangle, 0, x/200, y/200 );
    }
}

void Objet_rotationWorld( Objet* objet, double x, double y )
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
    	Cube_rotateCubeWorld(objet->type.cube, 0, x/200, y/200);
    }
    else if( strcmp( objet->typeObjet, "Rectangle" ) == 0 )
	{
		Rectangle_rotateWorld(objet->type.rectangle, 0, x/200, y/200);
	}
    else if( strcmp( objet->typeObjet, "Sphere" ) == 0 )
    {
    	//Sphere_rotateWorld(objet->type.sphere, 0, x/200, y/200);
    }
}

void Objet_homothetie( Objet* objet, int ratio )
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        Cube_modSize( objet->type.cube, ratio );
    }
}

void Objet_get_Color( Objet* objet, Color tColor )
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        tColor[0] = objet->type.cube->tColor[0];
        tColor[1] = objet->type.cube->tColor[1];
        tColor[2] = objet->type.cube->tColor[2];
        tColor[3] = objet->type.cube->tColor[3];
    }
    else if( strcmp( objet->typeObjet, "Rectangle" ) == 0 )
    {
        /* TODO pareil que ci dessus*/
    }
    else if( strcmp( objet->typeObjet, "Segment" ) == 0 )
    {
        /* TODO pareil que ci dessus*/
    }
    else if( strcmp( objet->typeObjet, "Sphere" ) == 0 )
    {
        /* TODO pareil que ci dessus*/
    }
}

void Objet_set_Color( Objet* objet, double r, double g, double b, double a )
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        Color_setColor( objet->type.cube->tColor,(r/255),(g/255),(b/255),a);
    }
    else if( strcmp( objet->typeObjet, "Rectangle" ) == 0 )
    {
        /* TODO pareil que ci dessus*/
    }
    else if( strcmp( objet->typeObjet, "Segment" ) == 0 )
    {
        /* TODO pareil que ci dessus*/
    }
    else if( strcmp( objet->typeObjet, "Sphere" ) == 0 )
    {
        /* TODO pareil que ci dessus*/
    }
}

void Objet_sauvegarde( Objet* objet, FILE* fichier )
{
    if( strcmp( objet->typeObjet, "Cube" ) == 0 )
    {
        fprintf( fichier, "%s\n%d %f %f %f %f\n", objet->typeObjet, objet->numeroGroupe, objet->type.cube->Center.tdCoordGroup[0], objet->type.cube->Center.tdCoordGroup[1],
                                              objet->type.cube->Center.tdCoordGroup[3],  abs( objet->type.cube->tPoint[0].tdCoordGroup[0] - objet->type.cube->tPoint[1].tdCoordGroup[0]) );
        fprintf( fichier, "%f %f %f %f\n", objet->type.cube->tColor[0]*255, objet->type.cube->tColor[1]*255, objet->type.cube->tColor[2]*255, objet->type.cube->tColor[3] );
    }
    else if( strcmp( objet->typeObjet, "Rectangle" ) == 0 )
    {
        fprintf( fichier, "%s\n%d %f %f %f %f %f\n", objet->typeObjet, objet->numeroGroupe, objet->type.rectangle->Center.tdCoordGroup[0], objet->type.rectangle->Center.tdCoordGroup[1],
          /*TODO calculer largeur hauteur*/                                    objet->type.rectangle->Center.tdCoordGroup[3], 250.0, 250.0 );
        fprintf( fichier, "%f %f %f %f\n", objet->type.rectangle->tColor[0]*255, objet->type.rectangle->tColor[1]*255, objet->type.rectangle->tColor[2]*255, objet->type.rectangle->tColor[3] );
    }
    else if( strcmp( objet->typeObjet, "Segment" ) == 0 )
    {
        /* TODO pareil que ci dessus*/
    }
    else if( strcmp( objet->typeObjet, "Sphere" ) == 0 )
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
    fscanf( fichier, "%s", typeObjet );

    if( strcmp( typeObjet, "Cube" ) ==0 )
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
    else if( strcmp( typeObjet, "Rectangle" ) == 0 )
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


