#ifndef CAMERA_H
#define CAMERA_H

#include "Point.h"
#include "MatrixTools.h"

typedef struct InfoCamera
{
	/** Position de la caméra dans le reférentiel de l'univers**/
	tdCoord CoordCam;

	/** Distance séparant le point de référence de la caméra (view point) du nearPlan **/
	double dDMin;
	/** Distance séparant le point de référence de la caméra du farPlan **/
	double dDMax;

	/* Definition des informatiosn relatives au frustum de la caméra*/
	double dUmin,dUmax,dRmin,dRmax;

	/* Matrice de passage pour passer en coordonées caméra*/
	tdMatrix tdMatPassage;
	/* Matrice de projection prenant en compte de toutes les informations renseignées dans cette structure*/
	tdMatrix tdMatProj;
}InfoCamera;

/**
 * Permet de mettre à jour les coordonnées de la caméra dans le repère de l'univers.
 *
 * @param pCam La structure InfoCamera à modifier
 * @param dX Coordonée sur l'axe X du point de référence de la caméra dans le repère du monde
 * @param dY Coordonée sur l'axe Y
 * @param dY Coordonée sur l'axe Z
 */
void Camera_setCoordCam(InfoCamera* pCam, double dX, double dY, double dZ);

/**
 * Met à jour la matrice de passage du référentiel de l'univers dans la référentiel de la caméra
 *
 * @param pCam La structure InfoCamera à modifier
 * @param dX Coordonée sur l'axe X du point de référence de la caméra dans le repère du monde
 * @param dY Coordonée sur l'axe Y
 * @param dY Coordonée sur l'axe Z
 */
void Camera_setMatPassage(InfoCamera* pCam,double dX, double dY, double dZ);

/**
 * Fonction permettant de modifier les valeurs du frustum de la caméra, en pratique ces valeur définissent
 * une pyramide à quatre côté représentant le champ de vision de la caméra.
 *
 * @param pCam Structure InfoCamera à modifier
 * @param dAngleV Demi angle d'ouverture vertical
 * @param dRatio Aspect ratio de l'image
 */
void Camera_setFrustum(InfoCamera* pCam, double dAngleV, double dRatio);

/**
 * Modifie les valeurs de distances des near et far plan.
 *
 * @param pCam La caméra concernée
 * @param dMin Distance near plan
 * @param dMax Distance far plan
 */
void Camera_setDistancePlan(InfoCamera* pCam, double dDMin, double dDmax);

/**
 * Calcule la matrice projection liée à une caméra donnée.
 *
 * @param La caméra concernée
 */
void Camera_projectionCalculation(InfoCamera *pCam);

/**
 * Création d'une camera par défaut (temporaire vu qu'inutile)
 *
 */
InfoCamera* Camera_createDefaultCam();

/**
 * Création d'une caméra positionnée correctement dans le repère de l'univers afin d'avoir un plan de projection
 * de la taille indiquée en paramètre.
 *
 * @param dLength Longueur
 * @param dWidth Largeur
 */
InfoCamera* Camera_createCam(double dLength, double dWidth);

#endif
