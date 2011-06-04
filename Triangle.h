#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdlib.h>
#include <gtk/gtk.h>

struct Objet;
#include "MatrixTools.h"
#include "Transformation.h"
#include "Point.h"
#include "Color.h"
#include "Camera.h"
#include "Selection.h"


typedef struct Triangle Triangle;
struct Triangle
{
    Point tPoint[3];

    gboolean estSelectionne;
    Point Center; /* Coordonées du repere de l'objet, utilisé pour les transformations */
	Color tColor; /* Couleur de l'objet */
};

/**
 * Contre toute attente cette fonction permet dessiner un Triangle à  l'écran.
 */
void Triangle_drawTriangle(struct Objet* pObj,cairo_t* cr,InfoCamera* pCam);

Triangle* Triangle_createTriangle(tCoord tCenter, tCoord tPoint1,tCoord tPoint2, tCoord tPoint3);

/**
 * Rotation d'un objet Triangle autour de son centre de gravité.
 * @param pTriangle L'objet Triangle à faire tourner
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 * @warning Les angles doivent être en radian
 */
void Triangle_rotateTriangle(Triangle* pTriangle, double dAngleX, double dAngleY, double dAngleZ);

/**
 * Rotation d'un objet Triangle autour du centre de l'univers.
 * @param pTriangle L'objet Triangle à faire tourner
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 * @warning Les angles doivent être en radian
 */
void Triangle_rotateTriangleWorld(Triangle* pTriangle, double dAngleX, double dAngleY, double dAngleZ);


void Triangle_updateCoordWolrd(struct Objet* pObj);

void initialiser_Triangle( Triangle* cTriangle, double dX, double dY, double dZ, double dCote );

void dessiner_Triangle( Triangle* cTriangle, cairo_t* cr, InfoCamera* cam);

gboolean Triangle_Contient_Point( Triangle* cTriangle, double x, double y, InfoCamera* pCam);

//gboolean est_dans_face( tdCoord2D* a, tdCoord2D* b, tdCoord2D* c, tdCoord2D* d, double x, double y );

int scalaire_result( tCoord2D* a, tCoord2D* b, int x, int y );

/**
 * Agrandit/retrécit un objet de type Triangle suivant le ratio fourni en param.
 * @param pRectangle Le Triangle en question
 * @param dRatio Le ratio d'agrandissement/rétrecissement
 */
void Triangle_modSize(Triangle* pRectangle, double dRatio);

GArray* Triangle_facesOrder(Triangle* pTriangle, InfoCamera* pCam);

/**
 * Va appliquer la transformation de la matrice passée en paramètre à tous les points de l'objet.
 *
 * @param pRec L'objet Triangle concerné
 * @param tdTransfoMat La matrice de transformation à appliquer
 */
void Triangle_transfo(Triangle* pTriangle, tdMatrix tdTransfoMat);

#endif //TRIANGLE_H
