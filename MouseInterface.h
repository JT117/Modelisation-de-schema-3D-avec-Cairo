#ifndef MOUSEINTERFACE_H
#define MOUSEINTERFACE_H
/**
 * Ce module va gérer l'ensemble des actions que l'utilisateurs peut effectuer avec sa souris sur la zone de dessin
 */

#define THRESHOLD 10

#include "Objet.h"
#include "Scene.h"
#include "FenSeg.h"



/**
 * Structure qui va nous permettre de créer un segment entre deux points selecitonnés par l'utilisateur
 */
typedef struct createSegment
{
	Objet* pObj1; /* Pointeur vers le premier objet selectionné */
	Objet* pObj2; /* Pointeur vers le second objet*/
	Point* pPoint1;
	Point* pPoint2;
	int iPhase; /*Indique à quelle phase de la création on est */
}createSegment;

/**
 * Va permettre de gérer les actions du curseur segment permettant de créer un segment entre deux points.
 *
 * @param scene La scene conscernée par les modifications
 */
void MouseInterface_segment(Scene* scene,  GdkEventButton* pEvent);


#endif
