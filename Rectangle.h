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
	 * 	0+---+1
	 * 	 |	 |
	 * 	3+---+2
	 */
	Point tPoint[4];

	Point Center; /* Coordonées du repere de l'objet, utilisé pour les transformations */
	Color tColor; /* Couleur de l'objet */
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
 * @param  pRectangle L'objet rectangle à considérer
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 */
void Rectangle_rotate(Rectangle* pRectangle, double dAngleX, double dAngleY, double dAngleZ);

/**
 * Rotation de l'objet rectangle par rapport au centre de l'univers
 * @param  pRectangle L'objet rectangle à considérer
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 */
void Rectangle_rotateWorld(Rectangle* pRectangle, double dAngleX, double dAngleY, double dAngleZ);

/**
 * Agrandit/retrécit un objet de type rectangle suivant le ratio fourni en param.
 * @param pRectangle Le rectangle en question
 * @param dRatio Le ratio d'agrandissement/rétrecissement
 */
void Rectangle_modSize(Rectangle* pRectangle, double dRatio);

gboolean Rectangle_Contient_Point(Rectangle* pRect, double x, double y );

#endif
