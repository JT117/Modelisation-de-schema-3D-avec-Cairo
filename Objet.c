#include "Objet.h"
#include "Groupe.h"
#include "Rectangle.h"
#include "Pyramid.h"
#include "Scene.h"

#include <math.h>

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

    objet->type.cube = cube;
    objet->iSelectedForSegment = -2; /* objet non selectionné pour création d'un segment */

    objet->doitEtreDeselectionner = TRUE;
    objet->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
    objet->aTransfo = g_array_new( FALSE, FALSE, sizeof( Transfo* ) );
}

void Objet_est_un_Triangle( Objet* objet, Triangle* pTri )
{
	objet->eType = TRIANGLE;
    objet->type.triangle = pTri;
    objet->iSelectedForSegment = -2; /* objet non selectionné pour création d'un segment */

    objet->doitEtreDeselectionner = TRUE;
    objet->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
    objet->aTransfo = g_array_new( FALSE, FALSE, sizeof( Transfo* ) );
}

void Objet_est_un_Quadrilateral( Objet* objet, Quadrilateral* pQuadri )
{
	objet->eType = QUADRILATERAL;
    objet->iSelectedForSegment = -2; /* objet non selectionné pour création d'un segment */

    objet->type.quadrilateral = pQuadri;
    objet->doitEtreDeselectionner = TRUE;
    objet->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
    objet->aTransfo = g_array_new( FALSE, FALSE, sizeof( Transfo* ) );
}

void Objet_est_une_Pyramide( Objet* objet, Pyramid* pPyr )
{
    objet->type.pyramid = pPyr;
    objet->iSelectedForSegment = -2; /* objet non selectionné pour création d'un segment */

    objet->doitEtreDeselectionner = TRUE;
    objet->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
    objet->aTransfo = g_array_new( FALSE, FALSE, sizeof( Transfo* ) );
}

void Objet_est_un_Rectangle( Objet* pObj, Rectangle* pRect )
{
    pObj->eType = RECTANGLE;
    pObj->type.rectangle = pRect; /* sauvegarde pointeur sur objet */
    pObj->doitEtreDeselectionner = TRUE;
    pObj->iSelectedForSegment = -2; /* objet non selectionné pour création d'un segment */

    /* Allocation des tableaux dynamiques contenant les transformations à appliquer sur l'objet */
    Matrix_initIdentityMatrix(pObj->tTransfoMatrix);
    pObj->aTransfo = g_array_new( FALSE, FALSE, sizeof( Transfo* ) );
    pObj->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
}

void Objet_est_un_Segment( Objet* pObj, Segment* pSeg )
{
	pObj->eType = SEGMENT;
	pObj->type.segment = pSeg; /* sauvegarde pointeur sur objet */
    pObj->iSelectedForSegment = -2; /* objet non selectionné pour création d'un segment */

	pObj->doitEtreDeselectionner = TRUE;
	pObj->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
    pObj->aTransfo = g_array_new( FALSE, FALSE, sizeof( Transfo* ) );

}

