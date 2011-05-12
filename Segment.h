#ifndef SEGMENT_H
#define SEGMENT_H

#include <cairo.h>
#include <gtk/gtk.h>

#include "Point.h"
#include "ProjectionTools.h"
#include "Camera.h"
#include "Color.h"

typedef struct Segment
{
	/**
	 * 	0+---+1
	 */
	Point tPoint[2];

	Point Center; /* Coordonées du repere de l'objet, utilisé pour les transformations */
	Color tColor; /* Couleur de l'objet */
}Segment;


/**
 * Crée un Segment à partir de 2 matrices de coordonnées.
 *
 * @param tdCoord1 Position première extremité
 * @param tdCoord2 Position seconde extrémité
 *
 * @return un structure Segment fraichement allouée
 */
Segment* Segment_createSegment(tdCoord tdCoord1,tdCoord tdCoord2);


/**
 * Dessine le Segment dans le plan de projection de la caméra passé en param
 */
void Segment_drawSegment(Segment* pSeg, cairo_t* cr, InfoCamera* pCam);

/**
 * Suppression d'une structure sSegment allouée
 *
 * @param pRect Pointeur ver sle structure à desallouer
 */
void Segment_destroySegment(Segment* pSeg);

/**
 * Rotation de l'objet en fonction des angles passés en paramètre, la rotation se fait par rapport au repere de l'objet.
 *
 * @param  pSegment L'objet Segment à considérer
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 */
void Segment_rotate(Segment* pSeg, double dAngleX, double dAngleY, double dAngleZ);

gboolean Segment_Contient_Point(Segment* pRect, double x, double y );

void Segment_setColor(Segment* pSeg,double dR, double dG, double dB);

#endif
