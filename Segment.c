#include <math.h>

#include "Segment.h"


Segment* Segment_createSegment(tCoord tdCoord1,tCoord tdCoord2)
{
	Segment* pNewSeg = NULL;

		if( (pNewSeg = (Segment*)malloc(sizeof(Segment))) != NULL )
		{
			/*Sauvegarde des infos sur les points dans notre structure */
			Point_init( &((pNewSeg->tPoint)[0]), tdCoord1[0], tdCoord1[1], tdCoord1[2]);
			Point_init( &((pNewSeg->tPoint)[1]), tdCoord2[0], tdCoord2[1], tdCoord2[2]);

			/* Init du centre du repere de la figure (centre de gravité du rectangle) */
			//Point_init( &(pNewRect->Center), tdCorner1[0]+((tdCorner2[0]-tdCorner1[0])/2),tdCorner1[1]+((tdCorner2[1]-tdCorner1[1])/2), tdCorner1[2]+((tdCorner2[2]-tdCorner1[2])/2));

			/*Couleur par défaut, pas de transparence*/
			pNewSeg->tColor[0]=0.0;
			pNewSeg->tColor[1]=0.0;
			pNewSeg->tColor[2]=0.0;
			pNewSeg->tColor[3]=1.0;

			pNewSeg->bArrowed = FALSE;
			pNewSeg->bDashed = FALSE;
		}
		else
		{
			/* TODO : Implémenter fatalError*/
		}
    pNewSeg->estSelectionne = FALSE;

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

void Segment_destroySegment(Segment* pSeg)
{

}


void Segment_rotate(Segment* pSeg, double dAngleX, double dAngleY, double dAngleZ)
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
