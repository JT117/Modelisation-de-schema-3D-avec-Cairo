#include <gtk/gtk.h>
#include <math.h>

#include "Cube.h"
#include "ProjectionTools.h"
#include "Point.h"

Cube* Cube_createCube(tdCoord tCenter, double dHeight,double dWidth, double dDepth)
{
	double dHalfH, dHalfW, dHalfD;
	Cube* pNewCube = NULL;

	if( (pNewCube = (Cube*)malloc(sizeof(Cube))) != NULL )
	{
		dHalfH = dHeight/2;
		dHalfW = dWidth/2;
		dHalfD = dDepth/2;

		/*Sauvegarde des infos sur les points dans notre structure */
		Point_init( &((pNewCube->tPoint)[0]), tCenter[0]-dHalfW, tCenter[1]+dHalfH, tCenter[2]-dHalfD);
		Point_init( &((pNewCube->tPoint)[1]), tCenter[0]+dHalfW, tCenter[1]+dHalfH, tCenter[2]-dHalfD);
		Point_init( &((pNewCube->tPoint)[2]), tCenter[0]+dHalfW, tCenter[1]-dHalfH, tCenter[2]-dHalfD);
		Point_init( &((pNewCube->tPoint)[3]), tCenter[0]-dHalfW, tCenter[1]-dHalfH, tCenter[2]-dHalfD);

		Point_init( &((pNewCube->tPoint)[4]), tCenter[0]-dHalfW, tCenter[1]+dHalfH, tCenter[2]+dHalfD);
		Point_init( &((pNewCube->tPoint)[5]), tCenter[0]+dHalfW, tCenter[1]+dHalfH, tCenter[2]+dHalfD);
		Point_init( &((pNewCube->tPoint)[6]), tCenter[0]+dHalfW, tCenter[1]-dHalfH, tCenter[2]+dHalfD);
		Point_init( &((pNewCube->tPoint)[7]), tCenter[0]-dHalfW, tCenter[1]-dHalfH, tCenter[2]+dHalfD);

		/* Init du centre du repere de la figure (centre de gravité du cube */
		Point_init( &(pNewCube->Center), tCenter[0], tCenter[1], tCenter[2]);

		/*Couleur par défaut, pas de transparence*/
		pNewCube->tColor[0]=0.4;
		pNewCube->tColor[1]=0.4;
		pNewCube->tColor[2]=0.8;

		pNewCube->tColor[3]=0.8;
		pNewCube->estSelectionne = FALSE;
	}
	else
	{
		/* TODO : Implémenter fatalError*/
	}
	return pNewCube;
}

GArray* Cube_facesOrder(Cube* pCube, InfoCamera* pCam)
{
	// sGravCenter -> structure qui va stocker le centre de gravité d'une face
	// sCamPoint -> point représentant les coordonnées du centre du repère de la caméra TODO : un peu la lose de devoir passer par cette structure, voir si on ne peut pas faire autrement
	Point sGravCenter, sCamPoint;
	GArray* gtIndexFaces=NULL; /* Tableau des index des faces à dessiner, c'est cette structure qui sera retournée*/
	GArray* gtDistances=NULL; /*Tableau des distance entre la caméra et le centre de gravité de chaque face, servira pour classer les indexs de face*/
	int iFaceIndex;
	int iLoopInsert = 0;
	int iPoint1, iPoint2; //stockage les index des points du cube pour calcul du centre de gravité
	double dDistance, dDistanceArray ;

	/*Allocation de GArray */
	//gtTabFaceOrder =  g_array_new(FALSE,FALSE,sizeof(int));
	gtIndexFaces = g_array_sized_new(FALSE,TRUE,sizeof(int),6);
	gtDistances = g_array_sized_new(FALSE,TRUE,sizeof(double),6);

	/* Création d'un point ayant pour coordonées le centre du repere de la caméra*/
	Point_init(&sCamPoint, pCam->CoordCam[0], pCam->CoordCam[1], pCam->CoordCam[2]);

	//On passe en revue les faces du cube, pour chaque face on calcul la distance centre de gravite-centre repère caméra
	for(iFaceIndex=1;iFaceIndex<=6;iFaceIndex++)
	{
		//calcul du milieu de la diagonale d'une face afin de trouver le centre de gravité de la face
		switch(iFaceIndex)
		{
			case 1://premiere face points 0-1-2-3, on prend deux points constituant une diagonale
			{
				iPoint1=0; iPoint2=2;
				break;
			}
			case 2:
			{
				iPoint1=1; iPoint2=6;
				break;
			}
			case 3:
			{
				iPoint1=5; iPoint2=7;
				break;
			}
			case 4:
			{
				iPoint1=4; iPoint2=3;
				break;
			}
			case 5:
			{
				iPoint1=0; iPoint2=5;
				break;
			}
			case 6:
			{
				iPoint1=3; iPoint2=6;
			}
		}

		//calcul du milieu du segment reliant les points opposés d'une face
		Point_middlePoint(&sGravCenter, &(pCube->tPoint)[iPoint1], &(pCube->tPoint)[iPoint2]);

		/* calcul de la distance entre le centre de la face et le centre du repere de la caméra */
		dDistance = Point_euclideanDistance(&sGravCenter, &sCamPoint);

		iLoopInsert=0;
		dDistanceArray = g_array_index(gtDistances,double,iLoopInsert);
		/* Insertion de l'index de la face là où il faut*/
		while(dDistanceArray!= 0 && dDistanceArray>dDistance)
		{
			iLoopInsert++;
			dDistanceArray = g_array_index(gtDistances,double,iLoopInsert);
		}

		/*Emplacement d'insertion retrouvé...*/
		g_array_insert_val(gtIndexFaces,iLoopInsert,iFaceIndex);
		g_array_insert_val(gtDistances,iLoopInsert,dDistance);
	}

	g_array_free(gtDistances, TRUE);
	return gtIndexFaces;
}

