#include <stdio.h>
#include <stdlib.h>

#include "Rectangle.h"
#include "Objet.h"


Rectangle* Rectangle_createRectangle(tCoord tdCorner1,tCoord tdCorner2, tCoord tdCenter)
{
	Rectangle* pNewRect = NULL;

	if( (pNewRect = (Rectangle*)malloc(sizeof(Rectangle))) != NULL )
	{
		/* Vérification : est ce que tdCorner1 correspond bien au sommet haut gauche ? */
		if(tdCorner1[1]>tdCorner2[1]) /* Verif effectuée sur l'axe des Y (arbitrairement axe vertical)*/
		{
			/*Sauvegarde des infos sur les points dans notre structure */
			Point_initGroup( &((pNewRect->tPoint)[0]), tdCorner1[0], tdCorner1[1], tdCorner1[2]);
			Point_initGroup( &((pNewRect->tPoint)[1]), tdCorner2[0], tdCorner1[1], tdCorner2[2]);
			Point_initGroup( &((pNewRect->tPoint)[2]), tdCorner2[0], tdCorner2[1], tdCorner2[2]);
			Point_initGroup( &((pNewRect->tPoint)[3]), tdCorner1[0], tdCorner2[1], tdCorner1[2]);

			/* On met aussi à jour les coordonnées dans le repere du monde (necessaire pour la projection) */
			/*
			Point_init( &((pNewRect->tPoint)[0]), tdCorner1[0], tdCorner1[1], tdCorner1[2]);
			Point_init( &((pNewRect->tPoint)[1]), tdCorner2[0], tdCorner1[1], tdCorner2[2]);
			Point_init( &((pNewRect->tPoint)[2]), tdCorner2[0], tdCorner2[1], tdCorner2[2]);
			Point_init( &((pNewRect->tPoint)[3]), tdCorner1[0], tdCorner2[1], tdCorner1[2]);
			*/


			/* Init du centre du repere de la figure (centre de gravité du rectangle) dans le repere parent */
			Point_initGroup( &(pNewRect->Center), tdCenter[0],tdCenter[1] , tdCenter[2]);

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
	pNewRect->estSelectionne = FALSE;

	return pNewRect;
}

void Rectangle_destroyRectangle(Rectangle* pRect)
{
	free(pRect);
}


void Rectangle_drawRectangle( Objet* pObj,cairo_t* cr,InfoCamera* pCam)
{
	int i,j;
	/* Coordonnées de points une fois projettés */
	tCoord2D* pPointProj0 = NULL;
	tCoord2D* pPointProj1 = NULL;
	tCoord2D* pPointProj2 = NULL;
	tCoord2D* pPointProj3 = NULL;
	tdMatrix tdMatPass;
	tCoord tdCoordBefore;
	tCoord tdCoordAfter; /* Va contenir les coordonnées de points màj après chaque itération */

	/* On cherche à exprimer l'ensemble des coordonnées de points dans notre repere de la caméra --> pour projection */
	Groupe* pFatherGroup = NULL;
	Rectangle* pRec = pObj->type.rectangle; /* Récupération du pointeur sur notre objet d'un type plus précis que Objet* */

	for(i=0;i<4;i++ )  /* On passe tous les points de l'objet en revue */
	{
		pFatherGroup = pObj->pFatherGroup; /* on récupère un pointeur vers le groupe pere */
		for(j=0;j<4;j++)  /* initialisation du tableau de coordonnées avant tout changement de base */
			tdCoordBefore[j] = pRec->tPoint[i].tdCoordGroup[j];

		/* PREMIER CHANGEMENT DE BASE = PASSAGE REPERE OBJET --> GROUPE PERE */
		/* Passage des coordonnées du point dans le premier groupe pere */
		Matrix_initIdentityMatrix(tdMatPass); /* Initialisation de la matrice pour construction d'un matrice de passage */
		/* COnstruction de la nouvelle matrice de passage grâce aux coordonnées du repere objet dans son groupe pere*/
		tdMatPass[0][3] = pRec->Center.tdCoordGroup[0];
		tdMatPass[1][3] = pRec->Center.tdCoordGroup[1];
		tdMatPass[2][3] = pRec->Center.tdCoordGroup[2];

		Matrix_multiMatrixVect(tdMatPass, tdCoordBefore, tdCoordAfter); /* tdCoordAfter contient les coordonnées du point après le premier changement de base*/
		ProjectionTools_getCoordWorld(tdCoordAfter,pFatherGroup,&(pRec->tPoint[i]));
	}
	/* Grace à nos 4 points ayant leurs coordonnées exprimées dans le repere du monde on projette ! */
	pPointProj0 = ProjectionTools_getPictureCoord(&(pRec->tPoint[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&(pRec->tPoint[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&(pRec->tPoint[2]),pCam);
	pPointProj3 = ProjectionTools_getPictureCoord(&(pRec->tPoint[3]),pCam);

	/* Construction du path */
	cairo_move_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
	cairo_line_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
	cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
	cairo_line_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
	cairo_close_path(cr);

	/* Réglage de la couleur du rectangle */
	cairo_set_source_rgba (cr, pRec->tColor[0], pRec->tColor[1], pRec->tColor[2] ,pRec->tColor[3]);
	cairo_fill_preserve( cr );/*remplissage du rectangle avec path preservé*/
	cairo_set_line_width(cr,0.8);/* réglage taille de la ligne*/

	if( pRec->estSelectionne )
		cairo_set_source_rgb ( cr, 1.0, 0, 0);
	else
		cairo_set_source_rgb ( cr, 0, 0, 0); /* couleur contour */

	cairo_stroke(cr); /* dessin contour, perte du path */

	free(pPointProj0);
	free(pPointProj1);
	free(pPointProj2);
	free(pPointProj3);
}

void Rectangle_transfoCenter(Rectangle* pRec, tdMatrix tdTransfoMat)
{
	tCoord tCoordApTransfo;

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);
	/*APplication de la transformation au centre du rectangle */
	Matrix_multiMatrixVect(tdTransfoMat, pRec->Center.tdCoordGroup, tCoordApTransfo);

	pRec->Center.tdCoordGroup[0] = tCoordApTransfo[0];
	pRec->Center.tdCoordGroup[1] = tCoordApTransfo[1];
	pRec->Center.tdCoordGroup[2] = tCoordApTransfo[2];
	pRec->Center.tdCoordGroup[3] = tCoordApTransfo[3];
}

void Rectangle_transfo(Rectangle* pRec, tdMatrix tdTransfoMat)
{
	tCoord tCoordApTransfo;
	int i;

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);
	for(i=0 ; i<4 ;i++)
	{
		Matrix_multiMatrixVect(tdTransfoMat, pRec->tPoint[i].tdCoordGroup, tCoordApTransfo);

		/* Modification des coordonées dans le repere objet */
		pRec->tPoint[i].tdCoordGroup[0] = tCoordApTransfo[0];
		pRec->tPoint[i].tdCoordGroup[1] = tCoordApTransfo[1];
		pRec->tPoint[i].tdCoordGroup[2] = tCoordApTransfo[2];
		pRec->tPoint[i].tdCoordGroup[3] = tCoordApTransfo[3];

		/* Modif dans le repere du monde : TODO --> faut il faire disparaitre la structure des info sur le repre du monde ? */
		/*
		pRectangle->tPoint[i].tdCoordWorld[0] = pRectangle->tPoint[i].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]);
		pRectangle->tPoint[i].tdCoordWorld[1] = pRectangle->tPoint[i].tdCoordWorld[1]
																+ (tdCoordApTransfo[1]-tdCoordRepObj[1]);
		pRectangle->tPoint[i].tdCoordWorld[2] = pRectangle->tPoint[i].tdCoordWorld[2]
																+ (tdCoordApTransfo[2]-tdCoordRepObj[2]);
		*/
		Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);  /* Reinitialisation de la matrice de coordonnées après transformation*/
	}
}

void Rectangle_rotate(Rectangle* pRectangle, double dAngleX, double dAngleY, double dAngleZ)
{
	int iLoop;
	tdMatrix tdMatTransfo, tdMatPassRepObj;
	tCoord tdCoordRepObj, tdCoordApTransfo;

	/* Initialisation des coordonées*/
	Point_initCoord(tdCoordRepObj, 0.0, 0.0, 0.0);
	Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);

	/* Construction de la matrice de passage World -> Repere objet */
	Matrix_initMatrix(tdMatPassRepObj); /* initialisation de la matrice de passage dans repere objet*/
	tdMatPassRepObj[0][0] = 1;
	tdMatPassRepObj[1][1] = 1;
	tdMatPassRepObj[2][2] = 1;
	tdMatPassRepObj[3][3] = 1;

	tdMatPassRepObj[0][3] = -pRectangle->Center.tdCoordWorld[0];
	tdMatPassRepObj[1][3] = -pRectangle->Center.tdCoordWorld[1];
	tdMatPassRepObj[2][3] = -pRectangle->Center.tdCoordWorld[2];

	if(dAngleX != 0)
	{
		/*Récupération de la matrice de rotation qui va bien */
		Transformation_getMatrixRotation(tdMatTransfo, dAngleX, AXEX);

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
			Point_initCoord(tdCoordRepObj, 0.0, 0.0, 0.0);
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}
	}

	if(dAngleY != 0)
	{
		Transformation_getMatrixRotation(tdMatTransfo, dAngleY, AXEY);

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

			Point_initCoord(tdCoordRepObj, 0.0, 0.0, 0.0);
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}

	}

	if(dAngleZ != 0)
	{
		Transformation_getMatrixRotation(tdMatTransfo, dAngleZ, AXEZ);

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

			Point_initCoord(tdCoordRepObj, 0.0, 0.0, 0.0);
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}
	}
}

