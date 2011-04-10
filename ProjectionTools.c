#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "./include/ProjectionTools.h"

tdCoord2D* ProjectionTools_getPictureCoord(sPoint* pPoint, sInfoCamera* pCam)
{
	tdCoord2D* tdPictureCoord = NULL;
	tdCoord tdCamCoord; /* Coordonnées dans le repère caméra */

	tdCamCoord[0]=0;
	tdCamCoord[1]=0;
	tdCamCoord[2]=0;
	tdCamCoord[3]=0;

	if( (tdPictureCoord = (tdCoord2D*)malloc(sizeof(tdCoord2D))) != NULL )
	{
		/* Recherche des coordonées dans le repère de la caméra */
		Matrix_multiMatrixVect(pCam->tdMatPassage, pPoint->tdCoordWorld, tdCamCoord);

		/* Calcul de coordonnées dans le plan de projection */
		*tdPictureCoord[0] = pCam->dDMin*tdCamCoord[0]/tdCamCoord[2];
		*tdPictureCoord[1] = pCam->dDMin*tdCamCoord[1]/tdCamCoord[2];
	}
	else
	{
		/* TODO : fatalError */
	}
	return tdPictureCoord;
}
