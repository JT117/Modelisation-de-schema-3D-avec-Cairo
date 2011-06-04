#include <gtk/gtk.h>
#include <math.h>

#include "Triangle.h"
#include "ProjectionTools.h"
#include "Point.h"
#include "Objet.h"

Triangle* Triangle_createTriangle(tCoord tCenter, tCoord2D tCoord1,tCoord2D tCoord2, tCoord2D tCoord3)
{
	Triangle* pNewTriangle = NULL;

	if( (pNewTriangle = (Triangle*)malloc(sizeof(Triangle))) != NULL )
	{
		/*Sauvegarde des infos sur les points dans notre structure */
		Point_initGroup( &((pNewTriangle->tPoint)[0]), tCoord1[0], tCoord1[1], 0);
		Point_initGroup( &((pNewTriangle->tPoint)[1]), tCoord2[0], tCoord2[1], 0);
		Point_initGroup( &((pNewTriangle->tPoint)[2]), tCoord3[0], tCoord3[1], 0);

		/* Init du centre du repere de la figure (centre de gravité du Triangle */
		Point_initGroup( &(pNewTriangle->Center), tCenter[0], tCenter[1], tCenter[2]);

		pNewTriangle->estSelectionne = FALSE;
	}
	else
	{
		/* TODO : Implémenter fatalError*/
	}
	return pNewTriangle;
}

void Triangle_updateCoordWolrd(Objet* pObj)
{
	int i,j;
	tdMatrix tdMatPass;
	tCoord tdCoordBefore;
	tCoord tdCoordAfter; /* Va contenir les coordonnées de points màj après chaque itération */

	/* On cherche à exprimer l'ensemble des coordonnées de points dans notre repere de la caméra --> pour projection */
	Groupe* pFatherGroup = NULL;
	Triangle* pTriangle = pObj->type.triangle; /* Récupération du pointeur sur notre objet d'un type plus précis que Objet* */

	for(i=0;i<3;i++ )  /* On passe tous les points de l'objet en revue */
	{
		pFatherGroup = pObj->pFatherGroup; /* on récupère un pointeur vers le groupe pere */
		for(j=0;j<4;j++)  /* initialisation du tableau de coordonnées avant tout changement de base */
			tdCoordBefore[j] = pTriangle->tPoint[i].tdCoordGroup[j];

		/* PREMIER CHANGEMENT DE BASE = PASSAGE REPERE OBJET --> GROUPE PERE */
		/* Passage des coordonnées du point dans le premier groupe pere */
		Matrix_initIdentityMatrix(tdMatPass); /* Initialisation de la matrice pour construction d'un matrice de passage */
		/* COnstruction de la nouvelle matrice de passage grâce aux coordonnées du repere objet dans son groupe pere*/
		tdMatPass[0][3] = pTriangle->Center.tdCoordGroup[0];
		tdMatPass[1][3] = pTriangle->Center.tdCoordGroup[1];
		tdMatPass[2][3] = pTriangle->Center.tdCoordGroup[2];

		Matrix_multiMatrixVect(tdMatPass, tdCoordBefore, tdCoordAfter); /* tdCoordAfter contient les coordonnées du point après le premier changement de base*/
		ProjectionTools_getCoordWorld(tdCoordAfter,pFatherGroup,&(pTriangle->tPoint[i]));
	}

	pFatherGroup = pObj->pFatherGroup;
	/* On met aussi à jour les coordonnées du centre de l'objet */
	ProjectionTools_getCoordWorld(pTriangle->Center.tdCoordGroup,pFatherGroup,&(pTriangle->Center));
}

void Triangle_drawTriangle( Objet* pObj,cairo_t* cr,InfoCamera* pCam)
{
	GArray* gtTabFacesOrder=NULL; /* Tableau indiquant dans quel ordre il faut dessiner les faces du Triangle */
	/* Coordonnées de points une fois projettés */
	tCoord2D* pPointProj0 = NULL;
	tCoord2D* pPointProj1 = NULL;
	tCoord2D* pPointProj2 = NULL;
	tCoord2D* pPointProj3 = NULL;

	Triangle* pTriangle = pObj->type.triangle;
	Triangle_updateCoordWolrd(pObj);

	/* Projection de tous les point du Triangle */
	pPointProj0 = ProjectionTools_getPictureCoord(&((pTriangle->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pTriangle->tPoint)[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pTriangle->tPoint)[2]),pCam);

	/* Construction du path */
	cairo_move_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
	cairo_line_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
	cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
	cairo_close_path(cr);

	cairo_set_source_rgba (cr, pTriangle->tColor[0], pTriangle->tColor[1], pTriangle->tColor[2] , pTriangle->tColor[3]); /*Couleur */
	cairo_fill_preserve( cr );/*remplissage du rectangle avec path preservé*/
	cairo_set_line_width(cr,0.8);/* réglage taille de la ligne*/

	if(pTriangle->estSelectionne == TRUE)  /* réglage de la couleur du contour*/
		cairo_set_source_rgb ( cr, 1.0, 0, 0);
	else
		cairo_set_source_rgb ( cr, 0, 0, 0);

	cairo_stroke(cr); /* dessin contour, perte du path */

	/* Libération mémoire */
	free(pPointProj0);
	free(pPointProj1);
	free(pPointProj2);
}

