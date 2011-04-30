#include <math.h>

#include "Cube.h"

Cube* Cube_createCube(tdCoord tCenter, double dHeight,double dWidth, double dDepth)
{
	double dHalfH, dHalfW, dHalfD;
	Cube* pNewCube = NULL;

	if( (pNewCube = (Cube*)malloc(sizeof(Cube))) != NULL )
	{
		dHalfH = dHeight/2;
		dHalfW = dWidth/2;
		dHalfD = dDepth/2;

		/*Sauvegarde des infos sur les points dans notre structure */
		Point_init( &((pNewCube->tPoint)[0]), tCenter[0]-dHalfW, tCenter[1]+dHalfH, tCenter[2]-dHalfD);
		Point_init( &((pNewCube->tPoint)[1]), tCenter[0]+dHalfW, tCenter[1]+dHalfH, tCenter[2]-dHalfD);
		Point_init( &((pNewCube->tPoint)[2]), tCenter[0]+dHalfW, tCenter[1]-dHeight, tCenter[2]-dHalfD);
		Point_init( &((pNewCube->tPoint)[3]), tCenter[0]-dHalfW, tCenter[1]-dHeight, tCenter[2]-dHalfD);

		Point_init( &((pNewCube->tPoint)[4]), tCenter[0]-dHalfW, tCenter[1]+dHalfH, tCenter[2]+dHalfD);
		Point_init( &((pNewCube->tPoint)[5]), tCenter[0]+dHalfW, tCenter[1]+dHalfH, tCenter[2]+dHalfD);
		Point_init( &((pNewCube->tPoint)[6]), tCenter[0]+dHalfW, tCenter[1]-dHalfH, tCenter[2]+dHalfD);
		Point_init( &((pNewCube->tPoint)[7]), tCenter[0]-dHalfW, tCenter[1]-dHalfH, tCenter[2]+dHalfD);


		/* Init du centre du repere de la figure (centre de gravité du cube */
		Point_init( &(pNewCube->Center), tCenter[0], tCenter[1], tCenter[2]);

		/*Couleur par défaut, pas de transparence*/
		pNewCube->tColor[0]=153.0;
		pNewCube->tColor[1]=153.0;
		pNewCube->tColor[2]=255.0;
		pNewCube->tColor[3]=1.0;
	}
	else
	{
		/* TODO : Implémenter fatalError*/
	}
	return pNewCube;
}

