#include <stdio.h>
#include <stdlib.h>

#include "./include/Rectangle.h"

sRectangle* Rectangle_createRectangle(tdCoord tdCorner1,tdCoord tdCorner2)
{
	sRectangle* pNewRect = NULL;

	if( (pNewRect = (sRectangle*)malloc(sizeof(sRectangle))) != NULL )
	{
		/* Vérification : est ce que tdCorner1 correspond bien au sommet haut gauche ? */
		if(tdCorner1[1]>tdCorner2[1]) /* Verif effectuée sur l'axe des Y (arbitrairement axe vertical)*/
		{
			/*Sauvegarde des infos sur les points dans notre structure */
			Point_init( &(pNewRect->tsPointsSquare[0]), tdCorner1[0], tdCorner1[1], tdCorner1[2]);
			Point_init( &(pNewRect->tsPointsSquare[1]), tdCorner1[0]+(tdCorner2[0]-tdCorner1[0]), tdCorner1[1], tdCorner1[2]+(tdCorner2[2]-tdCorner1[2]));
			Point_init( &(pNewRect->tsPointsSquare[2]), tdCorner2[0], tdCorner2[1], tdCorner2[2]);
			Point_init( &(pNewRect->tsPointsSquare[3]), tdCorner1[0],  tdCorner1[1]+(tdCorner2[1]-tdCorner1[1]),  tdCorner1[2]);
		}
		else
		{
			printf("Coordonnees passees en parametre non valides\n");
		}
	}
	else
	{
		/* TODO : Implémenter fatalError*/
	}
	return pNewRect;
}

void Rectangle_destroyRectangle(sRectangle* pRect)
{
	free(pRect);
}