void Triangle_modSize(Triangle* pTriangle, double dRatio)
{
	int iLoop;
	tdMatrix tdMatTransfo, tdMatPassRepObj;
	tCoord tdCoordRepObj, tdCoordApTransfo;

	/* Initialisation des coordonées*/
	Point_initCoord(tdCoordRepObj, 0, 0, 0);
	Point_initCoord(tdCoordApTransfo, 0, 0, 0);

	/* Construction de la matrice de passage World -> Repere objet */
	Matrix_initMatrix(tdMatPassRepObj); /* initialisation de la matrice de passage dans repere objet*/
	tdMatPassRepObj[0][3] = -pTriangle->Center.tdCoordWorld[0];
	tdMatPassRepObj[1][3] = -pTriangle->Center.tdCoordWorld[1];
	tdMatPassRepObj[2][3] = -pTriangle->Center.tdCoordWorld[2];

	if(dRatio != 1)
	{
		/*Récupération de la matrice d'homothétie*/
		Transformation_getMatrixHomothety(tdMatTransfo, dRatio);

		/* On effectue la transformation pour tous  les points du Triangle */
		for(iLoop=0 ; iLoop<8 ;iLoop++)
		{
			/* Tout d'abord recherche des coordonnées dans le repere de l'objet*/
			Matrix_multiMatrixVect(tdMatPassRepObj, pTriangle->tPoint[iLoop].tdCoordWorld, tdCoordRepObj);
			/*Puis transformation, toujours dans le repere objet*/
			Matrix_multiMatrixVect(tdMatTransfo, tdCoordRepObj, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pTriangle->tPoint[iLoop].tdCoordWorld[0] = pTriangle->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]);
			pTriangle->tPoint[iLoop].tdCoordWorld[1] = pTriangle->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]);
			pTriangle->tPoint[iLoop].tdCoordWorld[2] = pTriangle->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]);

			/* on réinitialise les vecteurs contenant les infos sur la transformation */
			Point_initCoord(tdCoordRepObj, 0, 0, 0);
			Point_initCoord(tdCoordApTransfo, 0, 0, 0);
		}
	}
}



gboolean Triangle_Contient_Point( Triangle* pTri, double x, double y, InfoCamera* pCam)
{
	gboolean est_contenu = FALSE;
	tCoord2D* tCoordTab[3]; /* Tableau qui va contenir les points relatifs à une face donnée */
	tCoord2D* pPointProj0 = NULL;
	tCoord2D* pPointProj1 = NULL;
	tCoord2D* pPointProj2 = NULL;

	/* Projection de tous les point du cube */
	pPointProj0 = ProjectionTools_getPictureCoord(&((pTri->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pTri->tPoint)[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pTri->tPoint)[2]),pCam);

	tCoordTab[0] = pPointProj0;
	tCoordTab[1] = pPointProj1;
	tCoordTab[2] = pPointProj2;
	est_contenu = est_contenu || Selection_inFace( tCoordTab,3, x, y );

	free(pPointProj0);
	free(pPointProj1);
	free(pPointProj2);
    return est_contenu;
}

void Triangle_transfoCenter(Triangle* pTriangle, tdMatrix tdTransfoMat)
{
	tCoord tCoordApTransfo;

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);
	/*APplication de la transformation au centre du rectangle */
	Matrix_multiMatrixVect(tdTransfoMat, pTriangle->Center.tdCoordGroup, tCoordApTransfo);

	pTriangle->Center.tdCoordGroup[0] = tCoordApTransfo[0];
	pTriangle->Center.tdCoordGroup[1] = tCoordApTransfo[1];
	pTriangle->Center.tdCoordGroup[2] = tCoordApTransfo[2];
	pTriangle->Center.tdCoordGroup[3] = tCoordApTransfo[3];
}

void Triangle_transfo(Triangle* pTriangle, tdMatrix tdTransfoMat)
{
	tCoord tCoordApTransfo;
	int i;

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);
	for(i=0 ; i<3 ;i++)
	{
		Matrix_multiMatrixVect(tdTransfoMat, pTriangle->tPoint[i].tdCoordGroup, tCoordApTransfo);

		/* Modification des coordonées dans le repere objet */
		pTriangle->tPoint[i].tdCoordGroup[0] = tCoordApTransfo[0];
		pTriangle->tPoint[i].tdCoordGroup[1] = tCoordApTransfo[1];
		pTriangle->tPoint[i].tdCoordGroup[2] = tCoordApTransfo[2];
		pTriangle->tPoint[i].tdCoordGroup[3] = tCoordApTransfo[3];

		Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);  /* Reinitialisation de la matrice de coordonnées après transformation*/
	}
}
