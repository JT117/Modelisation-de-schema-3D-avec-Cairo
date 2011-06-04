#include <stdio.h>
#include <stdlib.h>

#include "Quadrilateral.h"
#include "Objet.h"


Quadrilateral* Quadrilateral_createQuadrilateral(tCoord tCenter,tCoord2D tCoord1,tCoord2D tCoord2,tCoord2D tCoord3,tCoord2D tCoord4)
{
	Quadrilateral* pNewQuadri = NULL;

	if( ( pNewQuadri = (Quadrilateral*)malloc(sizeof(Quadrilateral))) != NULL )
	{
		/*Sauvegarde des infos sur les points dans notre structure */
		Point_initGroup( &((pNewQuadri->tPoint)[0]), tCoord1[0], tCoord1[1], 0);
		Point_initGroup( &((pNewQuadri->tPoint)[1]), tCoord2[0], tCoord2[1], 0);
		Point_initGroup( &((pNewQuadri->tPoint)[2]), tCoord3[0], tCoord3[1], 0);
		Point_initGroup( &((pNewQuadri->tPoint)[3]), tCoord3[0], tCoord3[1], 0);

		/* Init du centre du repere de la figure (centre de gravité du Triangle */
		Point_initGroup( &(pNewQuadri->Center), tCenter[0], tCenter[1], tCenter[2]);
	}
	else
	{
		/* TODO : Implémenter fatalError*/
	}
	pNewQuadri->estSelectionne = FALSE;

	return pNewQuadri;
}

void Quadrilateral_destroyQuadrilateral(Quadrilateral* pQuadrit)
{
	free(pQuadrit);
}

void Quadrilateral_updateCoordWolrd(Objet* pObj)
{
	int i,j;
	tdMatrix tdMatPass;
	tCoord tdCoordBefore;
	tCoord tdCoordAfter; /* Va contenir les coordonnées de points màj après chaque itération */

	/* On cherche à exprimer l'ensemble des coordonnées de points dans notre repere de la caméra --> pour projection */
	Groupe* pFatherGroup = NULL;
	Quadrilateral* pQuadri = pObj->type.quadrilateral; /* Récupération du pointeur sur notre objet d'un type plus précis que Objet* */

	for(i=0;i<4;i++ )  /* On passe tous les points de l'objet en revue */
	{
		pFatherGroup = pObj->pFatherGroup; /* on récupère un pointeur vers le groupe pere */
		for(j=0;j<4;j++)  /* initialisation du tableau de coordonnées avant tout changement de base */
			tdCoordBefore[j] = pQuadri->tPoint[i].tdCoordGroup[j];

		/* PREMIER CHANGEMENT DE BASE = PASSAGE REPERE OBJET --> GROUPE PERE */
		/* Passage des coordonnées du point dans le premier groupe pere */
		Matrix_initIdentityMatrix(tdMatPass); /* Initialisation de la matrice pour construction d'un matrice de passage */
		/* COnstruction de la nouvelle matrice de passage grâce aux coordonnées du repere objet dans son groupe pere*/
		tdMatPass[0][3] = pQuadri->Center.tdCoordGroup[0];
		tdMatPass[1][3] = pQuadri->Center.tdCoordGroup[1];
		tdMatPass[2][3] = pQuadri->Center.tdCoordGroup[2];

		Matrix_multiMatrixVect(tdMatPass, tdCoordBefore, tdCoordAfter); /* tdCoordAfter contient les coordonnées du point après le premier changement de base*/
		ProjectionTools_getCoordWorld(tdCoordAfter,pFatherGroup,&(pQuadri->tPoint[i]));
	}

	pFatherGroup = pObj->pFatherGroup;
	/* On met aussi à jour les coordonnées du centre de l'objet */
	ProjectionTools_getCoordWorld(pQuadri->Center.tdCoordGroup,pFatherGroup,&(pQuadri->Center));
}

