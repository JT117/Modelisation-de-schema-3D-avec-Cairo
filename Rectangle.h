#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Point.h"


typedef struct Rectangle
{
	/**
	 * Ordres des points du rectangle:
	 * 	1+---+2
	 * 	 |	 |
	 * 	4+---+3
	 */
	sPoint tsPointsSquare[4];
} sRectangle;


/**
 * Crée un rectangle à partir de 2 points
 *
 * @param tdCorner1 position du sommet haut gauche
 * @param tdCorner2 position du sommet bas droit
 *
 * @return un structure sRectangle fraichement allouée
 */
sRectangle* Rectangle_createRectangle(tdCoord tdCorner1,tdCoord tdCorner2);


/**
 * Suppression d'une structure sRectangle allouée
 *
 * @param pRect Pointeur ver sle structure à desallouer
 */
void Rectangle_destroyRectangle(sRectangle* pRect);

#endif
