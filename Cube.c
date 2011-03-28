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

    cCube->estSelectionne = FALSE;

}

void dessiner_Cube(Cube* cCube, cairo_t* cr )
{
   if( !cCube->estSelectionne )
   {
        cairo_set_source_rgb ( cr, 0, 0, 0);   // On definie la couleur du trait

        cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
        cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
        cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
        cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y );
        cairo_line_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y ); // Face 1 dessinée

        cairo_fill( cr );
        cairo_set_source_rgb ( cr, 255, 0, 0);

        cairo_move_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
        cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
        cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
        cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
        cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );// Face 2 dessinée

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
        cairo_line_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y ); // Face 4 dessinée

        cairo_fill( cr );
        cairo_set_source_rgb ( cr, 255, 255, 0);

        cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
        cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
        cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
        cairo_line_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y );
        cairo_line_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y ); // Face 5 dessinée

        cairo_fill( cr );
        cairo_set_source_rgb ( cr, 255, 0, 255);

        cairo_move_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y );
        cairo_line_to( cr, cCube->tPoint[7].x, cCube->tPoint[7].y );
        cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
        cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
        cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y ); // Face 6 dessinée

        cairo_fill( cr );
    }
    else if( cCube->estSelectionne )
    {
        cairo_set_source_rgb ( cr, 255, 0, 0);

        cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
        cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
        cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
        cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y );
        cairo_line_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y ); // Face 1 dessinée

        cairo_move_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
        cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
        cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
        cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
        cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );// Face 2 dessinée

        cairo_move_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y );
        cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
        cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
        cairo_line_to( cr, cCube->tPoint[7].x, cCube->tPoint[7].y );
        cairo_line_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y ); // Face 3 dessinée

        cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
        cairo_line_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y );
        cairo_line_to( cr, cCube->tPoint[7].x, cCube->tPoint[7].y );
        cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y );
        cairo_line_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y ); // Face 4 dessinée

        cairo_move_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y );
        cairo_line_to( cr, cCube->tPoint[1].x, cCube->tPoint[1].y );
        cairo_line_to( cr, cCube->tPoint[5].x, cCube->tPoint[5].y );
        cairo_line_to( cr, cCube->tPoint[4].x, cCube->tPoint[4].y );
        cairo_line_to( cr, cCube->tPoint[0].x, cCube->tPoint[0].y ); // Face 5 dessinée

        cairo_move_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y );
        cairo_line_to( cr, cCube->tPoint[7].x, cCube->tPoint[7].y );
        cairo_line_to( cr, cCube->tPoint[6].x, cCube->tPoint[6].y );
        cairo_line_to( cr, cCube->tPoint[2].x, cCube->tPoint[2].y );
        cairo_line_to( cr, cCube->tPoint[3].x, cCube->tPoint[3].y ); // Face 6 dessinée

        cairo_set_source_rgb ( cr, 0, 255, 0);
        cairo_fill_preserve( cr );

        cairo_set_source_rgb ( cr, 255, 0, 0);
        cairo_stroke( cr );
    }
}

void rotation_Cube( Cube* cCube, double dAngle, int iAxe )
{
    double dDecallage_X = cCube->tPoint[0].x + ( cCube->tPoint[1].x - cCube->tPoint[0].x ) / 2;
    double dDecallage_Y = cCube->tPoint[0].y + ( cCube->tPoint[1].y - cCube->tPoint[0].y ) / 2;
    double dDecallage_Z = cCube->tPoint[0].z + ( cCube->tPoint[1].z - cCube->tPoint[0].z ) / 2;

    if( iAxe == 1 ) // X
    {
        int i = 0;

        for( i = 0; i < 8; i++ )
        {
            rotation_X( &cCube->tPoint[i], dDecallage_Y, dDecallage_Z, dAngle );
        }
    }
    else if( iAxe == 2 ) // Y
    {
        int i = 0;

        for( i = 0; i < 8; i++ )
        {
            rotation_Y( &cCube->tPoint[i], dDecallage_X, dDecallage_Z, dAngle );
        }
    }
    else if( iAxe == 3 ) // Z
    {
        int i = 0;

        for( i = 0; i < 8; i++ )
        {
            rotation_Z( &cCube->tPoint[i], dDecallage_X, dDecallage_Y, dAngle );
        }
    }
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

void rotation_Y( Point* pPoint, double dDecallage_X, double dDecallage_Z, double dAngle )
{
    pPoint->x -= dDecallage_X;
    pPoint->z -= dDecallage_Z;

    pPoint->x = cos( dAngle ) * pPoint->x - sin( dAngle ) * pPoint->z;
    pPoint->z = sin( dAngle ) * pPoint->x + cos( dAngle ) * pPoint->z;

    pPoint->x += dDecallage_X;
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

gboolean Cube_Contient_Point( Cube* cCube, double x, double y )
{
    if( x >= cCube->tPoint[0].x  && x <= cCube->tPoint[1].x && y >= cCube->tPoint[0].y  && y <= cCube->tPoint[3].y
    ||  x >= cCube->tPoint[1].x  && x <= cCube->tPoint[5].x && y >= cCube->tPoint[5].y  && y <= cCube->tPoint[6].y
    ||  x >= cCube->tPoint[0].x  && x <= cCube->tPoint[4].x && y >= cCube->tPoint[4].y  && y <= cCube->tPoint[7].y
    ||  x >= cCube->tPoint[0].x  && x <= cCube->tPoint[1].x && y >= cCube->tPoint[4].y  && y <= cCube->tPoint[1].y
    ||  x >= cCube->tPoint[4].x  && x <= cCube->tPoint[5].x && y >= cCube->tPoint[4].y  && y <= cCube->tPoint[7].y
    ||  x >= cCube->tPoint[3].x  && x <= cCube->tPoint[2].x && y >= cCube->tPoint[7].y  && y <= cCube->tPoint[3].y)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

