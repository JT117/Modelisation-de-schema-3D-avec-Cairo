#ifndef POINT_H
#define POINT_H

#include <stdlib.h>
#include <gtk/gtk.h>

typedef double tdCoord[4];
typedef double tdCoord2D[2];

typedef struct Point
{
	/* TODO : garder une structure sur les deux */
    double x;
    double y;
    double z;

    /**Coordonnées du point dans le repère de l'univers**/
	tdCoord tdCoordWorld;
}Point;


/**
 * Initialise le point avec des coordonnées particulières.
 *
 * @param pPoint Le point à initialiser
 * @param dX Coordonnée sur axe x
 * @param dY Coordonnée sur axe Y
 * @param dZ Coordonnée sur axe Z
 */
void Point_init(Point* pPoint,double dX, double dY, double dZ);

/**
 * Initialise des coordonnées avec des valeurs particulières.
 *
 */
void Point_initCoord(tdCoord pCoord,double dX, double dY, double dZ);


#endif // POINT_H
