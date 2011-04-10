#ifndef PROJECTIONTOOLS_H
#define PROJECTIONTOOLS_H

#include "Camera.h"
#include "Point.h"

/**
 * Permet d'obtenir les coordonnées du point passé en paramètre dans le repère de l'image.
 *
 * @param pPoint pointeur vers la structure point à projetter
 * @param pCam Infos sur caméra
 */
tdCoord2D* ProjectionTools_getPictureCoord(sPoint* pPoint, sInfoCamera* pCam);

#endif
