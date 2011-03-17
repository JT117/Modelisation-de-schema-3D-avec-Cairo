#include <stdlib.h>
#include "Point.h"
#include <gtk/gtk.h>

typedef struct Cube Cube;
struct Cube
{
    double x;
    double y;
    double cote;

    double rotation_X;
   // double rotation_Y;
    double rotation_Z;
};

void initialiser_Cube( Cube* cube, double x1, double y1, double cote1 );

void dessiner_Cube( Cube* cube, cairo_t* cr );

void rotation_Y( Point* point, double reference_X, double angle );
