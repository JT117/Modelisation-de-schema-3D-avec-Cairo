#ifndef TRANSFOTOOLS_H
#define TRANSFOTOOLS_H

#include "MatrixTools.h"
#include "Groupe.h"


typedef enum{TRANSLATION, ROTATION, ROTATION_RECU, TRANSLATION_RECU, HOMOTHETIE} transfoType;

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
	/** Necessaire pour la sauvegarde */
	double x, y, z;
}Transfo;


/**
 * Mise à jour des infos de la matrice passée en param afin d'en faire une matrice de rotation.
 * @param pMatModif Matrice à modifier
 * @param dAngle Angle de rotation
 * @param eAxe L'axe autour duquel doit se faire la rotation
 */
void Transformation_getMatrixRotation(tdMatrix pMatModif, double dAngle, enum axe eAxe);


/**
 * Mise à jour des infos de la matrice passée en param afin d'en faire une matrice d'homothétie.
 * @param pMatModif Matrice à modifier
 * @param dRation Rapport d'agrandissement/rétrecissement
 */
void Transformation_getMatrixHomothety(tdMatrix pMatModif, double dRatio);


void Transformation_getMatrixTranslation(tdMatrix pMatModif, double dX, double dY, double dZ);

#endif
