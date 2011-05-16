#include <gtk/gtk.h>
#include "Cube.h"
#include "ProjectionTools.h"
#include <math.h>
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
	GArray* gtTabFaceOrder =NULL;
	int iFaceIndex;
	int iPoint1, iPoint2; //stockage les index des points du cube pour calcul du centre de gravité
	double dPrevDistance, dDistance;

	/*Allocation de GArray */
	gtTabFaceOrder =  g_array_new(FALSE,FALSE,sizeof(int));

	//On passe en revue les faces du cube, pour chaque face on calcul la distance centre de gravite-centre repère caméra
	for(iFaceIndex=1;iFaceIndex<=6;iFaceIndex++)
	{
		//calcul du milieu de la diagonale d'une face afin de trouver le centre de gravité de la face
		switch(iFaceIndex)
		{
			case 1://premiere face points 0-1-2-3
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
		Point_init(&sCamPoint, pCam->CoordCam[0], pCam->CoordCam[1], pCam->CoordCam[2]);

		/* calcul de la distance entre le centre de la face et le centre du repere de la caméra */
		dDistance = Point_euclideanDistance(&sGravCenter, &sCamPoint);

		if(iFaceIndex==1) /* premiere face */
		{
			g_array_append_val(gtTabFaceOrder,iFaceIndex);
			dPrevDistance = dDistance;
		}
		else
		{
			if(dDistance > dPrevDistance)
			{
				g_array_prepend_val(gtTabFaceOrder, iFaceIndex);
				dPrevDistance = dDistance;
			}
			else
			{
				g_array_append_val(gtTabFaceOrder, iFaceIndex);
				dPrevDistance = dDistance;
			}
		}
	}
	return gtTabFaceOrder;
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
	for(iFaceIndex=0;iFaceIndex<7;iFaceIndex++)
	{
		iFace = g_array_index(gtTabFacesOrder,int,iFaceIndex); /* Recup de la face à récupérer */

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
		if( pCube->estSelectionne )
		{
		    cairo_set_source_rgb ( cr, 0.9, 0, 0);
		}
		else
		{
            cairo_set_source_rgb ( cr, 0, 0, 0); /* couleur contour */
		}
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
			pCube->tPoint[iLoop].tdCoordWorld[0] = pCube->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]);
			pCube->tPoint[iLoop].tdCoordWorld[1] = pCube->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]);
			pCube->tPoint[iLoop].tdCoordWorld[2] = pCube->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]);

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
			pCube->tPoint[iLoop].tdCoordWorld[0] = pCube->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]);
			pCube->tPoint[iLoop].tdCoordWorld[1] = pCube->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]);
			pCube->tPoint[iLoop].tdCoordWorld[2] = pCube->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]);

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
			pCube->tPoint[iLoop].tdCoordWorld[0] = pCube->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]);
			pCube->tPoint[iLoop].tdCoordWorld[1] = pCube->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]);
			pCube->tPoint[iLoop].tdCoordWorld[2] = pCube->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]);

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

gboolean Cube_Contient_Point( Cube* pCube, double x, double y, InfoCamera* pCam )
{
    gboolean est_contenu = FALSE;

    tdCoord2D* pPointProj0 = NULL;tdCoord2D* pPointProj4 = NULL;
	tdCoord2D* pPointProj1 = NULL;tdCoord2D* pPointProj5 = NULL;
	tdCoord2D* pPointProj2 = NULL;tdCoord2D* pPointProj6 = NULL;
	tdCoord2D* pPointProj3 = NULL;tdCoord2D* pPointProj7 = NULL;

    pPointProj0 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[2]),pCam);
	pPointProj3 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[3]),pCam);
	pPointProj4 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[4]),pCam);
	pPointProj5 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[5]),pCam);
	pPointProj6 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[6]),pCam);
	pPointProj7 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[7]),pCam);

    est_contenu = est_contenu || est_dans_face( pPointProj0, pPointProj1, pPointProj2, pPointProj3, x, y )
                              || est_dans_face( pPointProj1, pPointProj5, pPointProj6, pPointProj2, x, y )
                              || est_dans_face( pPointProj4, pPointProj5, pPointProj6, pPointProj7, x, y )
                              || est_dans_face( pPointProj0, pPointProj4, pPointProj7, pPointProj3, x, y )
                              || est_dans_face( pPointProj0, pPointProj1, pPointProj5, pPointProj4, x, y )
                              || est_dans_face( pPointProj3, pPointProj2, pPointProj6, pPointProj7, x, y );

    free(pPointProj0);	free(pPointProj4);
	free(pPointProj1);	free(pPointProj5);
	free(pPointProj2);	free(pPointProj6);
	free(pPointProj3);	free(pPointProj7);

    return est_contenu;
}

gboolean est_dans_face( tdCoord2D* a, tdCoord2D* b, tdCoord2D* c, tdCoord2D* d, double x, double y )
{
    int nb = 0;

    nb += scalaire_result( a, b, x, y );
    nb += scalaire_result( b, c, x, y );
    nb += scalaire_result( c, d, x, y );
    nb += scalaire_result( d, a, x, y );

    if( nb == 4 || nb == -4 )
    {
        return TRUE;
    }
    else return FALSE;
}

int scalaire_result( tdCoord2D* a, tdCoord2D* b, int x, int y )
{
    Point ab;
    Point ap;
    double produitScalaire = 0.0;

    ab.x = (*b)[0] - (*a)[0];
    ab.y = (*b)[1] - (*b)[1];

    ap.x = x - (*a)[0];
    ap.y = y - (*a)[1];

    produitScalaire = ab.x * ap.x + ab.y * ap.y;

    if( produitScalaire > 0 )
    {
        return 1;
    }
    else if( produitScalaire < 0 )
    {
        return -1;
    }
    else return 0;
}
