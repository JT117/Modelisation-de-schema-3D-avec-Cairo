#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <cairo.h>
#include <gtk/gtk.h>

#include "Point.h"
#include "ProjectionTools.h"
#include "Camera.h"
#include "Color.h"

typedef struct Rectangle
{
	/**
	 * Ordres des points du rectangle:
	 * 	1+---+2
	 * 	 |	 |
	 * 	4+---+3
	 */
	Point tsPointsSquare[4];

	Point sCenter; /* Coordonées du repere de l'objet, utilisé pour les transformations */
	tColor tColor;
} Rectangle;


/**
 * Crée un rectangle à partir de 2 points
 *
 * @param tdCorner1 position du sommet haut gauche
 * @param tdCorner2 position du sommet bas droit
 *
 * @return un structure sRectangle fraichement allouée
 */
Rectangle* Rectangle_createRectangle(tdCoord tdCorner1,tdCoord tdCorner2);


/**
 * Dessine le rectangle dans le plan de projection de la caméra passé en param
 */
void Rectangle_drawRectangle(Rectangle* pRectangle, cairo_t* cr, InfoCamera* pCam);

/**
 * Suppression d'une structure sRectangle allouée
 *
 * @param pRect Pointeur ver sle structure à desallouer
 */
void Rectangle_destroyRectangle(Rectangle* pRect);

/**
 * Rotation de l'objet en fonction des angles passés en paramètre, la rotation se fait par rapport au repere de l'objet.
 *
 * @param  pRectangle L'objet rectangle à considérer
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 */
void Rectangle_rotateRectangle(Rectangle* pRectangle, double dAngleX, double dAngleY, double dAngleZ);

gboolean Rectangle_Contient_Point(Rectangle* pRect, double x, double y );

#endif

