#ifndef PYRAMID_H
#define PYRAMID_H

#include <stdlib.h>
#include <gtk/gtk.h>

struct Objet;
#include "MatrixTools.h"
#include "Transformation.h"
#include "Point.h"
#include "Color.h"
#include "Camera.h"
#include "Selection.h"

typedef enum{SQUARE, TRIANGULAR} pyramidType;

typedef struct Pyramid Pyramid;
struct Pyramid
{
    Point tPoint[5];

    pyramidType eType;

    gboolean estSelectionne;
    Point Center; /* Coordonées du repere de l'objet, utilisé pour les transformations */
	Color tColor; /* Couleur de l'objet */
};

/**
 * Contre toute attente cette fonction permet dessiner un Pyramid à  l'écran.
 */
void Pyramid_drawPyramid(struct Objet* pObj,cairo_t* cr,InfoCamera* pCam);

Pyramid* Pyramid_createPyramid(tCoord tCenter, double dHeight,double dWidth, double dDepth);

/**
 * Rotation d'un objet Pyramid autour de son centre de gravité.
 * @param pPyramid L'objet Pyramid à faire tourner
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 * @warning Les angles doivent être en radian
 */
void Pyramid_rotatePyramid(Pyramid* pPyramid, double dAngleX, double dAngleY, double dAngleZ);

/**
 * Rotation d'un objet Pyramid autour du centre de l'univers.
 * @param pPyramid L'objet Pyramid à faire tourner
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 * @warning Les angles doivent être en radian
 */
void Pyramid_rotatePyramidWorld(Pyramid* pPyramid, double dAngleX, double dAngleY, double dAngleZ);


void Pyramid_updateCoordWolrd(struct Objet* pObj);

void initialiser_Pyramid( Pyramid* cPyramid, double dX, double dY, double dZ, double dCote );

void dessiner_Pyramid( Pyramid* cPyramid, cairo_t* cr, InfoCamera* cam);

gboolean Pyramid_Contient_Point( Pyramid* cPyramid, double x, double y, InfoCamera* pCam);

//gboolean est_dans_face( tdCoord2D* a, tdCoord2D* b, tdCoord2D* c, tdCoord2D* d, double x, double y );

int scalaire_result( tCoord2D* a, tCoord2D* b, int x, int y );

/**
 * Agrandit/retrécit un objet de type Pyramid suivant le ratio fourni en param.
 * @param pRectangle Le Pyramid en question
 * @param dRatio Le ratio d'agrandissement/rétrecissement
 */
void Pyramid_modSize(Pyramid* pPyramid, double dRatio);

GArray* Pyramid_facesOrder(Pyramid* pPyramid, InfoCamera* pCam);

/**
 * Va appliquer la transformation de la matrice passée en paramètre à tous les points de l'objet.
 *
 * @param pRec L'objet Pyramid concerné
 * @param tdTransfoMat La matrice de transformation à appliquer
 */
void Pyramid_transfo(Pyramid* pPyramid, tdMatrix tdTransfoMat);

void Pyramid_transfoCenter(Pyramid* pPyramid, tdMatrix tdTransfoMat);

#endif //Pyramid_H
