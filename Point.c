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

void Point_middlePoint(Point* pRes, Point* pP1, Point* pP2)
{
	Point_init(pRes, (pP2->tdCoordWorld[0]+pP1->tdCoordWorld[0])/2, (pP2->tdCoordWorld[1]+pP1->tdCoordWorld[1])/2,
							(pP2->tdCoordWorld[2]+pP1->tdCoordWorld[2])/2);
}

double Point_euclideanDistance(Point* pP1, Point* pP2)
{
	return sqrt(pow(pP2->tdCoordWorld[0]-pP1->tdCoordWorld[0],2.0) + pow(pP2->tdCoordWorld[1]-pP1->tdCoordWorld[1],2.0)
							+ pow(pP2->tdCoordWorld[2]-pP1->tdCoordWorld[2],2.0));
}
