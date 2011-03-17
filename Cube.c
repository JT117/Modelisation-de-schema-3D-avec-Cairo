#include "Cube.h"
#include <math.h>

void initialiser_Cube( Cube* cube, double x1, double y1, double cote1 )
{
    cube->x = x1;
    cube->y = y1;
    cube->cote = cote1;

    cube->rotation_X = 0;
   // cube->rotation_Y = 0;
    cube->rotation_Z = 0;
}

void dessiner_Cube(Cube* cube, cairo_t* cr )
{
    double x1 = cube->x;
    double y1 = cube->y;
    double cote = cube->cote;
    double decallage_X = 50;
    double decallage_Y = 50;
    double x2 = cube->x + decallage_X;
    double y2 = cube->y + decallage_Y;

    cairo_rectangle ( cr, x1, y1, cote, cote); // On dessine le rectangle sur la surface
    cairo_rectangle ( cr, x2, y2, cote, cote); // On dessine le rectangle sur la surface

    cairo_line_to( cr, x1, y1 );               // On dessine à partir des coordonnées du cr jusqu'au point donné

    cairo_move_to( cr, x2 + cote, y2 + cote ); // On deplace le curseur de cr
    cairo_line_to( cr, x1 + cote, y1 + cote ); // On dessine à partir des coordonnées du cr jusqu'au point donné

    cairo_move_to( cr, x2 + cote, y2 );
    cairo_line_to( cr, x1 + cote, y1 );

    cairo_move_to( cr, x2, y2 + cote );
    cairo_line_to( cr, x1, y1 + cote );

    cairo_set_source_rgb ( cr, 0, 255, 0);   // On definie la couleur du trait

    cairo_stroke( cr ); // On ne dessine que les contours

    Point a, b, c, d;

    a.x = cube->x;
    a.y = cube->y;
    a.z = 0;

    b.x = cube->x + cube->cote;
    b.y = cube->y;
    b.z = 0;

    c.x = cube->x;
    c.y = cube->y + cube->cote;
    c.z = 0;

    d.x = cube->x + cube->cote;
    d.y = cube->y + cube->cote;
    d.z = 0;

    double reference_X = a.x + cube->cote / 2;

    printf( "Avant A: %f, B: %f, C: %f, D: %f \n", a.x, b.x, c.x, d.x );

    rotation_Y( &a, reference_X, M_PI/2);
    //rotation_Y( &b, reference_X, 25 );
    rotation_Y( &c, reference_X, M_PI/2 );
    //rotation_Y( &d, reference_X, 25 );

    printf( "Apres A: %f, B: %f, C: %f, D: %f \n", a.x, b.x, c.x, d.x );

    cairo_move_to( cr, a.x, a.y );

    cairo_line_to( cr, b.x, b.y );
    cairo_line_to( cr, d.x, d.y );
    cairo_line_to( cr, c.x, c.y );
    cairo_line_to( cr, a.x, a.y );

    cairo_set_source_rgb ( cr, 0, 0, 0);   // On definie la couleur du trait

    cairo_stroke( cr ); // On ne dessine que les contours
}

void rotation_Y( Point* point, double reference_X, double angle )
{
    double x = point->x;
    double z = point->z;

    point->x = x - point->y * 0 * sin( angle) + 1 - cos( angle) + z * 325 * sin(angle) + 1-cos(angle);

    //printf(" cos( angle ) = %f | sin( angle ) = %f \n", cos(angle), sin(angle) );


    //point->z = sin( angle ) + cos( angle );
}

