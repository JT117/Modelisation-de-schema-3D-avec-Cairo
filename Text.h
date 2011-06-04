#ifndef TEXT_H
#define TEXT_H

#include <stdlib.h>
#include <gtk/gtk.h>

#include "MatrixTools.h"
#include "Transformation.h"
#include "Point.h"
#include "Color.h"
#include "Camera.h"

typedef struct Text Text;
struct Text
{
	/* Position sur la projection du texte */
	tCoord2D coordText;

	/*Ce qui doit être affiché*/
	char* pText;

    gboolean estSelectionne;
	Color tColor; /* Couleur de l'objet */

	/* Taille du texte à afficher */
	double dSize;
};

void Text_drawText(Text* pText, cairo_t* cr, InfoCamera* pCam);

/**
 * Va créer un nouvel objet Text.
 *
 * @param tPos Position du nouveau texte
 * @param pText Chaîne à afficher
 *
 * @return Nouvelle structure texte fraichement allouée
 */
Text* Text_createText(tCoord2D tPos, char* pText);

/**
 * Rotation d'un objet Text autour du centre de l'univers.
 * @param pText L'objet Text à faire tourner
 * @param dAngleX Angle de rotation suivant l'axe X
 * @param dAngleY Angle de rotation suivant l'axe Y
 * @param dAngleZ Angle de rotation suivant l'axe Z
 * @warning Les angles doivent être en radian
 */
void Text_rotateTextWorld(Text* pText, double dAngleX, double dAngleY, double dAngleZ);

gboolean Text_Contient_Point( Text* pText, double x, double y );

/*
int scalaire_result( Point a, Point b, int x, int y );
*/

/**
 * Agrandit/retrécit un objet de type Text suivant le ratio fourni en param.
 * @param pRectangle Le Text en question
 * @param dRatio Le ratio d'agrandissement/rétrecissement
 */
void Text_modSize(Text* pText, double dRatio);


#endif //Text_H