void Cube_drawCube(Cube* pCube, cairo_t* cr, InfoCamera* pCam)
{
	tdCoord2D* pPointProj0 = NULL;tdCoord2D* pPointProj4 = NULL;
	tdCoord2D* pPointProj1 = NULL;tdCoord2D* pPointProj5 = NULL;
	tdCoord2D* pPointProj2 = NULL;tdCoord2D* pPointProj6 = NULL;
	tdCoord2D* pPointProj3 = NULL;tdCoord2D* pPointProj7 = NULL;

	pPointProj0 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[2]),pCam);
	pPointProj3 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[3]),pCam);
	pPointProj4 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[4]),pCam);
	pPointProj5 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[5]),pCam);
	pPointProj6 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[6]),pCam);
	pPointProj7 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[7]),pCam);

	/* Dessin de toutes les faces */
	/* FACE 1 : Construction du path */
	cairo_move_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
	cairo_line_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
	cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
	cairo_line_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
	cairo_line_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]); /* fermeture premiere face */

	cairo_set_source_rgba (cr, pCube->tColor[0], pCube->tColor[1], pCube->tColor[2] ,pCube->tColor[3]); /*Couleur */
	cairo_fill_preserve( cr );/*remplissage du rectangle avec path preservé*/
	cairo_set_line_width(cr,1.0);/* réglage taille de la ligne*/
	cairo_set_source_rgb ( cr, 0, 0, 0); /* couleur contour */
	cairo_stroke(cr); /* dessin contour, perte du path */

	/* FACE 2*/
	cairo_move_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
	cairo_line_to( cr, (*pPointProj5)[0], (*pPointProj5)[1]);
	cairo_line_to( cr, (*pPointProj6)[0], (*pPointProj6)[1]);
	cairo_line_to( cr, (*pPointProj7)[0], (*pPointProj7)[1]);
	cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);

	cairo_fill_preserve( cr );/*remplissage du rectangle avec path preservé*/
	cairo_set_line_width(cr,0.5);/* réglage taille de la ligne*/
	cairo_set_source_rgb ( cr, 0, 0, 0); /* couleur contour */
	cairo_stroke(cr); /* dessin contour, perte du path */


	cairo_set_source_rgba (cr, pCube->tColor[0], pCube->tColor[1], pCube->tColor[2] ,pCube->tColor[3]); /*Couleur */
	/* FACE 3*/
	cairo_move_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
	cairo_line_to( cr, (*pPointProj5)[0], (*pPointProj5)[1]);
	cairo_line_to( cr, (*pPointProj6)[0], (*pPointProj6)[1]);
	cairo_line_to( cr, (*pPointProj7)[0], (*pPointProj7)[1]);
	cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
	cairo_fill(cr);

	/* FACE 4*/
	cairo_move_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
	cairo_line_to( cr, (*pPointProj5)[0], (*pPointProj5)[1]);
	cairo_line_to( cr, (*pPointProj6)[0], (*pPointProj6)[1]);
	cairo_line_to( cr, (*pPointProj7)[0], (*pPointProj7)[1]);
	cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
	cairo_fill(cr);

	/* FACE 4*/
	cairo_move_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
	cairo_line_to( cr, (*pPointProj5)[0], (*pPointProj5)[1]);
	cairo_line_to( cr, (*pPointProj6)[0], (*pPointProj6)[1]);
	cairo_line_to( cr, (*pPointProj7)[0], (*pPointProj7)[1]);
	cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
	cairo_fill(cr);

	/* FACE 5*/
	cairo_move_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
	cairo_line_to( cr, (*pPointProj5)[0], (*pPointProj5)[1]);
	cairo_line_to( cr, (*pPointProj6)[0], (*pPointProj6)[1]);
	cairo_line_to( cr, (*pPointProj7)[0], (*pPointProj7)[1]);
	cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
	cairo_fill(cr);

	/* FACE 6*/
	cairo_move_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
	cairo_line_to( cr, (*pPointProj5)[0], (*pPointProj5)[1]);
	cairo_line_to( cr, (*pPointProj6)[0], (*pPointProj6)[1]);
	cairo_line_to( cr, (*pPointProj7)[0], (*pPointProj7)[1]);
	cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
	cairo_fill(cr);

	/* On relit les points manquants */
	cairo_move_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
	cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
	cairo_set_source_rgb ( cr, 0, 0, 0); /* couleur contour */
	cairo_set_line_width(cr,0.5);/* réglage taille de la ligne*/
	cairo_stroke(cr);

	cairo_move_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
	cairo_line_to( cr, (*pPointProj5)[0], (*pPointProj5)[1]);
	cairo_set_source_rgb ( cr, 0, 0, 0); /* couleur contour */
	cairo_set_line_width(cr,0.5);/* réglage taille de la ligne*/
	cairo_stroke(cr);

	cairo_move_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
	cairo_line_to( cr, (*pPointProj7)[0], (*pPointProj7)[1]);
	cairo_set_source_rgb ( cr, 0, 0, 0); /* couleur contour */
	cairo_set_line_width(cr,0.5);/* réglage taille de la ligne*/
	cairo_stroke(cr);

	cairo_move_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
	cairo_line_to( cr, (*pPointProj6)[0], (*pPointProj6)[1]);
	cairo_set_source_rgb ( cr, 0, 0, 0); /* couleur contour */
	cairo_set_line_width(cr,0.5);/* réglage taille de la ligne*/
	cairo_stroke(cr);


	free(pPointProj0);	free(pPointProj4);
	free(pPointProj1);	free(pPointProj5);
	free(pPointProj2);	free(pPointProj6);
	free(pPointProj3);	free(pPointProj7);
}

