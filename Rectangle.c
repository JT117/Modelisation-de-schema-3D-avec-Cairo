#include <stdio.h>
#include <stdlib.h>

#include "Rectangle.h"
#include "MatrixTools.h"
#include "TransfoTools.h"
#include "Point.h"

Rectangle* Rectangle_createRectangle(tdCoord tdCorner1,tdCoord tdCorner2)
{
	Rectangle* pNewRect = NULL;

	if( (pNewRect = (Rectangle*)malloc(sizeof(Rectangle))) != NULL )
	{
		/* Vérification : est ce que tdCorner1 correspond bien au sommet haut gauche ? */
		if(tdCorner1[1]>tdCorner2[1]) /* Verif effectuée sur l'axe des Y (arbitrairement axe vertical)*/
		{
			/*Sauvegarde des infos sur les points dans notre structure */
			Point_init( &((pNewRect->tPoint)[0]), tdCorner1[0], tdCorner1[1], tdCorner1[2]);
			Point_init( &((pNewRect->tPoint)[1]), tdCorner2[0], tdCorner1[1], tdCorner2[2]);
			Point_init( &((pNewRect->tPoint)[2]), tdCorner2[0], tdCorner2[1], tdCorner2[2]);
			Point_init( &((pNewRect->tPoint)[3]), tdCorner1[0], tdCorner2[1], tdCorner1[2]);

			/* Init du centre du repere de la figure (centre de gravité du rectangle) */
			Point_init( &(pNewRect->Center), tdCorner1[0]+((tdCorner2[0]-tdCorner1[0])/2),tdCorner1[1]+((tdCorner2[1]-tdCorner1[1])/2), tdCorner1[2]+((tdCorner2[2]-tdCorner1[2])/2));

			/*Couleur par défaut, pas de transparence*/
			pNewRect->tColor[0]=1.0;
			pNewRect->tColor[1]=0.39;
			pNewRect->tColor[2]=0.13;
			pNewRect->tColor[3]=0.7;
		}
		else
			printf("Coordonnees passees en parametre non valides\n");
	}
	else
	{
		/* TODO : Implémenter fatalError*/
	}
	return pNewRect;
}

void Rectangle_destroyRectangle(Rectangle* pRect)
{
	free(pRect);
}

void Rectangle_drawRectangle(Rectangle* pRectangle, cairo_t* cr, InfoCamera* pCam )
{
	tdCoord2D* pPointProj1 = NULL;
	tdCoord2D* pPointProj2 = NULL;

	/* Construction du path */
	pPointProj1 = ProjectionTools_getPictureCoord(&((pRectangle->tPoint)[0]),pCam);
	cairo_move_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pRectangle->tPoint)[1]),pCam);
	cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
	free(pPointProj2);
	pPointProj2 = ProjectionTools_getPictureCoord(&(pRectangle->tPoint[2]),pCam);
	cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
	free(pPointProj2);
	pPointProj2 = ProjectionTools_getPictureCoord(&(pRectangle->tPoint[3]),pCam);
	cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
	cairo_close_path(cr);

	/* Réglage de la couleur du rectangle */
	cairo_set_source_rgba (cr, pRectangle->tColor[0], pRectangle->tColor[1], pRectangle->tColor[2] ,pRectangle->tColor[3]);
	cairo_fill_preserve( cr );/*remplissage du rectangle avec path preservé*/
	cairo_set_line_width(cr,2.0);/* réglage taille de la ligne*/
	cairo_set_source_rgb ( cr, 0, 0, 0); /* couleur contour */
	cairo_stroke(cr); /* dessin contour, perte du path */

	free(pPointProj1);
	free(pPointProj2);
}

