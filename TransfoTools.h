#ifndef TRANSFOTOOLS_H
#define TRANSFOTOOLS_H

#include "MatrixTools.h"


typedef enum{TRANSLATION, ROTATION} transfoType;

typedef enum axe
{
	AXEX, AXEY, AXEZ
}axe;

typedef struct Transfo
{
	/*Définit le type de transformation*/
	transfoType eTransfoType;
	/** Définit la matrice de transformation **/
	tdMatrix tdMatrixTransfo;
}Transfo;


/**
 * Mise à jour des infos de la matrice passée en param afin d'en faire une matrice de rotation.
 * @param pMatModif Matrice à modifier
 * @param dAngle Angle de rotation
 * @param eAxe L'axe autour duquel doit se faire la rotation
 */
void TransfoTools_getMatrixRotation(tdMatrix pMatModif, double dAngle, enum axe eAxe);


/**
 * Mise à jour des infos de la matrice passée en param afin d'en faire une matrice d'homothétie.
 * @param pMatModif Matrice à modifier
 * @param dRation Rapport d'agrandissement/rétrecissement
 */
void TransfoTools_getMatrixHomothety(tdMatrix pMatModif, double dRatio);


#endif