void Cube_drawCube(Cube* pCube, cairo_t* cr, InfoCamera* pCam)
{
	int iFaceIndex;
	int iFace;
	GArray* gtTabFacesOrder=NULL;
	tdCoord2D* pPointProj0 = NULL;tdCoord2D* pPointProj4 = NULL;
	tdCoord2D* pPointProj1 = NULL;tdCoord2D* pPointProj5 = NULL;
	tdCoord2D* pPointProj2 = NULL;tdCoord2D* pPointProj6 = NULL;
	tdCoord2D* pPointProj3 = NULL;tdCoord2D* pPointProj7 = NULL;

	/* Projection de tous les point du cube */
	pPointProj0 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[2]),pCam);
	pPointProj3 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[3]),pCam);
	pPointProj4 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[4]),pCam);
	pPointProj5 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[5]),pCam);
	pPointProj6 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[6]),pCam);
	pPointProj7 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[7]),pCam);

	/* Recherche de l'ordre dans lequel on doit dessiner les faces */
	gtTabFacesOrder = Cube_facesOrder(pCube,pCam);

	/* Dessin face par face dans l'ordre*/
	for(iFaceIndex=0;iFaceIndex<6;iFaceIndex++)
	{
		iFace = g_array_index(gtTabFacesOrder,int,iFaceIndex); /* Recup de la face à dessiner*/

		/* Adpatation des points à dessiner en fonction de la face */
		switch(iFace)
		{
			case 1:
			{
				cairo_move_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
				cairo_line_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
				cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
				cairo_line_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
				cairo_line_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);

				break;
			}
			case 2:
			{
				cairo_move_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
				cairo_line_to( cr, (*pPointProj5)[0], (*pPointProj5)[1]);
				cairo_line_to( cr, (*pPointProj6)[0], (*pPointProj6)[1]);
				cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
				cairo_line_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);

				break;
			}
			case 3:
			{
				cairo_move_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
				cairo_line_to( cr, (*pPointProj5)[0], (*pPointProj5)[1]);
				cairo_line_to( cr, (*pPointProj6)[0], (*pPointProj6)[1]);
				cairo_line_to( cr, (*pPointProj7)[0], (*pPointProj7)[1]);
				cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);

				break;
			}
			case 4:
			{
				cairo_move_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
				cairo_line_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
				cairo_line_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
				cairo_line_to( cr, (*pPointProj7)[0], (*pPointProj7)[1]);
				cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
				break;
			}
			case 5:
			{
				cairo_move_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
				cairo_line_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
				cairo_line_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
				cairo_line_to( cr, (*pPointProj5)[0], (*pPointProj5)[1]);
				cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
				break;
			}
			case 6:
			{
				cairo_move_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
				cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
				cairo_line_to( cr, (*pPointProj6)[0], (*pPointProj6)[1]);
				cairo_line_to( cr, (*pPointProj7)[0], (*pPointProj7)[1]);
				cairo_line_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);

			}
		}
		cairo_set_source_rgba (cr, pCube->tColor[0], pCube->tColor[1], pCube->tColor[2] , pCube->tColor[3]); /*Couleur */
		cairo_fill_preserve( cr );/*remplissage du rectangle avec path preservé*/
		cairo_set_line_width(cr,0.8);/* réglage taille de la ligne*/

		if(pCube->estSelectionne == TRUE)
			cairo_set_source_rgb ( cr, 1.0, 0, 0); /* couleur contour */
		else
			cairo_set_source_rgb ( cr, 0, 0, 0); /* couleur contour */

		cairo_stroke(cr); /* dessin contour, perte du path */
	}

	/* Libération mémoire */
	g_array_free(gtTabFacesOrder, TRUE);
	free(pPointProj0);	free(pPointProj4);
	free(pPointProj1);	free(pPointProj5);
	free(pPointProj2);	free(pPointProj6);
	free(pPointProj3);	free(pPointProj7);
}

