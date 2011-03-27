#ifndef CUBE_H
#define CUBE_H

#include <stdlib.h>
#include "Point.h"
#include <gtk/gtk.h>

typedef struct Cube Cube;
struct Cube
{
    // 0-----1
    // |     |
    // |     |
    // 3-----2

    Point tPoint[8];
    gboolean estSelectionne;

};

void initialiser_Cube( Cube* cCube, double dX, double dY, double dZ, double dCote );

void dessiner_Cube( Cube* cCube, cairo_t* cr );

void rotation_Cube( Cube* cCube, double dAngle, int iAxe );

void rotation_X( Point* pPoint, double dDecallage_Y, double dDecallage_Z, double angle );

void rotation_Y( Point* pPoint, double dDecallage_X, double dDecallage_Z, double angle );

void rotation_Z( Point* pPoint, double dDecallage_X, double dDecallage_Y, double angle );

gboolean contientPoint( Cube* cCube, double x, double y );


#endif //CUBE_H
