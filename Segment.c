#include "Segment.h"


Segment* Segment_createSegment(tdCoord tdCoord1,tdCoord tdCoord2)
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
		}
		else
		{
			/* TODO : Implémenter fatalError*/
		}
	return pNewSeg;
}

void Segment_drawSegment(Segment* pSeg, cairo_t* cr, InfoCamera* pCam)
{
	tdCoord2D* pPointProj1 = NULL;
	tdCoord2D* pPointProj2 = NULL;

	pPointProj1 = ProjectionTools_getPictureCoord(&((pSeg->tPoint)[0]), pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pSeg->tPoint)[1]), pCam);

	cairo_move_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
	cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);

	/* On dessine */
	cairo_set_source_rgba (cr, pSeg->tColor[0], pSeg->tColor[1], pSeg->tColor[2] ,pSeg->tColor[3]);
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
}