void Cube_rotateCube(Cube* pCube, double dAngleX, double dAngleY, double dAngleZ)
{
	int iLoop;
	tdMatrix tdMatTransfo, tdMatPassRepObj; /* MAtrice de rotation, matrice de passage dans le repère objet*/
	tdCoord tdCoordRepObj, tdCoordApTransfo;

	/* Initialisation des coordonées*/
	Point_initCoord(tdCoordRepObj, 0.0, 0.0, 0.0);
	Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);

	/* Construction de la matrice de passage World -> Repere objet */
	Matrix_initMatrix(tdMatPassRepObj); /* initialisation de la matrice de passage dans repere objet*/
	tdMatPassRepObj[0][0] = 1;
	tdMatPassRepObj[1][1] = 1;
	tdMatPassRepObj[2][2] = 1;
	tdMatPassRepObj[3][3] = 1;

	tdMatPassRepObj[0][3] = -pCube->Center.tdCoordWorld[0];
	tdMatPassRepObj[1][3] = -pCube->Center.tdCoordWorld[1];
	tdMatPassRepObj[2][3] = -pCube->Center.tdCoordWorld[2];

	if(dAngleX != 0)
	{
		/*Récupération de la matrice de rotation qui va bien */
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleX, axeX);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<8 ;iLoop++)
		{
			/* Tout d'abord recherche des coordonnées dans le repere de l'objet*/
			Matrix_multiMatrixVect(tdMatPassRepObj, pCube->tPoint[iLoop].tdCoordWorld, tdCoordRepObj);
			/*Puis transformation, toujours dans le repere objet*/
			Matrix_multiMatrixVect(tdMatTransfo, tdCoordRepObj, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pCube->tPoint[iLoop].tdCoordWorld[0] = (double)round(pCube->tPoint[iLoop].tdCoordWorld[0]
			               																+ (tdCoordApTransfo[0]-tdCoordRepObj[0])) ;
			pCube->tPoint[iLoop].tdCoordWorld[1] = (double)round(pCube->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]));
			pCube->tPoint[iLoop].tdCoordWorld[2] = (double)round(pCube->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]));

			/* on réinitialise les vecteurs contenant les infos sur la transformation */
			Point_initCoord(tdCoordRepObj, 0.0, 0.0, 0.0);
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}
	}

	if(dAngleY != 0)
	{
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleY, axeY);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<8 ;iLoop++)
		{
			/* Tout d'abord recherche des coordonnées dans le repere de l'objet*/
			Matrix_multiMatrixVect(tdMatPassRepObj, pCube->tPoint[iLoop].tdCoordWorld, tdCoordRepObj);
			/*Puis transformation, toujours dans le repere objet*/
			Matrix_multiMatrixVect(tdMatTransfo, tdCoordRepObj, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pCube->tPoint[iLoop].tdCoordWorld[0] = (double)round(pCube->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]));
			pCube->tPoint[iLoop].tdCoordWorld[1] = (double)round(pCube->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]));
			pCube->tPoint[iLoop].tdCoordWorld[2] = (double)round(pCube->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]));

			Point_initCoord(tdCoordRepObj, 0.0, 0.0, 0.0);
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}

	}

	if(dAngleZ != 0)
	{
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleZ, axeZ);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<8 ;iLoop++)
		{
			/* Tout d'abord recherche des coordonnées dans le repere de l'objet*/
			Matrix_multiMatrixVect(tdMatPassRepObj, pCube->tPoint[iLoop].tdCoordWorld, tdCoordRepObj);
			/*Puis transformation, toujours dans le repere objet*/
			Matrix_multiMatrixVect(tdMatTransfo, tdCoordRepObj, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pCube->tPoint[iLoop].tdCoordWorld[0] = (double)round(pCube->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]));
			pCube->tPoint[iLoop].tdCoordWorld[1] = (double)round(pCube->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]));
			pCube->tPoint[iLoop].tdCoordWorld[2] = (double)round(pCube->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]));

			Point_initCoord(tdCoordRepObj, 0.0, 0.0, 0.0);
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}
	}
}