void Rectangle_rotate(Rectangle* pRectangle, double dAngleX, double dAngleY, double dAngleZ)
{
	int iLoop;
	tdMatrix tdMatTransfo, tdMatPassRepObj;
	tdCoord tdCoordRepObj, tdCoordApTransfo;

	/* Initialisation des coordonées*/
	tdCoordRepObj[0] = 0;tdCoordApTransfo[0] = 0;
	tdCoordRepObj[1] = 0;tdCoordApTransfo[1] = 0;
	tdCoordRepObj[2] = 0;tdCoordApTransfo[2] = 0;
	tdCoordRepObj[3] = 0;tdCoordApTransfo[3] = 0;

	/* Construction de la matrice de passage World -> Repere objet */
	Matrix_initMatrix(tdMatPassRepObj); /* initialisation de la matrice de passage dans repere objet*/
	tdMatPassRepObj[0][3] = -pRectangle->Center.tdCoordWorld[0];
	tdMatPassRepObj[1][3] = -pRectangle->Center.tdCoordWorld[1];
	tdMatPassRepObj[2][3] = -pRectangle->Center.tdCoordWorld[2];

	if(dAngleX != 0)
	{
		/*Récupération de la matrice de rotation qui va bien */
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleX, axeX);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<4 ;iLoop++)
		{
			/* Tout d'abord recherche des coordonnées dans le repere de l'objet*/
			Matrix_multiMatrixVect(tdMatPassRepObj, pRectangle->tPoint[iLoop].tdCoordWorld, tdCoordRepObj);
			/*Puis transformation, toujours dans le repere objet*/
			Matrix_multiMatrixVect(tdMatTransfo, tdCoordRepObj, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pRectangle->tPoint[iLoop].tdCoordWorld[0] = pRectangle->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]);
			pRectangle->tPoint[iLoop].tdCoordWorld[1] = pRectangle->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]);
			pRectangle->tPoint[iLoop].tdCoordWorld[2] = pRectangle->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]);

			/* on réinitialise les vecteurs contenant les infos sur la transformation */
			tdCoordRepObj[0] = 0;tdCoordApTransfo[0] = 0;
			tdCoordRepObj[1] = 0;tdCoordApTransfo[1] = 0;
			tdCoordRepObj[2] = 0;tdCoordApTransfo[2] = 0;
			tdCoordRepObj[3] = 0;tdCoordApTransfo[3] = 0;
		}
	}

	if(dAngleY != 0)
	{
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleY, axeY);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<4 ;iLoop++)
		{
			/* Tout d'abord recherche des coordonnées dans le repere de l'objet*/
			Matrix_multiMatrixVect(tdMatPassRepObj, pRectangle->tPoint[iLoop].tdCoordWorld, tdCoordRepObj);
			/*Puis transformation, toujours dans le repere objet*/
			Matrix_multiMatrixVect(tdMatTransfo, tdCoordRepObj, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pRectangle->tPoint[iLoop].tdCoordWorld[0] = pRectangle->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]);
			pRectangle->tPoint[iLoop].tdCoordWorld[1] = pRectangle->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]);
			pRectangle->tPoint[iLoop].tdCoordWorld[2] = pRectangle->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]);

			tdCoordRepObj[0] = 0;tdCoordApTransfo[0] = 0;
			tdCoordRepObj[1] = 0;tdCoordApTransfo[1] = 0;
			tdCoordRepObj[2] = 0;tdCoordApTransfo[2] = 0;
			tdCoordRepObj[3] = 0;tdCoordApTransfo[3] = 0;
		}

	}

	if(dAngleZ != 0)
	{
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleZ, axeZ);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<4 ;iLoop++)
		{
			/* Tout d'abord recherche des coordonnées dans le repere de l'objet*/
			Matrix_multiMatrixVect(tdMatPassRepObj, pRectangle->tPoint[iLoop].tdCoordWorld, tdCoordRepObj);
			/*Puis transformation, toujours dans le repere objet*/
			Matrix_multiMatrixVect(tdMatTransfo, tdCoordRepObj, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pRectangle->tPoint[iLoop].tdCoordWorld[0] = pRectangle->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]);
			pRectangle->tPoint[iLoop].tdCoordWorld[1] = pRectangle->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]);
			pRectangle->tPoint[iLoop].tdCoordWorld[2] = pRectangle->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]);

			tdCoordRepObj[0] = 0;tdCoordApTransfo[0] = 0;
			tdCoordRepObj[1] = 0;tdCoordApTransfo[1] = 0;
			tdCoordRepObj[2] = 0;tdCoordApTransfo[2] = 0;
			tdCoordRepObj[3] = 0;tdCoordApTransfo[3] = 0;
		}
	}
}

