#ifndef CUBE_H
#define CUBE_H

#include <stdlib.h>
#include <gtk/gtk.h>

#include "MatrixTools.h"
#include "TransfoTools.h"
#include "Point.h"
#include "Color.h"
#include "Camera.h"

typedef struct Cube Cube;
struct Cube
{
    // 0-----1
    // |     |
    // |
    // 3-----2
    Point tPoint[8];

    gboolean estSelectionne;
    Point Center; /* Coordonées du repere de l'objet, utilisé pour les transformations */
	Color tColor; /* Couleur de l'objet */
};

void Cube_drawCube(Cube* pCube, cairo_t* cr, InfoCamera* pCam);

Cube* Cube_createCube(tdCoord tCenter, double dHeight,double dWidth, double dDepth);

/**
 * Rotation d'un objet cube autour de son centre de gravité.
 * @param pCube L'objet cube à faire tourner
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 * @warning Les angles doivent être en radian
 */
void Cube_rotateCube(Cube* pCube, double dAngleX, double dAngleY, double dAngleZ);

/**
 * Rotation d'un objet cube autour du centre de l'univers.
 * @param pCube L'objet cube à faire tourner
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 * @warning Les angles doivent être en radian
 */
void Cube_rotateCubeWorld(Cube* pCube, double dAngleX, double dAngleY, double dAngleZ);

void initialiser_Cube( Cube* cCube, double dX, double dY, double dZ, double dCote );

void dessiner_Cube( Cube* cCube, cairo_t* cr, InfoCamera* cam);

void rotation_Cube( Cube* cCube, double dAngle, int iAxe );

void rotation_X( Point* pPoint, double dDecallage_Y, double dDecallage_Z, double angle );

void rotation_Y( Point* pPoint, double dDecallage_X, double dDecallage_Z, double angle );

void rotation_Z( Point* pPoint, double dDecallage_X, double dDecallage_Y, double angle );

gboolean Cube_Contient_Point( Cube* cCube, double x, double y, InfoCamera* pCam);

gboolean est_dans_face( Point a, Point b, Point c, Point d, double x, double y );

int scalaire_result( Point a, Point b, int x, int y );

/**
 * Agrandit/retrécit un objet de type cube suivant le ratio fourni en param.
 * @param pRectangle Le cube en question
 * @param dRatio Le ratio d'agrandissement/rétrecissement
 */
void Cube_modSize(Cube* pRectangle, double dRatio);

GArray* Cube_facesOrder(Cube* pCube, InfoCamera* pCam);

#endif //CUBE_H
