#include "Cube.h"
#include <math.h>

void initialiser_Cube( Cube* cCube, double dX, double dY, double dZ, double dCote )
{
    cCube->tPoint[0].x = dX;
    cCube->tPoint[0].y = dY;
    cCube->tPoint[0].z = dZ;

    cCube->tPoint[1].x = dX + dCote;
    cCube->tPoint[1].y = dY;
    cCube->tPoint[1].z = dZ;

    cCube->tPoint[2].x = dX + dCote;
    cCube->tPoint[2].y = dY + dCote;
    cCube->tPoint[2].z = dZ;

    cCube->tPoint[3].x = dX;
    cCube->tPoint[3].y = dY + dCote;
    cCube->tPoint[3].z = dZ;

    cCube->tPoint[4].x = dX;
    cCube->tPoint[4].y = dY;
    cCube->tPoint[4].z = dZ + dCote;

    cCube->tPoint[5].x = dX + dCote;
    cCube->tPoint[5].y = dY;
    cCube->tPoint[5].z = dZ + dCote;

    cCube->tPoint[6].x = dX + dCote;
    cCube->tPoint[6].y = dY + dCote;
    cCube->tPoint[6].z = dZ + dCote;

    cCube->tPoint[7].x = dX;
    cCube->tPoint[7].y = dY + dCote;
    cCube->tPoint[7].z = dZ + dCote;

}

void dessiner_Cube(Cube* cCube, cairo_t* cr )
{
    int i = 0;

    cairo_set_source_rgb ( cr, 0, 0, 0);   // On definie la couleur du trait

    cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
    cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
    cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
    cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y ); // Face 1 dessinée

    cairo_fill( cr );
    cairo_set_source_rgb ( cr, 255, 0, 0);

    cairo_move_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
    cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
    cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
    cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y ); // Face 2 dessinée

    cairo_fill( cr );
    cairo_set_source_rgb ( cr, 0, 255, 0);

    cairo_move_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y );
    cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
    cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
    cairo_line_to( cr, cCube->tPoint[7].x, cCube->tPoint[7].y );
    cairo_line_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y ); // Face 3 dessinée

    cairo_fill( cr );
    cairo_set_source_rgb ( cr, 0, 0, 255);

    cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
    cairo_line_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y );
    cairo_line_to( cr, cCube->tPoint[7].x, cCube->tPoint[7].y );
    cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y );
    cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y ); // Face 4 dessinée

    cairo_fill( cr );
    cairo_set_source_rgb ( cr, 255, 255, 0);

    cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
    cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
    cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
    cairo_line_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y );
    cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y ); // Face 5 dessinée

    cairo_fill( cr );
    cairo_set_source_rgb ( cr, 255, 0, 255);

    cairo_move_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y );
    cairo_line_to( cr, cCube->tPoint[7].x, cCube->tPoint[7].y );
    cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
    cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
    cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y ); // Face 3 de6sinée

    cairo_fill( cr );
    cairo_set_source_rgb ( cr, 255, 255, 255);


    //cairo_stroke( cr ); // On ne dessine que les contours
    /*double x1 = cube->x;
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

    d.x = cube->x;
    d.y = cube->y + cube->cote;
    d.z = 0;

    c.x = cube->x + cube->cote;
    c.y = cube->y + cube->cote;
    c.z = 0;

    double dDecallage_X = a.x + cube->cote / 2;
    double dDecallage_Y = a.y + cube->cote / 2;
    double dDecallage_Z = a.z + cube->cote / 2;

    printf( "Avant A: %f, B: %f, C: %f, D: %f \n", a.x, b.x, c.x, d.x );

    rotation_Y( &a, dDecallage_X, dDecallage_Z, 2*M_PI);
    rotation_Y( &d, dDecallage_X, dDecallage_Z, 2*M_PI);

    rotation_X( &c, dDecallage_Y, dDecallage_Z, 2*M_PI);
    rotation_X( &d, dDecallage_Y, dDecallage_Z, 2*M_PI);

    rotation_Z( &a, dDecallage_X, dDecallage_Y, M_PI/2);
    rotation_Z( &b, dDecallage_X, dDecallage_Y, M_PI/2);
    rotation_Z( &c, dDecallage_X, dDecallage_Y, M_PI/2);
    rotation_Z( &d, dDecallage_X, dDecallage_Y, M_PI/2);

    printf( "Apres A: %f, B: %f, C: %f, D: %f \n", a.x, b.x, c.x, d.x );

    cairo_move_to( cr, a.x, a.y );

    cairo_line_to( cr, b.x, b.y );
    cairo_line_to( cr, c.x, c.y );
    cairo_line_to( cr, d.x, d.y );
    cairo_line_to( cr, a.x, a.y );

    cairo_set_source_rgb ( cr, 0, 0, 0);   // On definie la couleur du trait

    cairo_stroke( cr ); // On ne dessine que les contours*/
}

void rotation_Y( Point* pPoint, double dDecallage_X, double dDecallage_Z, double dAngle )
{
    pPoint->x -= dDecallage_X;
    pPoint->z -= dDecallage_Z;

    pPoint->x = cos( dAngle ) * pPoint->x - sin( dAngle ) * pPoint->z;
    pPoint->z = sin( dAngle ) * pPoint->x + cos( dAngle ) * pPoint->z;

    pPoint->x += dDecallage_X;
    pPoint->z += dDecallage_Z;

}

void rotation_X( Point* pPoint, double dDecallage_Y, double dDecallage_Z, double dAngle )
{
    pPoint->y -= dDecallage_Y;
    pPoint->z -= dDecallage_Z;

    pPoint->y = cos( dAngle ) * pPoint->y + sin( dAngle ) * pPoint->z;
    pPoint->z = -sin( dAngle ) * pPoint->y + cos( dAngle ) * pPoint->z;

    pPoint->y += dDecallage_Y;
    pPoint->z += dDecallage_Z;

}

void rotation_Z( Point* pPoint, double dDecallage_X, double dDecallage_Y, double dAngle )
{
    pPoint->x -= dDecallage_X;
    pPoint->y -= dDecallage_Y;

    pPoint->x = cos( dAngle ) * pPoint->x + sin( dAngle ) * pPoint->y;
    pPoint->y = -sin( dAngle ) * pPoint->x + cos( dAngle ) * pPoint->y;

    pPoint->x += dDecallage_X;
    pPoint->y += dDecallage_Y;

}