void Cube_rotateCubeWorld(Cube* pCube, double dAngleX, double dAngleY, double dAngleZ)
{
	int iLoop;
	tdMatrix tdMatTransfo;
	tdCoord tdCoordApTransfo;

	Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);

	if(dAngleX != 0)
	{
		/*Récupération de la matrice de rotation qui va bien */
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleX, axeX);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<8 ;iLoop++)
		{
			/*Transformation dans le repere du monde*/
			Matrix_multiMatrixVect(tdMatTransfo, pCube->tPoint[iLoop].tdCoordWorld, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pCube->tPoint[iLoop].tdCoordWorld[0] = tdCoordApTransfo[0];
			pCube->tPoint[iLoop].tdCoordWorld[1] = tdCoordApTransfo[1];
			pCube->tPoint[iLoop].tdCoordWorld[2] = tdCoordApTransfo[2];

			/* réinitialisation coord après transformation pour le  point suivant */
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}
	}

	if(dAngleY != 0)
	{
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleY, axeY);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<8 ;iLoop++)
		{
			/*Transformation dans le repere du monde*/
			Matrix_multiMatrixVect(tdMatTransfo, pCube->tPoint[iLoop].tdCoordWorld, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pCube->tPoint[iLoop].tdCoordWorld[0] = tdCoordApTransfo[0];
			pCube->tPoint[iLoop].tdCoordWorld[1] = tdCoordApTransfo[1];
			pCube->tPoint[iLoop].tdCoordWorld[2] = tdCoordApTransfo[2];

			/* réinitialisation coord après transformation pour le  point suivant */
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}

	}

	if(dAngleZ != 0)
	{
		TransfoTools_getMatrixRotation(tdMatTransfo, dAngleZ, axeZ);

		/* On effectue la transformation pour tous  les points du rectangle */
		for(iLoop=0 ; iLoop<8 ;iLoop++)
		{
			/*Transformation dans le repere du monde*/
			Matrix_multiMatrixVect(tdMatTransfo, pCube->tPoint[iLoop].tdCoordWorld, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pCube->tPoint[iLoop].tdCoordWorld[0] = tdCoordApTransfo[0];
			pCube->tPoint[iLoop].tdCoordWorld[1] = tdCoordApTransfo[1];
			pCube->tPoint[iLoop].tdCoordWorld[2] = tdCoordApTransfo[2];

			/* réinitialisation coord après transformation pour le  point suivant */
			Point_initCoord(tdCoordApTransfo, 0.0, 0.0, 0.0);
		}
	}
}

void Cube_modSize(Cube* pCube, double dRatio)
{
	int iLoop;
	tdMatrix tdMatTransfo, tdMatPassRepObj;
	tdCoord tdCoordRepObj, tdCoordApTransfo;

	/* Initialisation des coordonées*/
	Point_initCoord(tdCoordRepObj, 0, 0, 0);
	Point_initCoord(tdCoordApTransfo, 0, 0, 0);

	/* Construction de la matrice de passage World -> Repere objet */
	Matrix_initMatrix(tdMatPassRepObj); /* initialisation de la matrice de passage dans repere objet*/
	tdMatPassRepObj[0][3] = -pCube->Center.tdCoordWorld[0];
	tdMatPassRepObj[1][3] = -pCube->Center.tdCoordWorld[1];
	tdMatPassRepObj[2][3] = -pCube->Center.tdCoordWorld[2];

	if(dRatio != 1)
	{
		/*Récupération de la matrice d'homothétie*/
		TransfoTools_getMatrixHomothety(tdMatTransfo, dRatio);

		/* On effectue la transformation pour tous  les points du Cube */
		for(iLoop=0 ; iLoop<8 ;iLoop++)
		{
			/* Tout d'abord recherche des coordonnées dans le repere de l'objet*/
			Matrix_multiMatrixVect(tdMatPassRepObj, pCube->tPoint[iLoop].tdCoordWorld, tdCoordRepObj);
			/*Puis transformation, toujours dans le repere objet*/
			Matrix_multiMatrixVect(tdMatTransfo, tdCoordRepObj, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pCube->tPoint[iLoop].tdCoordWorld[0] = pCube->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]);
			pCube->tPoint[iLoop].tdCoordWorld[1] = pCube->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]);
			pCube->tPoint[iLoop].tdCoordWorld[2] = pCube->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]);

			/* on réinitialise les vecteurs contenant les infos sur la transformation */
			Point_initCoord(tdCoordRepObj, 0, 0, 0);
			Point_initCoord(tdCoordApTransfo, 0, 0, 0);
		}
	}
}

