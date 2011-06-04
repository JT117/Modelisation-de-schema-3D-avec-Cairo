#ifndef Quadrilateral_H
#define Quadrilateral_H

#include <cairo.h>
#include <gtk/gtk.h>

#include "Point.h"
#include "MatrixTools.h"
#include "Transformation.h"
#include "ProjectionTools.h"
#include "Camera.h"
#include "Color.h"
#include "Groupe.h"
struct Objet;

typedef struct Quadrilateral
{
	/**
	 * Ordres des points du rectangle:
	 * 	0+---+1
	 * 	 |	 |
	 * 	3+---+2
	 */
	Point tPoint[4];

    gboolean estSelectionne;
	Point Center; /* Coordonées du repere de l'objet, utilisé pour les transformations */
	Color tColor; /* Couleur de l'objet */
} Quadrilateral;


void Quadrilateral_updateCoordWolrd(struct Objet* pObj);

/**
 * Crée un rectangle à partir de 2 points
 *
 * @param tdCorner1 position du sommet haut gauche
 * @param tdCorner2 position du sommet bas droit
 * @param tdCenter Position du centre de gravité du rectangle
 *
 * @return un structure sRectangle fraichement allouée
 */
Quadrilateral* Quadrilateral_createQuadrilateral(tCoord tdCenter,tCoord2D tP1,tCoord2D tP2,tCoord2D tP3,tCoord2D tP4);

/**
 * Dessine le Quadrilateral dans le plan de projection de la caméra passé en param
 */
void Quadrilateral_drawQuadrilateral( struct Objet* pObj, cairo_t* cr, InfoCamera* pCam);


/**
 * Suppression d'une structure sQuadrilateral allouée
 *
 * @param pRect Pointeur ver sle structure à desallouer
 */
void Quadrilateral_destroyQuadrilateral(Quadrilateral* pQuadri);

/**
 * Rotation de l'objet en fonction des angles passés en paramètre, la rotation se fait par rapport au repere de l'objet.
 * @param  pQuadrilateral L'objet Quadrilateral à considérer
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 */
void Quadrilateral_rotate(Quadrilateral* pQuadri, double dAngleX, double dAngleY, double dAngleZ);

/**
 * Rotation de l'objet Quadrilateral par rapport au centre de l'univers
 * @param  pQuadrilateral L'objet Quadrilateral à considérer
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 */
void Quadrilateral_rotateWorld(Quadrilateral* pQuadri, double dAngleX, double dAngleY, double dAngleZ);

/**
 * Agrandit/retrécit un objet de type Quadrilateral suivant le ratio fourni en param.
 * @param pQuadrilateral Le Quadrilateral en question
 * @param dRatio Le ratio d'agrandissement/rétrecissement
 */
void Quadrilateral_modSize(Quadrilateral* pQuadri, double dRatio);

gboolean Quadrilateral_Contient_Point(Quadrilateral* pQuadri, double x, double y, InfoCamera* pCam);


/**
 * Va appliquer la transformation de la matrice passée en paramètre à tous les points de l'objet.
 *
 * @param pRec L'objet Quadrilateral concerné
 * @param tdTransfoMat La matrice de transformation à appliquer
 */
void Quadrilateral_transfo(Quadrilateral* pQuadri, tdMatrix tdTransfoMat);

#endif