void Quadrilateral_drawQuadrilateral( Objet* pObj,cairo_t* cr,InfoCamera* pCam)
{
	Quadrilateral* pQuadri = NULL;
	/* Coordonnées de points une fois projettés */
	tCoord2D* pPointProj0 = NULL;
	tCoord2D* pPointProj1 = NULL;
	tCoord2D* pPointProj2 = NULL;
	tCoord2D* pPointProj3 = NULL;

	Quadrilateral_updateCoordWolrd(pObj); /* Changement de repere -> on passe dans le repere objet */

	pQuadri = pObj->type.quadrilateral;
	/* Grace à nos 4 points ayant leurs coordonnées exprimées dans le repere du monde on projette ! */
	pPointProj0 = ProjectionTools_getPictureCoord(&(pQuadri->tPoint[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&(pQuadri->tPoint[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&(pQuadri->tPoint[2]),pCam);
	pPointProj3 = ProjectionTools_getPictureCoord(&(pQuadri->tPoint[3]),pCam);

	/* Construction du path */
	cairo_move_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
	cairo_line_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
	cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
	cairo_line_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
	cairo_close_path(cr);

	/* Réglage de la couleur du Quadrilateral */
	cairo_set_source_rgba (cr, pQuadri->tColor[0], pQuadri->tColor[1], pQuadri->tColor[2] ,pQuadri->tColor[3]);
	cairo_fill_preserve( cr );/*remplissage du Quadrilateral avec path preservé*/
	cairo_set_line_width(cr,0.8);/* réglage taille de la ligne*/

	if( pQuadri->estSelectionne )
		cairo_set_source_rgb ( cr, 1.0, 0, 0);
	else
		cairo_set_source_rgb ( cr, 0, 0, 0); /* couleur contour */

	cairo_stroke(cr); /* dessin contour, perte du path */

	free(pPointProj0);
	free(pPointProj1);
	free(pPointProj2);
	free(pPointProj3);
}

void Quadrilateral_transfoCenter(Quadrilateral* pQuadri, tdMatrix tdTransfoMat)
{
	tCoord tCoordApTransfo;

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);
	/*APplication de la transformation au centre du Quadrilateral */
	Matrix_multiMatrixVect(tdTransfoMat, pQuadri->Center.tdCoordGroup, tCoordApTransfo);

	pQuadri->Center.tdCoordGroup[0] = tCoordApTransfo[0];
	pQuadri->Center.tdCoordGroup[1] = tCoordApTransfo[1];
	pQuadri->Center.tdCoordGroup[2] = tCoordApTransfo[2];
	pQuadri->Center.tdCoordGroup[3] = tCoordApTransfo[3];
}

void Quadrilateral_transfo(Quadrilateral* pQuadri, tdMatrix tdTransfoMat)
{
	tCoord tCoordApTransfo;
	int i;

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);
	for(i=0 ; i<4 ;i++)
	{
		Matrix_multiMatrixVect(tdTransfoMat, pQuadri->tPoint[i].tdCoordGroup, tCoordApTransfo);

		/* Modification des coordonées dans le repere objet */
		pQuadri->tPoint[i].tdCoordGroup[0] = tCoordApTransfo[0];
		pQuadri->tPoint[i].tdCoordGroup[1] = tCoordApTransfo[1];
		pQuadri->tPoint[i].tdCoordGroup[2] = tCoordApTransfo[2];
		pQuadri->tPoint[i].tdCoordGroup[3] = tCoordApTransfo[3];

		Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);  /* Reinitialisation de la matrice de coordonnées après transformation*/
	}
}


void Quadrilateral_modSize(Quadrilateral* pQuadrilateral, double dRatio)
{
	int iLoop;
	tdMatrix tdMatTransfo, tdMatPassRepObj;
	tCoord tdCoordRepObj, tdCoordApTransfo;

	/* Initialisation des coordonées*/
	Point_initCoord(tdCoordRepObj, 0, 0, 0);
	Point_initCoord(tdCoordApTransfo, 0, 0, 0);

	/* Construction de la matrice de passage World -> Repere objet */
	Matrix_initMatrix(tdMatPassRepObj); /* initialisation de la matrice de passage dans repere objet*/
	tdMatPassRepObj[0][3] = -pQuadrilateral->Center.tdCoordWorld[0];
	tdMatPassRepObj[1][3] = -pQuadrilateral->Center.tdCoordWorld[1];
	tdMatPassRepObj[2][3] = -pQuadrilateral->Center.tdCoordWorld[2];

	if(dRatio != 1)
	{
		/*Récupération de la matrice d'homothétie*/
		Transformation_getMatrixHomothety(tdMatTransfo, dRatio);

		/* On effectue la transformation pour tous  les points du Quadrilateral */
		for(iLoop=0 ; iLoop<4 ;iLoop++)
		{
			/* Tout d'abord recherche des coordonnées dans le repere de l'objet*/
			Matrix_multiMatrixVect(tdMatPassRepObj, pQuadrilateral->tPoint[iLoop].tdCoordWorld, tdCoordRepObj);
			/*Puis transformation, toujours dans le repere objet*/
			Matrix_multiMatrixVect(tdMatTransfo, tdCoordRepObj, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pQuadrilateral->tPoint[iLoop].tdCoordWorld[0] = pQuadrilateral->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]);
			pQuadrilateral->tPoint[iLoop].tdCoordWorld[1] = pQuadrilateral->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]);
			pQuadrilateral->tPoint[iLoop].tdCoordWorld[2] = pQuadrilateral->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]);

			/* on réinitialise les vecteurs contenant les infos sur la transformation */
			Point_initCoord(tdCoordRepObj, 0, 0, 0);
			Point_initCoord(tdCoordApTransfo, 0, 0, 0);
		}
	}
}

gboolean Quadrilateral_Contient_Point( Quadrilateral* pQuadri, double x, double y,InfoCamera* pCam)
{
	gboolean est_contenu = FALSE;
	tCoord2D* tCoordTab[4]; /* Tableau qui va contenir les points relatifs à une face donnée */
	tCoord2D* pPointProj0 = NULL;
	tCoord2D* pPointProj1 = NULL;
	tCoord2D* pPointProj2 = NULL;
	tCoord2D* pPointProj3 = NULL;

		/* Projection de tous les point du cube */
	pPointProj0 = ProjectionTools_getPictureCoord(&((pQuadri->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pQuadri->tPoint)[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pQuadri->tPoint)[2]),pCam);
	pPointProj3 = ProjectionTools_getPictureCoord(&((pQuadri->tPoint)[3]),pCam);

	tCoordTab[0] = pPointProj0;
	tCoordTab[1] = pPointProj1;
	tCoordTab[2] = pPointProj2;
	tCoordTab[3] = pPointProj3;
	est_contenu = est_contenu || Selection_inFace( tCoordTab,4, x, y );

	free(pPointProj0);
	free(pPointProj1);
	free(pPointProj2);
	free(pPointProj3);
	return est_contenu;
}
