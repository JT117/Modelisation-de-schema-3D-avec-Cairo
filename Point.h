#ifndef POINT_H
#define POINT_H

#include <stdlib.h>
#include <gtk/gtk.h>

#include "MatrixTools.h"

typedef double tCoord[4];
typedef double tCoord2D[2];

typedef struct Point
{
	/* TODO : garder une structure sur les deux */
    double x;
    double y;
    double z;
    tCoord2D tCoordProjection;
    /**Coordonnées du point dans le repère de l'univers**/
	tCoord tdCoordWorld;
	/** Coordonnées du point dans le repere du groupe parent **/
	tCoord tdCoordGroup;
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
void Point_initCoord(tCoord pCoord,double dX, double dY, double dZ);

/**
 * Initialisation des coordonées 2D.
 */
void Point_initCoord2D(tCoord pCoord,double dX, double dY);

void Point_middlePoint(Point* pRes, Point* pP1, Point* pP2);

double Point_euclideanDistance(Point* pP1, Point* pP2);

double Point_euclideanDistance2D(tCoord2D pP1, tCoord2D pP2);

double Point_determinant(tCoord2D tP1, tCoord2D tP2);

void Point_initGroup(Point* pPoint, double dX, double dY, double dZ);

void Point_initWorld(Point* pPoint, double dX, double dY, double dZ);




#endif // POINT_H