void Rectangle_rotateWorld(Rectangle* pRectangle, double dAngleX, double dAngleY, double dAngleZ)
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
		for(iLoop=0 ; iLoop<4 ;iLoop++)
		{
			/*Transformation dans le repere du monde*/
			Matrix_multiMatrixVect(tdMatTransfo, pRectangle->tPoint[iLoop].tdCoordWorld, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pRectangle->tPoint[iLoop].tdCoordWorld[0] = tdCoordApTransfo[0];
			pRectangle->tPoint[iLoop].tdCoordWorld[1] = tdCoordApTransfo[1];
			pRectangle->tPoint[iLoop].tdCoordWorld[2] = tdCoordApTransfo[2];

			/* réinitialisation coord après transformation pour le  point suivant */
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}
	}

	if(dAngleY != 0)
	{
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleY, axeY);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<4 ;iLoop++)
		{
			/*Transformation dans le repere du monde*/
			Matrix_multiMatrixVect(tdMatTransfo, pRectangle->tPoint[iLoop].tdCoordWorld, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pRectangle->tPoint[iLoop].tdCoordWorld[0] = tdCoordApTransfo[0];
			pRectangle->tPoint[iLoop].tdCoordWorld[1] = tdCoordApTransfo[1];
			pRectangle->tPoint[iLoop].tdCoordWorld[2] = tdCoordApTransfo[2];

			/* réinitialisation coord après transformation pour le  point suivant */
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}

	}

	if(dAngleZ != 0)
	{
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleZ, axeZ);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<4 ;iLoop++)
		{
			/*Transformation dans le repere du monde*/
			Matrix_multiMatrixVect(tdMatTransfo, pRectangle->tPoint[iLoop].tdCoordWorld, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pRectangle->tPoint[iLoop].tdCoordWorld[0] = tdCoordApTransfo[0];
			pRectangle->tPoint[iLoop].tdCoordWorld[1] = tdCoordApTransfo[1];
			pRectangle->tPoint[iLoop].tdCoordWorld[2] = tdCoordApTransfo[2];

			/* réinitialisation coord après transformation pour le  point suivant */
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}
	}
}

void Rectangle_modSize(Rectangle* pRectangle, double dRatio)
{
	int iLoop;
	tdMatrix tdMatTransfo, tdMatPassRepObj;
	tdCoord tdCoordRepObj, tdCoordApTransfo;

	/* Initialisation des coordonées*/
	Point_initCoord(tdCoordRepObj, 0, 0, 0);
	Point_initCoord(tdCoordApTransfo, 0, 0, 0);

	/* Construction de la matrice de passage World -> Repere objet */
	Matrix_initMatrix(tdMatPassRepObj); /* initialisation de la matrice de passage dans repere objet*/
	tdMatPassRepObj[0][3] = -pRectangle->Center.tdCoordWorld[0];
	tdMatPassRepObj[1][3] = -pRectangle->Center.tdCoordWorld[1];
	tdMatPassRepObj[2][3] = -pRectangle->Center.tdCoordWorld[2];

	if(dRatio != 1)
	{
		/*Récupération de la matrice d'homothétie*/
		TransfoTools_getMatrixHomothety(tdMatTransfo, dRatio);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<4 ;iLoop++)
		{
			/* Tout d'abord recherche des coordonnées dans le repere de l'objet*/
			Matrix_multiMatrixVect(tdMatPassRepObj, pRectangle->tPoint[iLoop].tdCoordWorld, tdCoordRepObj);
			/*Puis transformation, toujours dans le repere objet*/
			Matrix_multiMatrixVect(tdMatTransfo, tdCoordRepObj, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pRectangle->tPoint[iLoop].tdCoordWorld[0] = pRectangle->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]);
			pRectangle->tPoint[iLoop].tdCoordWorld[1] = pRectangle->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]);
			pRectangle->tPoint[iLoop].tdCoordWorld[2] = pRectangle->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]);

			/* on réinitialise les vecteurs contenant les infos sur la transformation */
			Point_initCoord(tdCoordRepObj, 0, 0, 0);
			Point_initCoord(tdCoordApTransfo, 0, 0, 0);
		}
	}
}

