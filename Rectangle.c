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

	/* TODO TODO : MODIF ! */
    /*est_contenu = est_contenu || est_dans_face( cCube->tPoint[0], cCube->tPoint[1], cCube->tPoint[2], cCube->tPoint[3], x, y )
                              || est_dans_face( cCube->tPoint[1], cCube->tPoint[5], cCube->tPoint[6], cCube->tPoint[2], x, y )
                              || est_dans_face( cCube->tPoint[4], cCube->tPoint[5], cCube->tPoint[6], cCube->tPoint[7], x, y )
                              || est_dans_face( cCube->tPoint[0], cCube->tPoint[4], cCube->tPoint[7], cCube->tPoint[3], x, y )
                              || est_dans_face( cCube->tPoint[0], cCube->tPoint[1], cCube->tPoint[5], cCube->tPoint[4], x, y )
                              || est_dans_face( cCube->tPoint[3], cCube->tPoint[2], cCube->tPoint[6], cCube->tPoint[7], x, y );
                              */
    return est_contenu;
}