void initialiser_Cube( Cube* cCube, double dX, double dY, double dZ, double dCote )
{
    cCube->tPoint[0].x = dX;
    cCube->tPoint[0].y = dY;
    cCube->tPoint[0].z = dZ;

    cCube->tPoint[1].x = dX + dCote;
    cCube->tPoint[1].y = dY;
    cCube->tPoint[1].z = dZ;

    cCube->tPoint[2].x = dX + dCote;
    cCube->tPoint[2].y = dY + dCote;
    cCube->tPoint[2].z = dZ;

    cCube->tPoint[3].x = dX;
    cCube->tPoint[3].y = dY + dCote;
    cCube->tPoint[3].z = dZ;

    cCube->tPoint[4].x = dX;
    cCube->tPoint[4].y = dY;
    cCube->tPoint[4].z = dZ + dCote;

    cCube->tPoint[5].x = dX + dCote;
    cCube->tPoint[5].y = dY;
    cCube->tPoint[5].z = dZ + dCote;

    cCube->tPoint[6].x = dX + dCote;
    cCube->tPoint[6].y = dY + dCote;
    cCube->tPoint[6].z = dZ + dCote;

    cCube->tPoint[7].x = dX;
    cCube->tPoint[7].y = dY + dCote;
    cCube->tPoint[7].z = dZ + dCote;

    cCube->estSelectionne = FALSE;

}

void dessiner_Cube(Cube* cCube, cairo_t* cr, InfoCamera* cam)
{
   if( !cCube->estSelectionne )
   {
        cairo_set_source_rgb ( cr, 0, 0, 0);   // On definie la couleur du trait
        cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
        cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
        cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
        cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y );
        cairo_line_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y ); // Face 1 dessinée

        cairo_fill( cr );
        cairo_set_source_rgb ( cr, 0.255, 0, 0);

        cairo_move_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
        cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
        cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
        cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
        cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );// Face 2 dessinée

        cairo_fill( cr );
        cairo_set_source_rgb ( cr, 0.196, 0.804, 0.196);

        cairo_move_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y );
        cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
        cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
        cairo_line_to( cr, cCube->tPoint[7].x, cCube->tPoint[7].y );
        cairo_line_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y ); // Face 3 dessinée

        cairo_fill( cr );
        cairo_set_source_rgb ( cr, 0, 0, 0.9);

        cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
        cairo_line_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y );
        cairo_line_to( cr, cCube->tPoint[7].x, cCube->tPoint[7].y );
        cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y );
        cairo_line_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y ); // Face 4 dessinée

        cairo_fill( cr );
        cairo_set_source_rgb ( cr, 0.255, 0.9, 0);

        cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
        cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
        cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
        cairo_line_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y );
        cairo_line_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y ); // Face 5 dessinée

        cairo_fill( cr );
        cairo_set_source_rgb ( cr, 255, 0, 255);

        cairo_move_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y );
        cairo_line_to( cr, cCube->tPoint[7].x, cCube->tPoint[7].y );
        cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
        cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
        cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y ); // Face 6 dessinée

        cairo_fill( cr );
    }
    else if( cCube->estSelectionne )
    {
        cairo_set_source_rgb ( cr, 255, 0, 0);

        cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
        cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
        cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
        cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y );
        cairo_line_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y ); // Face 1 dessinée

        cairo_move_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
        cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
        cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
        cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
        cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );// Face 2 dessinée

        cairo_move_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y );
        cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
        cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
        cairo_line_to( cr, cCube->tPoint[7].x, cCube->tPoint[7].y );
        cairo_line_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y ); // Face 3 dessinée

        cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
        cairo_line_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y );
        cairo_line_to( cr, cCube->tPoint[7].x, cCube->tPoint[7].y );
        cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y );
        cairo_line_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y ); // Face 4 dessinée

        cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
        cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
        cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
        cairo_line_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y );
        cairo_line_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y ); // Face 5 dessinée

        cairo_move_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y );
        cairo_line_to( cr, cCube->tPoint[7].x, cCube->tPoint[7].y );
        cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
        cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
        cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y ); // Face 6 dessinée

        cairo_set_source_rgb ( cr, 0, 255, 0);
        cairo_fill_preserve( cr );

        cairo_set_source_rgb ( cr, 255, 0, 0);
        cairo_stroke( cr );
    }
}

