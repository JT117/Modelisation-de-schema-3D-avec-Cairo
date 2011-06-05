#include <gtk/gtk.h>
#include <math.h>

#include "Cube.h"
#include "ProjectionTools.h"
#include "Point.h"
#include "Objet.h"

Cube* Cube_createCube(tCoord tCenter, double dHeight,double dWidth, double dDepth)
{
	double dHalfH, dHalfW, dHalfD;
	Cube* pNewCube = NULL;

	if( (pNewCube = (Cube*)malloc(sizeof(Cube))) != NULL )
	{
		dHalfH = dHeight/2;
		dHalfW = dWidth/2;
		dHalfD = dDepth/2;

		/*Sauvegarde des infos sur les points dans notre structure */
		Point_initGroup( &((pNewCube->tPoint)[0]), -dHalfW, dHalfH, -dHalfD);
		Point_initGroup( &((pNewCube->tPoint)[1]), dHalfW, dHalfH, -dHalfD);
		Point_initGroup( &((pNewCube->tPoint)[2]), dHalfW, -dHalfH, -dHalfD);
		Point_initGroup( &((pNewCube->tPoint)[3]), -dHalfW, -dHalfH, -dHalfD);

		Point_initGroup( &((pNewCube->tPoint)[4]), -dHalfW, dHalfH, dHalfD);
		Point_initGroup( &((pNewCube->tPoint)[5]), dHalfW, dHalfH, dHalfD);
		Point_initGroup( &((pNewCube->tPoint)[6]), dHalfW, -dHalfH, dHalfD);
		Point_initGroup( &((pNewCube->tPoint)[7]), -dHalfW, -dHalfH, dHalfD);

		/* Init du centre du repere de la figure (centre de gravité du cube */
		Point_initGroup( &(pNewCube->Center), tCenter[0], tCenter[1], tCenter[2]);

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
	int iFaceIndex = 0;
	int iLoopInsert = 0;
	int iPoint1, iPoint2; //stockage les index des points du cube pour calcul du centre de gravité
	double dDistance, dDistanceArray=0.0 ;

	/*Allocation de GArray */
	gtIndexFaces = g_array_sized_new(FALSE,TRUE,sizeof(int),6);
	gtDistances = g_array_sized_new(FALSE,TRUE,sizeof(double),6);

	/* Initialisation du tableau des distances */
	for(iLoopInsert=0;iLoopInsert<=5;iLoopInsert++)
	{
		g_array_insert_val(gtIndexFaces,iLoopInsert,iFaceIndex);
		g_array_insert_val(gtDistances,iLoopInsert,dDistanceArray);
	}

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

void Cube_updateCoordWolrd(Objet* pObj)
{
	int i,j;
	tdMatrix tdMatPass;
	tCoord tdCoordBefore;
	tCoord tdCoordAfter; /* Va contenir les coordonnées de points màj après chaque itération */

	/* On cherche à exprimer l'ensemble des coordonnées de points dans notre repere de la caméra --> pour projection */
	Groupe* pFatherGroup = NULL;
	Cube* pCube = pObj->type.cube; /* Récupération du pointeur sur notre objet d'un type plus précis que Objet* */

	for(i=0;i<8;i++ )  /* On passe tous les points de l'objet en revue */
	{
		pFatherGroup = pObj->pFatherGroup; /* on récupère un pointeur vers le groupe pere */
		for(j=0;j<4;j++)  /* initialisation du tableau de coordonnées avant tout changement de base */
			tdCoordBefore[j] = pCube->tPoint[i].tdCoordGroup[j];

		/* PREMIER CHANGEMENT DE BASE = PASSAGE REPERE OBJET --> GROUPE PERE */
		/* Passage des coordonnées du point dans le premier groupe pere */
		Matrix_initIdentityMatrix(tdMatPass); /* Initialisation de la matrice pour construction d'un matrice de passage */
		/* COnstruction de la nouvelle matrice de passage grâce aux coordonnées du repere objet dans son groupe pere*/
		tdMatPass[0][3] = pCube->Center.tdCoordGroup[0];
		tdMatPass[1][3] = pCube->Center.tdCoordGroup[1];
		tdMatPass[2][3] = pCube->Center.tdCoordGroup[2];

		Matrix_multiMatrixVect(tdMatPass, tdCoordBefore, tdCoordAfter); /* tdCoordAfter contient les coordonnées du point après le premier changement de base*/
		ProjectionTools_getCoordWorld(tdCoordAfter,pFatherGroup,&(pCube->tPoint[i]));
	}

	pFatherGroup = pObj->pFatherGroup;
	/* On met aussi à jour les coordonnées du centre de l'objet */
	ProjectionTools_getCoordWorld(pCube->Center.tdCoordGroup,pFatherGroup,&(pCube->Center));
}

void Cube_drawCube( Objet* pObj,cairo_t* cr,InfoCamera* pCam)
{
	GArray* gtTabFacesOrder=NULL; /* Tableau indiquant dans quel ordre il faut dessiner les faces du cube */
	/* Coordonnées de points une fois projettés */
	tCoord2D* pPointProj0 = NULL;tCoord2D* pPointProj4 = NULL;
	tCoord2D* pPointProj1 = NULL;tCoord2D* pPointProj5 = NULL;
	tCoord2D* pPointProj2 = NULL;tCoord2D* pPointProj6 = NULL;
	tCoord2D* pPointProj3 = NULL;tCoord2D* pPointProj7 = NULL;
	int i;
	Cube* pCube = pObj->type.cube;

	Cube_updateCoordWolrd(pObj);

	/* Projection de tous les point du cube */
	pPointProj0 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[2]),pCam);
	pPointProj3 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[3]),pCam);
	pPointProj4 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[4]),pCam);
	pPointProj5 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[5]),pCam);
	pPointProj6 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[6]),pCam);
	pPointProj7 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[7]),pCam);

	/* Mise à jour des coordonnées un fois projettées */
	Point_initCoord2D(pCube->tPoint[0].tCoordProjection,(*pPointProj0)[0], (*pPointProj0)[1]);
	Point_initCoord2D(pCube->tPoint[1].tCoordProjection,(*pPointProj1)[0], (*pPointProj1)[1]);
	Point_initCoord2D(pCube->tPoint[2].tCoordProjection,(*pPointProj2)[0], (*pPointProj2)[1]);
	Point_initCoord2D(pCube->tPoint[3].tCoordProjection,(*pPointProj3)[0], (*pPointProj3)[1]);
	Point_initCoord2D(pCube->tPoint[4].tCoordProjection,(*pPointProj4)[0], (*pPointProj4)[1]);
	Point_initCoord2D(pCube->tPoint[5].tCoordProjection,(*pPointProj5)[0], (*pPointProj5)[1]);
	Point_initCoord2D(pCube->tPoint[6].tCoordProjection,(*pPointProj6)[0], (*pPointProj6)[1]);
	Point_initCoord2D(pCube->tPoint[7].tCoordProjection,(*pPointProj7)[0], (*pPointProj7)[1]);

	/* Recherche de l'ordre dans lequel on doit dessiner les faces */
	gtTabFacesOrder = Cube_facesOrder(pCube,pCam);
	int iFaceIndex = 0;
	int iFace =0;
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
				cairo_close_path(cr);
				//cairo_line_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);

				break;
			}
			case 2:
			{
				cairo_move_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
				cairo_line_to( cr, (*pPointProj5)[0], (*pPointProj5)[1]);
				cairo_line_to( cr, (*pPointProj6)[0], (*pPointProj6)[1]);
				cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
				cairo_close_path(cr);
				//cairo_line_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);

				break;
			}
			case 3:
			{
				cairo_move_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
				cairo_line_to( cr, (*pPointProj5)[0], (*pPointProj5)[1]);
				cairo_line_to( cr, (*pPointProj6)[0], (*pPointProj6)[1]);
				cairo_line_to( cr, (*pPointProj7)[0], (*pPointProj7)[1]);
				cairo_close_path(cr);
				//cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);

				break;
			}
			case 4:
			{
				cairo_move_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
				cairo_line_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
				cairo_line_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
				cairo_line_to( cr, (*pPointProj7)[0], (*pPointProj7)[1]);
				cairo_close_path(cr);
				//cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
				break;
			}
			case 5:
			{
				cairo_move_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
				cairo_line_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
				cairo_line_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
				cairo_line_to( cr, (*pPointProj5)[0], (*pPointProj5)[1]);
				cairo_close_path(cr);
				//cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
				break;
			}
			case 6:
			{
				cairo_move_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
				cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
				cairo_line_to( cr, (*pPointProj6)[0], (*pPointProj6)[1]);
				cairo_line_to( cr, (*pPointProj7)[0], (*pPointProj7)[1]);
				cairo_close_path(cr);
				//cairo_line_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);

			}
		}
		cairo_set_source_rgba (cr, pCube->tColor[0], pCube->tColor[1], pCube->tColor[2] , pCube->tColor[3]); /*Couleur */
		cairo_fill_preserve( cr );/*remplissage du rectangle avec path preservé*/
		cairo_set_line_width(cr,0.8);/* réglage taille de la ligne*/

		if(pCube->estSelectionne == TRUE)  /* réglage de la couleur du contour*/
			cairo_set_source_rgb ( cr, 1.0, 0, 0);
		else
			cairo_set_source_rgb ( cr, 0, 0, 0);

		cairo_stroke(cr); /* dessin contour, perte du path */
	}

	/* On dessine les points d'intérêts si il le faut*/
	if( pObj->iSelectedForSegment == -1 )
	{
		for(i=0;i<8;i++)
		{
			cairo_arc(cr, pCube->tPoint[i].tCoordProjection[0], pCube->tPoint[i].tCoordProjection[1], 5, 0.0, 2*M_PI);
			cairo_set_source_rgba (cr,1.0, 0.0, 0.0 , 1.0); /*Couleur */
			cairo_stroke(cr);
		}
	}
	else if ( pObj->iSelectedForSegment >= 0)
	{  /* On dessine le point selectionné rempli en rouge */
		cairo_arc(cr, pCube->tPoint[pObj->iSelectedForSegment].tCoordProjection[0], pCube->tPoint[pObj->iSelectedForSegment].tCoordProjection[1], 3, 0.0, 2*M_PI);
		cairo_set_source_rgba (cr,1.0, 0.0, 0.0 , 1.0); /*Couleur */
		cairo_fill(cr);
	}

	/* Libération mémoire */
	g_array_free(gtTabFacesOrder, TRUE);
	free(pPointProj0);	free(pPointProj4);
	free(pPointProj1);	free(pPointProj5);
	free(pPointProj2);	free(pPointProj6);
	free(pPointProj3);	free(pPointProj7);
}


