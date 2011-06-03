#include <math.h>

#include "Point.h"
#include "MatrixTools.h"

void Point_init(Point* pPoint, double dX, double dY, double dZ)  /* TODO: Enelver ce truc */
{
	pPoint->tdCoordWorld[0]=dX;
	pPoint->tdCoordWorld[1]=dY;
	pPoint->tdCoordWorld[2]=dZ;
	pPoint->tdCoordWorld[3]=1;
}

void Point_initWorld(Point* pPoint, double dX, double dY, double dZ)
{
	pPoint->tdCoordWorld[0]=dX;
	pPoint->tdCoordWorld[1]=dY;
	pPoint->tdCoordWorld[2]=dZ;
	pPoint->tdCoordWorld[3]=1;
}

void Point_initGroup(Point* pPoint, double dX, double dY, double dZ)
{
	pPoint->tdCoordGroup[0] = dX;
	pPoint->tdCoordGroup[1] = dY;
	pPoint->tdCoordGroup[2] = dZ;
	pPoint->tdCoordGroup[3] = 1;
}

void Point_initCoord(tCoord pCoord,double dX, double dY, double dZ)
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

double Point_euclideanDistance2D(tCoord2D tP1, tCoord2D tP2)
{
	return sqrt(pow(tP2[0]-tP1[0],2.0) + pow(tP2[1]-tP1[1],2.0));
}

double Point_determinant(tCoord2D tP1, tCoord2D tP2)
{
	return -(tP1[0]*tP2[1]-tP1[1]*tP2[0]);
}

void Point_initCoord2D(tCoord pCoord,double dX, double dY)
{
	pCoord[0] = dX;
	pCoord[1] = dY;
}

void Point_transfo(Point p, tdMatrix tdTransfoMat)
{
	tCoord tdAfterTransfo;
	Matrix_multiMatrixVect(tdTransfoMat,p.tdCoordGroup,tdAfterTransfo);

	p.tdCoordGroup[0] = tdAfterTransfo[0];
	p.tdCoordGroup[1] = tdAfterTransfo[1];
	p.tdCoordGroup[2] = tdAfterTransfo[2];
	p.tdCoordGroup[3] = tdAfterTransfo[3];
}