gboolean Cube_inFace(tdCoord2D tP1,tdCoord2D tP2,tdCoord2D tP3, tdCoord2D tP4, double dXClick, double dYClick )
{
	int iNb = 0, iLoop = 0;
	double tDistanceClick[2]; /* Distance (sur x et y) entre la position du curseur et chaque point  */
	double tDistancePoints[2]; /* Distance entre deux points d'une arrête*/
	double dDet = 0;
	tdCoord2D tCoordClick;

	Point_initCoord2D(tCoordClick, dXClick, dYClick); /* Coordonnées du clique */

	/* On passe chaque arrête en revue */
	for(iLoop=0; iLoop<4; iLoop++)
	{
		switch(iLoop)
		{
			case 0:
			{
				tDistanceClick[0] =tCoordClick[0]-tP1[0];
				tDistanceClick[1] = tCoordClick[1]-tP1[1];
				tDistancePoints[0] = tP2[0]-tP1[0];
				tDistancePoints[1] = tP2[1]-tP1[1];
				break;
			}
			case 1:
			{
				tDistanceClick[0] = tCoordClick[0]-tP2[0];
				tDistanceClick[1] = tCoordClick[1]-tP2[1];
				tDistancePoints[0] = tP3[0]-tP2[0];
				tDistancePoints[1] = tP3[1]-tP2[1];
				break;
			}
			case 2:
			{
				tDistanceClick[0] = tCoordClick[0]-tP3[0];
				tDistanceClick[1] = tCoordClick[1]-tP3[1];
				tDistancePoints[0] = tP4[0]-tP3[0];
				tDistancePoints[1] = tP4[1]-tP3[1];
				break;
			}
			case 3:
			{
				tDistanceClick[0] = tCoordClick[0]-tP4[0];
				tDistanceClick[1] = tCoordClick[1]-tP4[1];
				tDistancePoints[0] = tP1[0]-tP4[0];
				tDistancePoints[1] = tP1[1]-tP4[1];
				break;
			}
		}
		dDet = Point_determinant(tDistancePoints,tDistanceClick );

		if( dDet > 0)
				iNb++;
		else if( dDet < 0)
				iNb--;
	}

    if( iNb == 4 || iNb == -4 )
        return TRUE;
    else
    	return FALSE;
}

gboolean Cube_Contient_Point( Cube* pCube, double x, double y, InfoCamera* pCam)
{
    gboolean est_contenu = FALSE;
	tdCoord2D* pPointProj0 = NULL;tdCoord2D* pPointProj4 = NULL;
	tdCoord2D* pPointProj1 = NULL;tdCoord2D* pPointProj5 = NULL;
	tdCoord2D* pPointProj2 = NULL;tdCoord2D* pPointProj6 = NULL;
	tdCoord2D* pPointProj3 = NULL;tdCoord2D* pPointProj7 = NULL;

	/* Projection de tous les point du cube */
	pPointProj0 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[2]),pCam);
	pPointProj3 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[3]),pCam);
	pPointProj4 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[4]),pCam);
	pPointProj5 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[5]),pCam);
	pPointProj6 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[6]),pCam);
	pPointProj7 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[7]),pCam);

    est_contenu = est_contenu || Cube_inFace( (*pPointProj0), (*pPointProj1), (*pPointProj2), (*pPointProj3), x, y )
                              || Cube_inFace( (*pPointProj1), (*pPointProj5), (*pPointProj6), (*pPointProj2), x, y )
                              || Cube_inFace( (*pPointProj4), (*pPointProj5), (*pPointProj6), (*pPointProj7), x, y )
                              || Cube_inFace( (*pPointProj0), (*pPointProj4), (*pPointProj7), (*pPointProj3), x, y )
                              || Cube_inFace( (*pPointProj0), (*pPointProj1), (*pPointProj5), (*pPointProj4), x, y )
                              || Cube_inFace( (*pPointProj3), (*pPointProj2), (*pPointProj6), (*pPointProj7), x, y );

    free(pPointProj0);	free(pPointProj4);
	free(pPointProj1);	free(pPointProj5);
	free(pPointProj2);	free(pPointProj6);
	free(pPointProj3);	free(pPointProj7);
    return est_contenu;
}