void Cube_modSize(Cube* pCube, double dRatio)
{
	int iLoop;
	tdMatrix tdMatTransfo, tdMatPassRepObj;
	tCoord tdCoordRepObj, tdCoordApTransfo;

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
		Transformation_getMatrixHomothety(tdMatTransfo, dRatio);

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

gboolean Cube_Contient_Point( Cube* pCube, double x, double y, InfoCamera* pCam)
{
    gboolean est_contenu = FALSE;
    tCoord2D* tCoordTab[4]; /* Tableauq qui va contenir les points relatifs à une face donnée */
	tCoord2D* pPointProj0 = NULL;tCoord2D* pPointProj4 = NULL;
	tCoord2D* pPointProj1 = NULL;tCoord2D* pPointProj5 = NULL;
	tCoord2D* pPointProj2 = NULL;tCoord2D* pPointProj6 = NULL;
	tCoord2D* pPointProj3 = NULL;tCoord2D* pPointProj7 = NULL;

	/* Projection de tous les point du cube */
	pPointProj0 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[2]),pCam);
	pPointProj3 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[3]),pCam);
	pPointProj4 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[4]),pCam);
	pPointProj5 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[5]),pCam);
	pPointProj6 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[6]),pCam);
	pPointProj7 = ProjectionTools_getPictureCoord(&((pCube->tPoint)[7]),pCam);

	tCoordTab[0] = pPointProj0;
	tCoordTab[1] = pPointProj1;
	tCoordTab[2] = pPointProj2;
	tCoordTab[3] = pPointProj3;
	est_contenu = est_contenu || Selection_inFace(tCoordTab,4,x,y);
	tCoordTab[0] = pPointProj1;
	tCoordTab[1] = pPointProj5;
	tCoordTab[2] = pPointProj6;
	tCoordTab[3] = pPointProj2;
	est_contenu = est_contenu || Selection_inFace(tCoordTab,4,x,y);
	tCoordTab[0] = pPointProj4;
	tCoordTab[1] = pPointProj5;
	tCoordTab[2] = pPointProj6;
	tCoordTab[3] = pPointProj7;
	est_contenu = est_contenu || Selection_inFace(tCoordTab,4,x,y);
	tCoordTab[0] = pPointProj0;
	tCoordTab[1] = pPointProj4;
	tCoordTab[2] = pPointProj7;
	tCoordTab[3] = pPointProj3;
	est_contenu = est_contenu || Selection_inFace(tCoordTab,4,x,y);
	tCoordTab[0] = pPointProj0;
	tCoordTab[1] = pPointProj1;
	tCoordTab[2] = pPointProj5;
	tCoordTab[3] = pPointProj4;
	est_contenu = est_contenu || Selection_inFace(tCoordTab,4,x,y);
	tCoordTab[0] = pPointProj3;
	tCoordTab[1] = pPointProj2;
	tCoordTab[2] = pPointProj6;
	tCoordTab[3] = pPointProj7;
	est_contenu = est_contenu || Selection_inFace(tCoordTab,4,x,y);

    free(pPointProj0);	free(pPointProj4);
	free(pPointProj1);	free(pPointProj5);
	free(pPointProj2);	free(pPointProj6);
	free(pPointProj3);	free(pPointProj7);
    return est_contenu;
}

