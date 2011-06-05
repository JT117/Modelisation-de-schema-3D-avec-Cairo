#ifndef SPHERE_H
#define SPHERE_H

#include <stdlib.h>
#include <gtk/gtk.h>

struct Objet;
#include "MatrixTools.h"
#include "Transformation.h"
#include "Point.h"
#include "Color.h"
#include "Camera.h"

typedef struct Sphere Sphere;
struct Sphere
{
    /* Rayon de la sphere*/
    Point tPoint[1];

    gboolean estSelectionne;
    Point Center; /* Coordonées du repere de l'objet, utilisé pour les transformations */
	Color tColor; /* Couleur de l'objet */

	/*Garde une trace du rayon de la sphere, utile notamment pour la fonction
	 * de rotation afin de garder la distance centre-point pour tracé et
	 * ne pas deformer la taille de la sphere en faisant tourner le
	 * poitn en question autour de Y (et ainsi eviter la déformation de la sphere projettée*/
	double dRadius;  /* TODO : enlever ce long commentaire et le mettre dans le rapport */
};

void Sphere_drawSphere(struct Objet* pObj, cairo_t* cr, InfoCamera* pCam);

Sphere* Sphere_createSphere(tCoord tCenter, double dRadius);

/**
 * Rotation d'un objet Sphere autour du centre de l'univers.
 * @param pSphere L'objet Sphere à faire tourner
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 * @warning Les angles doivent être en radian
 */
void Sphere_rotateWorld(Sphere* pSphere, double dAngleX, double dAngleY, double dAngleZ);

gboolean Sphere_est_dans_face( Point a, Point b, Point c, Point d, double x, double y );

int Sphere_scalaire_result( Point a, Point b, int x, int y );

/**
 * Agrandit/retrécit un objet de type Sphere suivant le ratio fourni en param.
 * @param pRectangle Le Sphere en question
 * @param dRatio Le ratio d'agrandissement/rétrecissement
 */
void Sphere_modSize(Sphere* pRectangle, double dRatio);

/**
 * Indique si la position du clique de l'utilisateur correspond à la sphere.
 */
gboolean Sphere_Contient_Point( Sphere* pSphere, double x, double y, InfoCamera* pCam);

void Sphere_transfo(Sphere* pSphere, tdMatrix tdTransfoMat);

void Sphere_transfoCenter(Sphere* pSphere, tdMatrix tdTransfoMat);


#endif //Sphere_H