void Rectangle_rotateWorld(Rectangle* pRectangle, double dAngleX, double dAngleY, double dAngleZ)
{
	int iLoop;
	tdMatrix tdMatTransfo;
	tCoord tdCoordApTransfo;

	if(dAngleX != 0)
	{
		/*Récupération de la matrice de rotation qui va bien */
		Transformation_getMatrixRotation(tdMatTransfo, dAngleX, AXEX);

		Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
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

		/* Modification des coordonnées du centre de la figure */
		Matrix_multiMatrixVect(tdMatTransfo, pRectangle->Center.tdCoordWorld, tdCoordApTransfo);
		Point_init(&(pRectangle->Center),tdCoordApTransfo[0],tdCoordApTransfo[1],tdCoordApTransfo[2]);
	}

	if(dAngleY != 0)
	{
		Transformation_getMatrixRotation(tdMatTransfo, dAngleY, AXEY);

		Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
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
		/* Modification des coordonnées du centre de la figure */
		Matrix_multiMatrixVect(tdMatTransfo, pRectangle->Center.tdCoordWorld, tdCoordApTransfo);
		Point_init(&(pRectangle->Center),tdCoordApTransfo[0],tdCoordApTransfo[1],tdCoordApTransfo[2]);
	}

	if(dAngleZ != 0)
	{
		Transformation_getMatrixRotation(tdMatTransfo, dAngleZ, AXEZ);

		Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
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
		/* Modification des coordonnées du centre de la figure */
		Matrix_multiMatrixVect(tdMatTransfo, pRectangle->Center.tdCoordWorld, tdCoordApTransfo);
		Point_init(&(pRectangle->Center),tdCoordApTransfo[0],tdCoordApTransfo[1],tdCoordApTransfo[2]);
	}
}