void rotation_Cube( Cube* cCube, double dAngle, int iAxe )
{
    double dDecallage_X = cCube->tPoint[0].x + ( cCube->tPoint[1].x - cCube->tPoint[0].x ) / 2;
    double dDecallage_Y = cCube->tPoint[0].y + ( cCube->tPoint[1].y - cCube->tPoint[0].y ) / 2;
    double dDecallage_Z = cCube->tPoint[0].z + ( cCube->tPoint[1].z - cCube->tPoint[0].z ) / 2;

    if( iAxe == 1 ) // X
    {
        int i = 0;

        for( i = 0; i < 8; i++ )
        {
            rotation_X( &cCube->tPoint[i], dDecallage_Y, dDecallage_Z, dAngle );
        }
    }
    else if( iAxe == 2 ) // Y
    {
        int i = 0;

        for( i = 0; i < 8; i++ )
        {
            rotation_Y( &cCube->tPoint[i], dDecallage_X, dDecallage_Z, dAngle );
        }
    }
    else if( iAxe == 3 ) // Z
    {
        int i = 0;

        for( i = 0; i < 8; i++ )
        {
            rotation_Z( &cCube->tPoint[i], dDecallage_X, dDecallage_Y, dAngle );
        }
    }
}

void rotation_X( Point* pPoint, double dDecallage_Y, double dDecallage_Z, double dAngle )
{
    pPoint->y -= dDecallage_Y;
    pPoint->z -= dDecallage_Z;

    pPoint->y = cos( dAngle ) * pPoint->y + sin( dAngle ) * pPoint->z;
    pPoint->z = -sin( dAngle ) * pPoint->y + cos( dAngle ) * pPoint->z;

    pPoint->y += dDecallage_Y;
    pPoint->z += dDecallage_Z;

}

void rotation_Y( Point* pPoint, double dDecallage_X, double dDecallage_Z, double dAngle )
{
    pPoint->x -= dDecallage_X;
    pPoint->z -= dDecallage_Z;

    pPoint->x = cos( dAngle ) * pPoint->x - sin( dAngle ) * pPoint->z;
    pPoint->z = sin( dAngle ) * pPoint->x + cos( dAngle ) * pPoint->z;

    pPoint->x += dDecallage_X;
    pPoint->z += dDecallage_Z;

}

void rotation_Z( Point* pPoint, double dDecallage_X, double dDecallage_Y, double dAngle )
{
    pPoint->x -= dDecallage_X;
    pPoint->y -= dDecallage_Y;

    pPoint->x = cos( dAngle ) * pPoint->x + sin( dAngle ) * pPoint->y;
    pPoint->y = -sin( dAngle ) * pPoint->x + cos( dAngle ) * pPoint->y;

    pPoint->x += dDecallage_X;
    pPoint->y += dDecallage_Y;

}

gboolean Cube_Contient_Point( Cube* cCube, double x, double y )
{
    gboolean est_contenu = FALSE;

    est_contenu = est_contenu || est_dans_face( cCube->tPoint[0], cCube->tPoint[1], cCube->tPoint[2], cCube->tPoint[3], x, y )
                              || est_dans_face( cCube->tPoint[1], cCube->tPoint[5], cCube->tPoint[6], cCube->tPoint[2], x, y )
                              || est_dans_face( cCube->tPoint[4], cCube->tPoint[5], cCube->tPoint[6], cCube->tPoint[7], x, y )
                              || est_dans_face( cCube->tPoint[0], cCube->tPoint[4], cCube->tPoint[7], cCube->tPoint[3], x, y )
                              || est_dans_face( cCube->tPoint[0], cCube->tPoint[1], cCube->tPoint[5], cCube->tPoint[4], x, y )
                              || est_dans_face( cCube->tPoint[3], cCube->tPoint[2], cCube->tPoint[6], cCube->tPoint[7], x, y );
    return est_contenu;
}

gboolean est_dans_face( Point a, Point b, Point c, Point d, double x, double y )
{
    int nb = 0;

    nb += scalaire_result( a, b, x, y );
    nb += scalaire_result( b, c, x, y );
    nb += scalaire_result( c, d, x, y );
    nb += scalaire_result( d, a, x, y );

    if( nb == 4 || nb == -4 )
    {
        return TRUE;
    }
    else return FALSE;
}

int scalaire_result( Point a, Point b, int x, int y )
{
    Point ab;
    Point ap;
    double produitScalaire = 0.0;

    ab.x = b.x - a.x;
    ab.y = b.y - a.y;

    ap.x = x - a.x;
    ap.y = y - a.y;

    produitScalaire = ab.x * ap.x + ab.y * ap.y;

    if( produitScalaire > 0 )
    {
        return 1;
    }
    else if( produitScalaire < 0 )
    {
        return -1;
    }
    else return 0;
}







