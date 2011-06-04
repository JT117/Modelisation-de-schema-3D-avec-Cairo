#include <gtk/gtk.h>
#include <math.h>

#include "Sphere.h"
#include "ProjectionTools.h"
#include "Point.h"
#include "Objet.h"

Sphere* Sphere_createSphere(tCoord tCenter, double dRadius)
{
	Sphere* pNewSphere = NULL;

	if( (pNewSphere = (Sphere*)malloc(sizeof(Sphere))) != NULL )
	{
		/*Sauvegarde des infos sur les points dans notre structure */
		Point_initGroup( &((pNewSphere->tPoint)[0]), 0, 0, 0); /* Dans le cas d'une sphere ce poitnest confondu avec le centre */

		/* Init du centre du repere de la figure (centre de gravité de la sphere) */
		Point_initGroup( &(pNewSphere->Center), tCenter[0], tCenter[1], tCenter[2]);

		pNewSphere->dRadius = dRadius;
		pNewSphere->estSelectionne = FALSE;
	}
	else
	{
		/* TODO : Implémenter fatalError*/
	}
	return pNewSphere;
}

void Sphere_drawSphere(Objet* pObj, cairo_t* cr, InfoCamera* pCam)
{
	int j;
	double dRadius=0;
	tCoord2D* pPointProj0 = NULL;
	tCoord2D* pPointProj1 = NULL;

	tdMatrix tdMatPass;
	tCoord tdCoordBefore;
	tCoord tdCoordAfter; /* Va contenir les coordonnées de points màj après chaque itération */

	/* On cherche à exprimer l'ensemble des coordonnées de points dans notre repere de la caméra --> pour projection */
	Groupe* pFatherGroup = NULL;
	Sphere* pSphere = pObj->type.sphere; /* Récupération du pointeur sur notre objet d'un type plus précis que Objet* */

	pFatherGroup = pObj->pFatherGroup; /* on récupère un pointeur vers le groupe pere */
	for(j=0;j<4;j++)  /* initialisation du tableau de coordonnées avant tout changement de base */
		tdCoordBefore[j] = pSphere->tPoint[0].tdCoordGroup[j];

	/* PREMIER CHANGEMENT DE BASE = PASSAGE REPERE OBJET --> GROUPE PERE */
	/* Passage des coordonnées du point dans le premier groupe pere */
	Matrix_initIdentityMatrix(tdMatPass); /* Initialisation de la matrice pour construction d'un matrice de passage */
	/* COnstruction de la nouvelle matrice de passage grâce aux coordonnées du repere objet dans son groupe pere*/
	tdMatPass[0][3] = pSphere->Center.tdCoordGroup[0];
	tdMatPass[1][3] = pSphere->Center.tdCoordGroup[1];
	tdMatPass[2][3] = pSphere->Center.tdCoordGroup[2];

	Matrix_multiMatrixVect(tdMatPass, tdCoordBefore, tdCoordAfter); /* tdCoordAfter contient les coordonnées du point après le premier changement de base*/
	ProjectionTools_getCoordWorld(tdCoordAfter,pFatherGroup,&(pSphere->tPoint[0]));

	/* Création d'un second point fictif pour la mesure du rayon une fois projetté */
	Point sTempPoint;
	Point_initWorld(&sTempPoint, (pSphere->tPoint->tdCoordWorld)[0],((pSphere->tPoint->tdCoordWorld)[1]+pSphere->dRadius),(pSphere->tPoint->tdCoordWorld)[2]);

	/* Projection des points du rayon de la sphere */
	pPointProj0 = ProjectionTools_getPictureCoord(&(pSphere->tPoint[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&(sTempPoint),pCam);

	dRadius = Point_euclideanDistance2D((*pPointProj0),(*pPointProj1));

	cairo_arc (cr, (*pPointProj0)[0], (*pPointProj0)[1], dRadius, 0.0, 2*M_PI);

	cairo_set_source_rgba (cr, pSphere->tColor[0], pSphere->tColor[1], pSphere->tColor[2] , pSphere->tColor[3]); /*Couleur */
	cairo_fill_preserve( cr );/*remplissage du rectangle avec path preservé*/
	cairo_set_line_width(cr,0.8);/* réglage taille de la ligne*/


	if(pSphere->estSelectionne == TRUE)  /* réglage de la couleur du contour*/
		cairo_set_source_rgb ( cr, 1.0, 0, 0);
	else
		cairo_set_source_rgb ( cr, 0, 0, 0);

	cairo_stroke(cr); /* dessin contour, perte du path */

	free(pPointProj0);
	free(pPointProj1);
}



void Sphere_modSize(Sphere* pSphere, double dRatio)
{
	int iLoop;
	tdMatrix tdMatTransfo, tdMatPassRepObj;
	tCoord tdCoordRepObj, tdCoordApTransfo;

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
		Transformation_getMatrixHomothety(tdMatTransfo, dRatio);

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

gboolean Sphere_Contient_Point( Sphere* pSphere, double x, double y, InfoCamera* pCam)
{
	tCoord2D* pPointProj0 = NULL;
	tCoord2D* pPointProj1 = NULL;
	tCoord2D tClickCoord;
	double dRadius, dDistClick;

	Point_initCoord2D(tClickCoord, x, y);

	pPointProj0 = ProjectionTools_getPictureCoord(&(pSphere->Center),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pSphere->tPoint)[0]),pCam);

	dRadius = Point_euclideanDistance2D((*pPointProj0),(*pPointProj1));
	dDistClick = Point_euclideanDistance2D((*pPointProj0), tClickCoord);

    return (dDistClick<=dRadius);
}


void Sphere_transfoCenter(Sphere* pSphere, tdMatrix tdTransfoMat)
{
	tCoord tCoordApTransfo;

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);
	/*APplication de la transformation au centre du Sphere */
	Matrix_multiMatrixVect(tdTransfoMat, pSphere->Center.tdCoordGroup, tCoordApTransfo);

	pSphere->Center.tdCoordGroup[0] = tCoordApTransfo[0];
	pSphere->Center.tdCoordGroup[1] = tCoordApTransfo[1];
	pSphere->Center.tdCoordGroup[2] = tCoordApTransfo[2];
	pSphere->Center.tdCoordGroup[3] = tCoordApTransfo[3];
}

void Sphere_transfo(Sphere* pSphere, tdMatrix tdTransfoMat)
{
	tCoord tCoordApTransfo;

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);

	Matrix_multiMatrixVect(tdTransfoMat, pSphere->tPoint[0].tdCoordGroup, tCoordApTransfo);

	/* Mod0f0cat0on des coordonées dans le repere objet */
	pSphere->tPoint[0].tdCoordGroup[0] = tCoordApTransfo[0];
	pSphere->tPoint[0].tdCoordGroup[1] = tCoordApTransfo[1];
	pSphere->tPoint[0].tdCoordGroup[2] = tCoordApTransfo[2];
	pSphere->tPoint[0].tdCoordGroup[3] = tCoordApTransfo[3];

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);  /* Re0n0t0al0sat0on de la matr0ce de coordonnées après transformat0on*/

}
