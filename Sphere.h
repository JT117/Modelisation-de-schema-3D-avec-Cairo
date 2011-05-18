#ifndef SPHERE_H
#define SPHERE_H

#include <stdlib.h>
#include <gtk/gtk.h>

#include "MatrixTools.h"
#include "TransfoTools.h"
#include "Point.h"
#include "Color.h"
#include "Camera.h"

typedef struct Sphere Sphere;
struct Sphere
{
    /* Rayon de la sphere*/
    Point tPoint[2];

    gboolean estSelectionne;
    Point Center; /* Coordonées du repere de l'objet, utilisé pour les transformations */
	Color tColor; /* Couleur de l'objet */
};

void Sphere_drawSphere(Sphere* pSphere, cairo_t* cr, InfoCamera* pCam);

Sphere* Sphere_createSphere(tdCoord tCenter, double dRadius);

/**
 * Rotation d'un objet Sphere autour du centre de l'univers.
 * @param pSphere L'objet Sphere à faire tourner
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 * @warning Les angles doivent être en radian
 */
void Sphere_rotateSphereWorld(Sphere* pSphere, double dAngleX, double dAngleY, double dAngleZ);

gboolean Sphere_Contient_Point( Sphere* cSphere, double x, double y );

gboolean est_dans_face( Point a, Point b, Point c, Point d, double x, double y );

int scalaire_result( Point a, Point b, int x, int y );

/**
 * Agrandit/retrécit un objet de type Sphere suivant le ratio fourni en param.
 * @param pRectangle Le Sphere en question
 * @param dRatio Le ratio d'agrandissement/rétrecissement
 */
void Sphere_modSize(Sphere* pRectangle, double dRatio);


#endif //Sphere_H
