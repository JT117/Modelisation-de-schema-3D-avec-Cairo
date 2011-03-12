#include "Cube.h"

void initialiser_Cube( Cube* cube, double x1, double y1, double width1, double height1, double x2, double y2, double width2, double height2, cairo_t* cr1 )
{
    cube->rect1.x = x1;
    cube->rect1.y = y1;
    cube->rect1.width = width1;
    cube->rect1.height = height1;

    cube->rect2.x = x2;
    cube->rect2.y = y2;
    cube->rect2.width = width2;
    cube->rect2.height = height2;

    cube->cr = cr1;
}

void dessiner_Cube(Cube* cube )
{
    printf("checkpoint 2 - %f - %f \n", cube->rect1.y, cube->rect1.x );
    cairo_rectangle (cube->cr, cube->rect1.x, cube->rect1.y, cube->rect1.width, cube->rect1.height); // On dessine le rectangle sur la surface
    printf("coucou \n");
    cairo_rectangle (cube->cr, cube->rect2.x, cube->rect2.y, cube->rect2.width, cube->rect2.height); // On dessine le rectangle sur la surface

    cairo_line_to( cube->cr, cube->rect1.x, cube->rect1.y );                             // On dessine à partir des coordonnées du cr jusqu'au point donné

    cairo_move_to( cube->cr, cube->rect2.x + cube->rect2.width, cube->rect2.y + cube->rect2.height ); // On deplace le curseur de cr
    cairo_line_to( cube->cr, cube->rect1.x + cube->rect1.width, cube->rect1.y + cube->rect2.height ); // On dessine à partir des coordonnées du cr jusqu'au point donné

    cairo_move_to( cube->cr, cube->rect2.x + cube->rect2.width, cube->rect2.y );
    cairo_line_to( cube->cr, cube->rect1.x + cube->rect1.width, cube->rect1.y );

    cairo_move_to( cube->cr, cube->rect2.x, cube->rect2.y + cube->rect2.height );
    cairo_line_to( cube->cr, cube->rect1.x, cube->rect1.y + cube->rect2.height );
printf("Checkpoint 1 \n");
    cairo_set_source_rgb (cube->cr, 0, 255, 0);   // On definie la couleur du trait

    cairo_stroke(cube->cr); // On ne dessine que les contours
}