gboolean Rectangle_Contient_Point( Rectangle* pRect, double x, double y,InfoCamera* pCam)
{
	gboolean est_contenu = FALSE;
	tdCoord2D* pPointProj0 = NULL;
	tdCoord2D* pPointProj1 = NULL;
	tdCoord2D* pPointProj2 = NULL;
	tdCoord2D* pPointProj3 = NULL;

		/* Projection de tous les point du cube */
	pPointProj0 = ProjectionTools_getPictureCoord(&((pRect->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pRect->tPoint)[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pRect->tPoint)[2]),pCam);
	pPointProj3 = ProjectionTools_getPictureCoord(&((pRect->tPoint)[3]),pCam);

	est_contenu = est_contenu || Cube_inFace( (*pPointProj0), (*pPointProj1), (*pPointProj2), (*pPointProj3), x, y );

	free(pPointProj0);
	free(pPointProj1);
	free(pPointProj2);
	free(pPointProj3);
	return est_contenu;
}

/* TODO : factoriser avec le fonction Cube_inFace à mettre dans le module selection*/
gboolean Rectangle_inFace(tdCoord2D tP1,tdCoord2D tP2,tdCoord2D tP3, tdCoord2D tP4, double dXClick, double dYClick )
{
	int iNb = 0, iLoop = 0;
	double tDistanceClick[2]; /* Distance (sur x et y) entre la position du curseur et chaque point  */
	double tDistancePoints[2]; /* Distance entre deux points d'une arrête*/
	double dDet = 0;
	tdCoord2D tCoordClick;

	Point_initCoord2D(tCoordClick, dXClick, dYClick); /* Coordonnées du clique */

	/* On passe chaque arrête en revue */
	for(iLoop=0; iLoop<4; iLoop++)
	{
		switch(iLoop)
		{
			case 0:
			{
				tDistanceClick[0] =tCoordClick[0]-tP1[0];
				tDistanceClick[1] = tCoordClick[1]-tP1[1];
				tDistancePoints[0] = tP2[0]-tP1[0];
				tDistancePoints[1] = tP2[1]-tP1[1];
				break;
			}
			case 1:
			{
				tDistanceClick[0] = tCoordClick[0]-tP2[0];
				tDistanceClick[1] = tCoordClick[1]-tP2[1];
				tDistancePoints[0] = tP3[0]-tP2[0];
				tDistancePoints[1] = tP3[1]-tP2[1];
				break;
			}
			case 2:
			{
				tDistanceClick[0] = tCoordClick[0]-tP3[0];
				tDistanceClick[1] = tCoordClick[1]-tP3[1];
				tDistancePoints[0] = tP4[0]-tP3[0];
				tDistancePoints[1] = tP4[1]-tP3[1];
				break;
			}
			case 3:
			{
				tDistanceClick[0] = tCoordClick[0]-tP4[0];
				tDistanceClick[1] = tCoordClick[1]-tP4[1];
				tDistancePoints[0] = tP1[0]-tP4[0];
				tDistancePoints[1] = tP1[1]-tP4[1];
				break;
			}
		}
		dDet = Point_determinant(tDistancePoints,tDistanceClick );

		if( dDet > 0)
				iNb++;
		else if( dDet < 0)
				iNb--;
	}

    if( iNb == 4 || iNb == -4 )
        return TRUE;
    else
    	return FALSE;
}
