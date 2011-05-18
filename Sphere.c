#include <gtk/gtk.h>
#include <math.h>

#include "Sphere.h"
#include "ProjectionTools.h"
#include "Point.h"

Sphere* Sphere_createSphere(tdCoord tCenter, double dRadius)
{
	Sphere* pNewSphere = NULL;

	if( (pNewSphere = (Sphere*)malloc(sizeof(Sphere))) != NULL )
	{
		/*Sauvegarde des infos sur les points dans notre structure */
		Point_init( &((pNewSphere->tPoint)[0]), tCenter[0], tCenter[1], tCenter[2]);
		Point_init( &((pNewSphere->tPoint)[0]), tCenter[0], tCenter[1], tCenter[2]+dRadius);
		//pNewSphere->dRadius = dRadius;

		/* Init du centre du repere de la figure (centre de gravité du Sphere) */
		Point_init( &(pNewSphere->Center), tCenter[0], tCenter[1], tCenter[2]);

		/*Couleur par défaut, pas de transparence*/
		pNewSphere->tColor[0]=0.4;
		pNewSphere->tColor[1]=0.4;
		pNewSphere->tColor[2]=0.8;
		pNewSphere->tColor[3]=1.0;

		pNewSphere->estSelectionne = FALSE;
	}
	else
	{
		/* TODO : Implémenter fatalError*/
	}
	return pNewSphere;
}

void Sphere_drawSphere(Sphere* pSphere, cairo_t* cr, InfoCamera* pCam)
{
	double dRadius=0;
	tdCoord2D* pPointProj0 = NULL;
	tdCoord2D* pPointProj1 = NULL;

	/* Projection des points du rayon de la sphere */
	pPointProj0 = ProjectionTools_getPictureCoord(&((pSphere->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pSphere->tPoint)[1]),pCam);

	dRadius = Point_euclideanDistance2D((*pPointProj0),(*pPointProj1));

	cairo_arc (cr, (*pPointProj0)[0], (*pPointProj0)[1], dRadius, 0.0, 2*M_PI);

	cairo_set_source_rgba (cr, pSphere->tColor[0], pSphere->tColor[1], pSphere->tColor[2] , pSphere->tColor[3]); /*Couleur */
	cairo_fill_preserve( cr );/*remplissage du rectangle avec path preservé*/
	cairo_set_line_width(cr,0.8);/* réglage taille de la ligne*/


	cairo_set_source_rgb ( cr, 0, 0, 0); /* couleur contour */
	cairo_stroke(cr); /* dessin contour, perte du path */

	free(pPointProj0);
	free(pPointProj1);
}

void Sphere_rotateSphereWorld(Sphere* pSphere, double dAngleX, double dAngleY, double dAngleZ)
{
	int iLoop;
	tdMatrix tdMatTransfo;
	tdCoord tdCoordApTransfo;

	Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);

	if(dAngleX != 0)
	{
		/*Récupération de la matrice de rotation qui va bien */
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleX, axeX);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<2 ;iLoop++)
		{
			/*Transformation dans le repere du monde*/
			Matrix_multiMatrixVect(tdMatTransfo, pSphere->tPoint[iLoop].tdCoordWorld, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pSphere->tPoint[iLoop].tdCoordWorld[0] = tdCoordApTransfo[0];
			pSphere->tPoint[iLoop].tdCoordWorld[1] = tdCoordApTransfo[1];
			pSphere->tPoint[iLoop].tdCoordWorld[2] = tdCoordApTransfo[2];

			/* réinitialisation coord après transformation pour le  point suivant */
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}
	}

	if(dAngleY != 0)
	{
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleY, axeY);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<2 ;iLoop++)
		{
			/*Transformation dans le repere du monde*/
			Matrix_multiMatrixVect(tdMatTransfo, pSphere->tPoint[iLoop].tdCoordWorld, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pSphere->tPoint[iLoop].tdCoordWorld[0] = tdCoordApTransfo[0];
			pSphere->tPoint[iLoop].tdCoordWorld[1] = tdCoordApTransfo[1];
			pSphere->tPoint[iLoop].tdCoordWorld[2] = tdCoordApTransfo[2];

			/* réinitialisation coord après transformation pour le  point suivant */
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}

	}

	if(dAngleZ != 0)
	{
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleZ, axeZ);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<2 ;iLoop++)
		{
			/*Transformation dans le repere du monde*/
			Matrix_multiMatrixVect(tdMatTransfo, pSphere->tPoint[iLoop].tdCoordWorld, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pSphere->tPoint[iLoop].tdCoordWorld[0] = tdCoordApTransfo[0];
			pSphere->tPoint[iLoop].tdCoordWorld[1] = tdCoordApTransfo[1];
			pSphere->tPoint[iLoop].tdCoordWorld[2] = tdCoordApTransfo[2];

			/* réinitialisation coord après transformation pour le  point suivant */
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}
	}
}

void Sphere_modSize(Sphere* pSphere, double dRatio)
{
	int iLoop;
	tdMatrix tdMatTransfo, tdMatPassRepObj;
	tdCoord tdCoordRepObj, tdCoordApTransfo;

	/* Initialisation des coordonées*/
	Point_initCoord(tdCoordRepObj, 0, 0, 0);
	Point_initCoord(tdCoordApTransfo, 0, 0, 0);

	/* Construction de la matrice de passage World -> Repere objet */
	Matrix_initMatrix(tdMatPassRepObj); /* initialisation de la matrice de passage dans repere objet*/
	tdMatPassRepObj[0][3] = -pSphere->Center.tdCoordWorld[0];
	tdMatPassRepObj[1][3] = -pSphere->Center.tdCoordWorld[1];
	tdMatPassRepObj[2][3] = -pSphere->Center.tdCoordWorld[2];

	if(dRatio != 1)
	{
		/*Récupération de la matrice d'homothétie*/
		TransfoTools_getMatrixHomothety(tdMatTransfo, dRatio);

		/* On effectue la transformation pour tous  les points du Sphere */
		for(iLoop=0 ; iLoop<8 ;iLoop++)
		{
			/* Tout d'abord recherche des coordonnées dans le repere de l'objet*/
			Matrix_multiMatrixVect(tdMatPassRepObj, pSphere->tPoint[iLoop].tdCoordWorld, tdCoordRepObj);
			/*Puis transformation, toujours dans le repere objet*/
			Matrix_multiMatrixVect(tdMatTransfo, tdCoordRepObj, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pSphere->tPoint[iLoop].tdCoordWorld[0] = pSphere->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]);
			pSphere->tPoint[iLoop].tdCoordWorld[1] = pSphere->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]);
			pSphere->tPoint[iLoop].tdCoordWorld[2] = pSphere->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]);

			/* on réinitialise les vecteurs contenant les infos sur la transformation */
			Point_initCoord(tdCoordRepObj, 0, 0, 0);
			Point_initCoord(tdCoordApTransfo, 0, 0, 0);
		}
	}
}

gboolean Sphere_Contient_Point( Sphere* cSphere, double x, double y )
{
    return TRUE;
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
