#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ProjectionTools.h"

void ProjectionTools_getCoordWorld()
{

}

tCoord2D* ProjectionTools_getPictureCoord(Point* pPoint, InfoCamera* pCam)
{
	tCoord2D* tdPictureCoord = NULL;
	tCoord tdCamCoord; /* Coordonnées dans le repère caméra */

	Point_initCoord(tdCamCoord, 0,0,0);

	if( (tdPictureCoord = (tCoord2D*)malloc(sizeof(tCoord2D))) != NULL )
	{
		(*tdPictureCoord)[0]=0;
		(*tdPictureCoord)[1]=0;

		/* Recherche des coordonées dans le repère de la caméra */
		Matrix_multiMatrixVect(pCam->tdMatPassage, pPoint->tdCoordWorld, tdCamCoord);

		/* Calcul de coordonnées dans le plan de projection */
		(*tdPictureCoord)[0] = (pCam->dDMin*tdCamCoord[0])/tdCamCoord[2] + pCam->dRmax;
		(*tdPictureCoord)[1] = -((pCam->dDMin*tdCamCoord[1])/tdCamCoord[2]) + pCam->dUmax;

		/* On teste si le point peut entrer dans notre image (dépende du réglage de la caméra)*/
		if( (*tdPictureCoord)[0]>2*pCam->dRmax && (*tdPictureCoord)[1]>2*pCam->dUmax )/* hauteur puis largeur*/
		{
			free(tdPictureCoord); /* Libération mémoire */
			return NULL;
		}
	}
	else
	{
		/* TODO : fatalError */
		return NULL;
	}
	return tdPictureCoord;
}
