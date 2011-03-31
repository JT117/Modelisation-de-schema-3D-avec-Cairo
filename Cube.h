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

gboolean Cube_Contient_Point( Cube* cCube, double x, double y );

gboolean est_dans_face( Point a, Point b, Point c, Point d, double x, double y );

int scalaire_result( Point a, Point b, int x, int y );



#endif //CUBE_H
