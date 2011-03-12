#include <stdlib.h>
#include <gtk/gtk.h>

typedef struct Cube Cube;
struct Cube
{
    cairo_rectangle_t rect1;
    cairo_rectangle_t rect2;

    cairo_t* cr;
};

void initialiser_Cube( Cube* cube, double x1, double y1, double width1, double height1, double x2, double y2, double width2, double height2, cairo_t* cr1 );

void dessiner_Cube(Cube* cube);
