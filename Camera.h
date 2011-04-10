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

	/* Matrice de projection prenant en compte de toutes les informations renseignées dans cette structure*/
	tdMatrix tdMatPassage;
	/* Matrice de projection prenant en compte de toutes les informations renseignées dans cette structure*/
	tdMatrix tdMatProj;
}sInfoCamera;

/**
 * Permet de mettre à jour les coordonnées de la caméra dans le repère de l'univers.
 *
 * @param pCam La structure sInfoCamera à modifier
 * @param dX Coordonée sur l'axe X du point de référence de la caméra dans le repère du monde
 * @param dY Coordonée sur l'axe Y
 * @param dY Coordonée sur l'axe Z
 */
void Camera_setCoordCam(sInfoCamera* pCam, double dX, double dY, double dZ);

/**
 * Met à jour la matrice de passage du référentiel de l'univers dans la référentiel de la caméra
 *
 * @param pCam La structure sInfoCamera à modifier
 * @param dX Coordonée sur l'axe X du point de référence de la caméra dans le repère du monde
 * @param dY Coordonée sur l'axe Y
 * @param dY Coordonée sur l'axe Z
 */
void Camera_setMatPassage(sInfoCamera* pCam,double dX, double dY, double dZ);

/**
 * Fonction permettant de modifier les valeurs du frustum de la caméra, en pratique ces valeur définissent
 * une pyramide à quatre côté représentant le champ de vision de la caméra.
 *
 * @param pCam Structure sInfoCamera à modifier
 * @param dAngleV Demi angle d'ouverture vertical
 * @param dRatio Aspect ratio de l'image
 */
void Camera_setFrustum(sInfoCamera* pCam, double dAngleV, double dRatio);

/**
 * Modifie les valeurs de distances des near et far plan.
 *
 * @param pCam La caméra concernée
 * @param dMin Distance near plan
 * @param dMax Distance far plan
 */
void Camera_setDistancePlan(sInfoCamera* pCam, double dDMin, double dDmax);

/**
 * Calcule la matrice projection liée à une caméra donnée.
 *
 * @param La caméra concernée
 */
void Camera_projectionCalculation(sInfoCamera *pCam);

#endif