void Rectangle_modSize(Rectangle* pRectangle, double dRatio)
{
	int iLoop;
	tdMatrix tdMatTransfo, tdMatPassRepObj;
	tCoord tdCoordRepObj, tdCoordApTransfo;

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
		Transformation_getMatrixHomothety(tdMatTransfo, dRatio);

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
	tCoord2D* pPointProj0 = NULL;
	tCoord2D* pPointProj1 = NULL;
	tCoord2D* pPointProj2 = NULL;
	tCoord2D* pPointProj3 = NULL;

		/* Projection de tous les point du cube */
	pPointProj0 = ProjectionTools_getPictureCoord(&((pRect->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pRect->tPoint)[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pRect->tPoint)[2]),pCam);
	pPointProj3 = ProjectionTools_getPictureCoord(&((pRect->tPoint)[3]),pCam);

	est_contenu = est_contenu || Selection_inFace( (*pPointProj0), (*pPointProj1), (*pPointProj2), (*pPointProj3), x, y );

	free(pPointProj0);
	free(pPointProj1);
	free(pPointProj2);
	free(pPointProj3);
	return est_contenu;
}

/* TODO : factoriser avec le fonction Cube_inFace à mettre dans le module selection*/
gboolean Rectangle_inFace(tCoord2D tP1,tCoord2D tP2,tCoord2D tP3, tCoord2D tP4, double dXClick, double dYClick )
{
	int iNb = 0, iLoop = 0;
	double tDistanceClick[2]; /* Distance (sur x et y) entre la position du curseur et chaque point  */
	double tDistancePoints[2]; /* Distance entre deux points d'une arrête*/
	double dDet = 0;
	tCoord2D tCoordClick;

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
