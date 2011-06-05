#include <math.h>

#include "Segment.h"
#include "Objet.h"

Segment* Segment_createSegment(tCoord tCenter,tCoord tdCoord1,tCoord tdCoord2)
{
	Segment* pNewSeg = NULL;

	if( (pNewSeg = (Segment*)malloc(sizeof(Segment))) != NULL )
	{
		/*Sauvegarde des infos sur les points dans notre structure */
		Point_initGroup( &((pNewSeg->tPoint)[0]), tdCoord1[0], tdCoord1[1], tdCoord1[2]);
		Point_initGroup( &((pNewSeg->tPoint)[1]), tdCoord2[0], tdCoord2[1], tdCoord2[2]);

		Point_initGroup(&(pNewSeg->Center),tCenter[0],tCenter[1],tCenter[2]); /* Recherche du milieu du segment */

		pNewSeg->bArrowed = FALSE; /* Pas flêché ni dashé */
		pNewSeg->bDashed = FALSE;
		pNewSeg->estSelectionne = FALSE;
	}
	else
	{
		/* TODO : Implémenter fatalError*/
	}

	return pNewSeg;
}

void Segment_drawSegment(Segment* pSeg, cairo_t* cr, InfoCamera* pCam)
{
	double dDashLength = 10.0;
	double dArrowAngle = 0.0; /* Va permettre de stocker l'angle entre le segment et l'axe X */
	tCoord2D tArrow1, tArrow2; /* Va permettre de stocker les positions des extrémités d'une segment avec flêche */
	tCoord2D* pPointProj1 = NULL;
	tCoord2D* pPointProj2 = NULL;

	pPointProj1 = ProjectionTools_getPictureCoord(&((pSeg->tPoint)[0]), pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pSeg->tPoint)[1]), pCam);

	cairo_move_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
	cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);

	/* On dessine */
	cairo_set_source_rgba (cr, pSeg->tColor[0], pSeg->tColor[1], pSeg->tColor[2] ,pSeg->tColor[3]);

	if(pSeg->bDashed == TRUE)
		cairo_set_dash(cr,&dDashLength,1,0);

	if( pSeg->bArrowed == TRUE ) /* On trace la tête de la flêche */
	{
		/* Recherche des points constituants les deux extrémités(gauche et droite) de la fléche */
		dArrowAngle = atan2 ( (*pPointProj2)[1] - (*pPointProj1)[1], (*pPointProj2)[0] - (*pPointProj1)[0] ) + M_PI;

		tArrow1[0] = (*pPointProj2)[0]  + 15 * cos(dArrowAngle - 0.5); /* 15 correspond à la taille de la flêche */
		tArrow1[1] = (*pPointProj2)[1]  + 15 * sin(dArrowAngle - 0.5); /* 0.5 à l'angle entre le segment et une extrémité de la flêche*/

		tArrow2[0] = (*pPointProj2)[0]  + 15 * cos(dArrowAngle + 0.5);
		tArrow2[1] = (*pPointProj2)[1]  + 15 * sin(dArrowAngle + 0.5);

		cairo_move_to( cr, tArrow1[0], tArrow1[1]);
		cairo_line_to( cr, (*pPointProj2)[0],(*pPointProj2)[1]);
		cairo_move_to( cr, tArrow1[0], tArrow1[1]);
	}
	cairo_set_line_width(cr, 0.8);
	cairo_stroke(cr); /* dessin contour, perte du path */
}

void Segment_updateCoordWolrd(Objet* pObj)
{
	int i,j;
	tdMatrix tdMatPass;
	tCoord tdCoordBefore;
	tCoord tdCoordAfter; /* Va contenir les coordonnées de points màj après chaque itération */

	/* On cherche à exprimer l'ensemble des coordonnées de points dans notre repere de la caméra --> pour projection */
	Groupe* pFatherGroup = NULL;
	Segment* pSeg = pObj->type.segment; /* Récupération du pointeur sur notre objet d'un type plus précis que Objet* */

	for(i=0;i<2;i++ )  /* On passe tous les points de l'objet en revue */
	{
		pFatherGroup = pObj->pFatherGroup; /* on récupère un pointeur vers le groupe pere */
		for(j=0;j<4;j++)  /* initialisation du tableau de coordonnées avant tout changement de base */
			tdCoordBefore[j] = pSeg->tPoint[i].tdCoordGroup[j];

		/* PREMIER CHANGEMENT DE BASE = PASSAGE REPERE OBJET --> GROUPE PERE */
		/* Passage des coordonnées du point dans le premier groupe pere */
		Matrix_initIdentityMatrix(tdMatPass); /* Initialisation de la matrice pour construction d'un matrice de passage */
		/* COnstruction de la nouvelle matrice de passage grâce aux coordonnées du repere objet dans son groupe pere*/
		tdMatPass[0][3] = pSeg->Center.tdCoordGroup[0];
		tdMatPass[1][3] = pSeg->Center.tdCoordGroup[1];
		tdMatPass[2][3] = pSeg->Center.tdCoordGroup[2];

		Matrix_multiMatrixVect(tdMatPass, tdCoordBefore, tdCoordAfter); /* tdCoordAfter contient les coordonnées du point après le premier changement de base*/
		ProjectionTools_getCoordWorld(tdCoordAfter,pFatherGroup,&(pSeg->tPoint[i]));
	}

	pFatherGroup = pObj->pFatherGroup;
	/* On met aussi à jour les coordonnées du centre de l'objet */
	ProjectionTools_getCoordWorld(pSeg->Center.tdCoordGroup,pFatherGroup,&(pSeg->Center));
}

void Segment_destroySegment(Segment* pSeg)
{

}

gboolean Segment_Contient_Point(Segment* pRect, double x, double y )
{
    return FALSE;
}

void Segment_setColor(Segment* pSeg,double dR, double dG, double dB)
{
	pSeg->tColor[0]=dR;
	pSeg->tColor[1]=dG;
	pSeg->tColor[2]=dB;
}

void Segment_setArrowed(Segment* pSeg)
{
	pSeg->bArrowed =TRUE;
}

void Segment_setDashed(Segment* pSeg,double dR, double dG, double dB)
{
	pSeg->bDashed = TRUE;
}
