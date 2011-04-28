#include <math.h>

#include "Point.h"
#include "MatrixTools.h"

/**
 * Rotation suivant l'axe de référence X
 */
void Point_rotateX(Point* pPoint, double angle)
{

}

/**
 * Rotation suivant l'axe de référence X
 */
void Point_rotateY(Point* pPoint, double angle)
{

}

/**
 * Rotation suivant l'axe de référence X
 */
void Point_rotateZ(Point* pPoint, double angle)
{

}

void Point_init(Point* pPoint, double dX, double dY, double dZ)
{
	pPoint->tdCoordWorld[0]=dX;
	pPoint->tdCoordWorld[1]=dY;
	pPoint->tdCoordWorld[2]=dZ;
	pPoint->tdCoordWorld[3]=1;
}


void Point_initCoord(tdCoord pCoord,double dX, double dY, double dZ)
{
	pCoord[0] = dX;
	pCoord[1] = dY;
	pCoord[2] = dZ;
	pCoord[3] = 1;
}