void Cube_transfoCenter(Cube* pCube, tdMatrix tdTransfoMat)
{
	tCoord tCoordApTransfo;

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);
	/*APplication de la transformation au centre du rectangle */
	Matrix_multiMatrixVect(tdTransfoMat, pCube->Center.tdCoordGroup, tCoordApTransfo);

	pCube->Center.tdCoordGroup[0] = tCoordApTransfo[0];
	pCube->Center.tdCoordGroup[1] = tCoordApTransfo[1];
	pCube->Center.tdCoordGroup[2] = tCoordApTransfo[2];
	pCube->Center.tdCoordGroup[3] = tCoordApTransfo[3];
}

void Cube_transfo(Cube* pCube, tdMatrix tdTransfoMat)
{
	tCoord tCoordApTransfo;
	int i;

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);
	for(i=0 ; i<8 ;i++)
	{
		Matrix_multiMatrixVect(tdTransfoMat, pCube->tPoint[i].tdCoordGroup, tCoordApTransfo);

		/* Modification des coordonées dans le repere objet */
		pCube->tPoint[i].tdCoordGroup[0] = tCoordApTransfo[0];
		pCube->tPoint[i].tdCoordGroup[1] = tCoordApTransfo[1];
		pCube->tPoint[i].tdCoordGroup[2] = tCoordApTransfo[2];
		pCube->tPoint[i].tdCoordGroup[3] = tCoordApTransfo[3];

		Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);  /* Reinitialisation de la matrice de coordonnées après transformation*/
	}
}