void Objet_est_une_Sphere( Objet* pObj, Sphere* pSph )
{
	pObj->eType = SPHERE;
	pObj->type.sphere = pSph; /* sauvegarde pointeur sur objet */
    pObj->iSelectedForSegment = -2; /* objet non selectionné pour création d'un segment */

	pObj->doitEtreDeselectionner = TRUE;
	pObj->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
    pObj->aTransfo = g_array_new( FALSE, FALSE, sizeof( Transfo* ) );

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
		Segment_drawSegment( objet->type.segment, cr, cam);
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
	    if( objet->font == NULL )
	    {
	        cairo_set_source_rgb(cr, 0.0, 0.0, 0.9 );
            cairo_set_font_size( cr, 22 );
            cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD );
            cairo_move_to(cr, objet->x, objet->y );
	    }
	    else
	    {
	        char* dup = NULL;
	        dup = (char*)malloc( strlen(objet->font)+1 * sizeof( char ));
	        strcpy( dup, objet->font );
	        char delimit[1] = " ";
	        char* chaine = strtok( dup, delimit );
	        int i = 1;
	        char* font = NULL;
	        char* param1 = NULL;
	        char* param2 = NULL;
	        char* param3 = NULL;

	        while( chaine != NULL )
	        {
                if( i == 1 )
                {
                    font = (char*)malloc( strlen(chaine)+1 * sizeof(char) );
                    strcpy( font, chaine );
                }
                else if( i == 2 )
                {
                    param1 = (char*)malloc( strlen(chaine)+1 * sizeof(char) );
                    strcpy( param1, chaine);
                }
                else if( i == 3 )
                {
                    param2 = (char*)malloc( strlen(chaine)+1 * sizeof(char) );
                    strcpy( param2, chaine);
                }
                else if( i == 4 )
                {
                    param3 = (char*)malloc( strlen(chaine)+1 * sizeof(char) );
                    strcpy( param3, chaine);
                }
                chaine = strtok( NULL, delimit );
                i++;

	        }

            if( i == 3 )
            {
                cairo_select_font_face(cr, font, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL );
                cairo_set_font_size( cr, atof(param1) );
                free( font );
                free( param1 );
            }
            else if( i == 4 )
            {
                cairo_select_font_face(cr, font, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD );
                cairo_set_font_size( cr, atof(param2) );
                free( font );
                free( param1 );
                free( param2 );
            }
            else if( i == 5 )
            {
                cairo_select_font_face(cr, font, CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_BOLD );
                cairo_set_font_size( cr, atof(param3) );
                free( font );
                free( param1 );
                free( param2 );
                free( param3 );
            }

            cairo_set_source_rgb(cr, 0.0, 0.0, 0.9 );

            cairo_move_to(cr, objet->x, objet->y );
            free( dup );
	    }

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
    else if( objet->eType == SEGMENT )
    {
    	return FALSE;
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
	else if( objet->eType == SEGMENT )
	{
		Segment_updateCoordWolrd(objet);
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
	else if( objet->eType == SEGMENT )
	{
		Segment_transfo(objet->type.segment, tdTransfo);
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
	else if( objet->eType == SEGMENT )
	{
		Segment_transfoCenter(objet->type.segment, tdTransfo);
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
        tColor[0] = objet->type.rectangle->tColor[0];
        tColor[1] = objet->type.rectangle->tColor[1];
        tColor[2] = objet->type.rectangle->tColor[2];
        tColor[3] = objet->type.rectangle->tColor[3];
    }
    else if( objet->eType == SEGMENT )
    {
        tColor[0] = objet->type.segment->tColor[0];
        tColor[1] = objet->type.segment->tColor[1];
        tColor[2] = objet->type.segment->tColor[2];
        tColor[3] = objet->type.segment->tColor[3];
    }
    else if( objet->eType == SPHERE )
    {
        tColor[0] = objet->type.sphere->tColor[0];
        tColor[1] = objet->type.sphere->tColor[1];
        tColor[2] = objet->type.sphere->tColor[2];
        tColor[3] = objet->type.sphere->tColor[3];
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
         Color_setColor( objet->type.rectangle->tColor,(r/255),(g/255),(b/255),a);
    }
    else if( objet->eType == SEGMENT )
    {
         Color_setColor( objet->type.segment->tColor,(r/255),(g/255),(b/255),a);
    }
    else if( objet->eType == SPHERE )
    {
         Color_setColor( objet->type.sphere->tColor,(r/255),(g/255),(b/255),a);
    }
}

void Objet_sauvegarde( Objet* objet, FILE* fichier )
{
    if( objet->eType == CUBE )
    {
        fprintf( fichier, "%d\n%d %f %f %f %f\n", objet->eType, objet->numeroGroupe, objet->type.cube->Center.tdCoordGroup[0], objet->type.cube->Center.tdCoordGroup[1],
                                              objet->type.cube->Center.tdCoordGroup[2],  (float)abs( objet->type.cube->tPoint[0].tdCoordGroup[0] - objet->type.cube->tPoint[1].tdCoordGroup[0]) );
        fprintf( fichier, "%f %f %f %f\n", objet->type.cube->tColor[0]*255, objet->type.cube->tColor[1]*255, objet->type.cube->tColor[2]*255, objet->type.cube->tColor[3] );

    }
    else if( objet->eType == RECTANGLE )
    {
        fprintf( fichier, "%d\n%d %f %f %f %f %f\n", objet->eType, objet->numeroGroupe, objet->type.rectangle->Center.tdCoordGroup[0], objet->type.rectangle->Center.tdCoordGroup[1],
                                                     objet->type.rectangle->Center.tdCoordGroup[2], (float)abs( objet->type.rectangle->tPoint[0].tdCoordGroup[0] - objet->type.rectangle->tPoint[1].tdCoordGroup[0]) , (float)abs( objet->type.rectangle->tPoint[0].tdCoordGroup[1] - objet->type.rectangle->tPoint[2].tdCoordGroup[1]) );
        fprintf( fichier, "%f %f %f %f\n", objet->type.rectangle->tColor[0]*255, objet->type.rectangle->tColor[1]*255, objet->type.rectangle->tColor[2]*255, objet->type.rectangle->tColor[3] );
    }
    else if( objet->eType == SEGMENT )
    {
         fprintf( fichier, "%d\n%d %f %f %f %f %f %f\n", objet->eType, objet->numeroGroupe, objet->type.segment->tPoint[0].tdCoordGroup[0], objet->type.segment->tPoint[0].tdCoordGroup[1],
                                                  objet->type.segment->tPoint[0].tdCoordGroup[2], objet->type.segment->tPoint[1].tdCoordGroup[0], objet->type.segment->tPoint[1].tdCoordGroup[1],
                                                  objet->type.segment->tPoint[1].tdCoordGroup[2]   );
         fprintf( fichier, "%f %f %f %f\n", objet->type.segment->tColor[0]*255, objet->type.segment->tColor[1]*255, objet->type.segment->tColor[2]*255, objet->type.segment->tColor[3] );

         if( objet->type.segment->bDashed == TRUE )
         {
             fprintf( fichier, "%d ", 1 );
         }
         else
         {
             fprintf( fichier, "%d ", 0 );
         }

         if( objet->type.segment->bArrowed == TRUE )
         {
             fprintf( fichier, "%d\n", 1 );
         }
         else
         {
             fprintf( fichier, "%d\n", 0 );
         }

    }
    else if( objet->eType == SPHERE )
    {
        fprintf( fichier, "%d\n%d %f %f %f %f\n", objet->eType, objet->numeroGroupe, objet->type.sphere->Center.tdCoordGroup[0], objet->type.sphere->Center.tdCoordGroup[1],
                                                     objet->type.sphere->Center.tdCoordGroup[2], objet->type.sphere->dRadius );
        fprintf( fichier, "%f %f %f %f\n", objet->type.rectangle->tColor[0]*255, objet->type.sphere->tColor[1]*255, objet->type.sphere->tColor[2]*255, objet->type.sphere->tColor[3] );

    }
    else if( objet->eType == QUADRILATERAL )
    {
         fprintf( fichier, "%d\n%d %f %f %f %f %f %f %f %f %f %f %f %f\n", objet->eType, objet->numeroGroupe, objet->type.quadrilateral->tPoint[0].tdCoordGroup[0], objet->type.quadrilateral->tPoint[0].tdCoordGroup[1],
                                                  objet->type.quadrilateral->tPoint[0].tdCoordGroup[2], objet->type.quadrilateral->tPoint[1].tdCoordGroup[0], objet->type.quadrilateral->tPoint[1].tdCoordGroup[1],
                                                  objet->type.quadrilateral->tPoint[1].tdCoordGroup[2], objet->type.quadrilateral->tPoint[2].tdCoordGroup[0], objet->type.quadrilateral->tPoint[2].tdCoordGroup[1],
                                                  objet->type.quadrilateral->tPoint[2].tdCoordGroup[2], objet->type.quadrilateral->tPoint[3].tdCoordGroup[0], objet->type.quadrilateral->tPoint[3].tdCoordGroup[1],
                                                  objet->type.quadrilateral->tPoint[3].tdCoordGroup[2]   );
         fprintf( fichier, "%f %f %f %f\n", objet->type.quadrilateral->tColor[0]*255, objet->type.quadrilateral->tColor[1]*255, objet->type.quadrilateral->tColor[2]*255, objet->type.quadrilateral->tColor[3] );
    }
    else if( objet->eType == TEXTE )
    {
        fprintf( fichier, "%s %s %f %f\n", objet->texte, objet->font, objet->x, objet->y );
    }

    fprintf( fichier , "%d\n", (int)objet->aTransfo->len );

    int i = 0;
    for( i = 0; i < objet->aTransfo->len; i++ )
    {
        Transfo* transfo = g_array_index( objet->aTransfo, Transfo*, i );
        if( transfo->eTransfoType == ROTATION )
        {
            fprintf( fichier, "Rotation %f %f %f\n", transfo->x, transfo->y, transfo->z );
        }
        else if( transfo->eTransfoType == TRANSLATION )
        {
            fprintf( fichier, "Translation %f %f %f\n", transfo->x, transfo->y, transfo->z );
        }
        else if( transfo->eTransfoType == HOMOTHETIE )
        {
            fprintf( fichier, "Homothetie %f %f %f\n", transfo->x, transfo->y, transfo->z );
        }
    }
}

void Objet_restaure( FILE* fichier, struct Scene* scene )
{
    int idGroupe = 0;
    float r=0; float g=0; float b=0; float a=0; float dWidth=0; float x=0; float y=0; float z = 0;
    tCoord tdCenter;

    int typeObjet = -1;
    fscanf( fichier, "%d\n", &typeObjet );

    if( typeObjet == CUBE )
    {
        fscanf( fichier, "%d %f %f %f %f\n", &idGroupe, &x, &y, &z, &dWidth  );
        fscanf( fichier, "%f %f %f %f\n", &r, &g, &b , &a );

        Point_initCoord( tdCenter, x, y, z);
        Cube* pNewCube = Cube_createCube(tdCenter, dWidth, dWidth, dWidth);

        Groupe* groupe = Groupe_trouver_ById( scene, idGroupe );

        Scene_ajouter_cube( scene, pNewCube, groupe->id );
        Color_setColor(pNewCube->tColor,(r/255),(g/255),(b/255),a);

        Modification_modification_effectuer( scene );
    }
    else if( typeObjet == RECTANGLE )
    {
        float dHeight = 0;
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
    else if( typeObjet == SEGMENT )
    {
        float x2 =0, y2 = 0, z2 = 0;
        int dash = 0, arrow = 0;
        Segment* pNewSeg = NULL;
        tCoord tCoord1,tCoord2, tCenter;

        fscanf( fichier, "%d %f %f %f %f %f %f", &idGroupe, &x, &y, &z, &x2, &y2, &z2  );
        fscanf( fichier, "%f %f %f %f", &r, &g, &b , &a );
        fscanf( fichier , "%d %d", &dash, &arrow );

        /* Initialisation des coordonnées dans son propre repère*/
        Point_initCoord(tCoord1, x, y, z );
        Point_initCoord(tCoord2, x2, y2, z2 );
        Point_initCoord(tCenter, abs( x - x2)/2, abs( y - y2)/2, abs( z - z2)/2 ); /* A calibrer avec Ludo */
        pNewSeg = Segment_createSegment(tCenter,tCoord1,tCoord2);

        Color_setColor(pNewSeg->tColor,(r/255),(g/255),(b/255),a);

        if( dash )
        {
            Segment_setDashed( pNewSeg );
        }
        if( arrow )
        {
            Segment_setArrowed(pNewSeg);
        }

        Scene_ajouter_segment( scene, pNewSeg, GROUPE0 );

    }
    else if( typeObjet == SPHERE )
    {
        float dRadius = 0;
        tCoord tdCenter;

        fscanf( fichier, "%d %f %f %f %f", &idGroupe, &x, &y, &z, &dRadius  );
        fscanf( fichier, "%f %f %f %f", &r, &g, &b , &a );

        Sphere* pNewSph =NULL;

        Groupe* groupe = Groupe_trouver_ById( scene, idGroupe ) ;

        Point_initCoord( tdCenter, x-groupe->tCenterGroup.tdCoordWorld[0], y-groupe->tCenterGroup.tdCoordWorld[1], z-groupe->tCenterGroup.tdCoordWorld[2]); /* Récupération des coordonées du centre (par rapport au groupe)*/
        pNewSph = Sphere_createSphere( tdCenter, dRadius );
        Color_setColor(pNewSph->tColor,(r/255),(g/255),(b/255),a);

        Scene_ajouter_sphere( scene, pNewSph, groupe->id );
    }
    else if( typeObjet == QUADRILATERAL )
    {
        float x2 =0, y2 = 0, z2 = 0, x3=0, y3=0, z3=0, x4=0, y4=0, z4=0;
        tCoord tCoord1,tCoord2, tCenter;

        fscanf( fichier, "%d %f %f %f %f %f %f %f %f %f %f %f %f", &idGroupe, &x, &y, &z, &x2, &y2, &z2, &x3, &y3, &z3, &x4, &y4, &z4  );
        fscanf( fichier, "%f %f %f %f", &r, &g, &b , &a );
    }
    else if( typeObjet == TEXTE )
    {
        Objet* objet = (Objet*)malloc( 1 * sizeof( Objet ) );
        objet->eType = TEXTE;
        objet->doitEtreDeselectionner = TRUE;
        objet->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
        objet->texte = (char*)malloc( 60 * sizeof( char ) );
        objet->font = (char*)malloc( 60 * sizeof( char ) );

        fscanf( fichier, "%s %s %f %f", objet->texte, objet->font, &(objet->x), &(objet->y) );

        g_array_append_val( scene->tObjet, objet );
        scene->nbObjet++;

        Groupe* groupe = Groupe_trouver_ById( scene, 0 );

        Groupe_ajouter_Objet( groupe, objet );

        gtk_tree_store_append ( scene->store, objet->iter, groupe->iter );
        gtk_tree_store_set (scene->store, objet->iter, GROUPE, objet->texte, -1);
    }

    Objet* objet = g_array_index( scene->tObjet, Objet*, scene->nbObjet-1 );

    int nb = 0; int i =0;

    fscanf( fichier , "%d\n", &nb );

    for( i = 0; i < nb; i++ )
    {
        float a,b,c;
        char* temp = (char*)malloc( 20 * sizeof( char ) );
        Transfo* transfo = (Transfo*)malloc( 1 * sizeof( Transfo ) );
        fscanf( fichier, "%s %f %f %f", temp, &a, &b, &c );

        transfo->x = a;
        transfo->y = b;
        transfo->z = c;

        if( strcmp( temp, "Rotation") == 0 )
        {
            transfo->eTransfoType = ROTATION;
            tdMatrix tdTransfoMat,tdNewTransfo;

            tdMatrix tdTransfoMat,tdNewTransfo;
            Matrix_initIdentityMatrix(tdTransfoMat); /* Initialisation de la matrice de rotation */

            if( transfo->x != 0 )
            {
                Transformation_getMatrixRotation(tdNewTransfo, transfo->x, AXEX);
                Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
            }

            if( transfo->y != 0)
            {
                Transformation_getMatrixRotation( tdNewTransfo, transfo->y, AXEY );
                Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
            }

            if( transfo->z != 0)
            {
                Transformation_getMatrixRotation( tdNewTransfo,transfo->z, AXEZ );
                Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
            }

            //Objet_transfoCenter(objet, tdTransfoMat);   // on fait tourner le centre du repre objet
            Objet_transfo( objet , tdTransfoMat);    // ainsi qu l'intégralité de ses points
        }
        else if( strcmp( temp, "Translation") == 0  )
        {
            transfo->eTransfoType = TRANSLATION;
            tdMatrix tdTransfoMat,tdNewTransfo;
            Matrix_initIdentityMatrix(tdTransfoMat); /* Initialisation de la matrice de rotation */

            Transformation_getMatrixTranslation(tdNewTransfo, transfo->x, transfo->y, transfo->z );
            Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
            Objet_transfoCenter(objet, tdTransfoMat);   // on fait tourner le centre du repre objet
            Objet_transfo( objet , tdTransfoMat);    // ainsi qu l'intégralité de ses points
        }
        else if(  strcmp( temp, "Homothetie") == 0  )
        {
            transfo->eTransfoType = HOMOTHETIE;
            tdMatrix tdTransfoMat;
            if( transfo->x > 0 )
            {
                Transformation_getMatrixHomothety( tdTransfoMat, transfo->x);
            }
            else if( transfo->y > 0 )
            {
                Transformation_getMatrixHomothety( tdTransfoMat, transfo->y );
            }

            Objet_transfoCenter(objet, tdTransfoMat);   // on fait tourner le centre du repre objet
            Objet_transfo( objet , tdTransfoMat);    // ainsi qu l'intégralité de ses points
        }

        g_array_append_val( objet->aTransfo, transfo );
        free(temp);
        Modification_modification_effectuer( scene );
    }
}

