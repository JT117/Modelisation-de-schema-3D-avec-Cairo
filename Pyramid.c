#include <gtk/gtk.h>
#include <math.h>

#include "Pyramid.h"
#include "ProjectionTools.h"
#include "Point.h"
#include "Objet.h"

Pyramid* Pyramid_createSquarePyramid(tCoord tCenter, tCoord2D tCoord1,tCoord2D tCoord2,tCoord2D tCoord3,tCoord2D tCoord4, double dHeight )
{
	Pyramid* pNewPyramid = NULL;

	if( (pNewPyramid = (Pyramid*)malloc(sizeof(Pyramid))) != NULL )
	{
		/* COnstruction de la base */
		Point_initGroup( &((pNewPyramid->tPoint)[1]), tCoord1[0], -dHeight/2, tCoord1[1]);
		Point_initGroup( &((pNewPyramid->tPoint)[2]), tCoord2[0], -dHeight/2, tCoord2[1]);
		Point_initGroup( &((pNewPyramid->tPoint)[3]), tCoord3[0], -dHeight/2, tCoord3[1]);
		Point_initGroup( &((pNewPyramid->tPoint)[4]), tCoord4[0], -dHeight/2, tCoord4[0]);

		/* Ajout du sommet de la pyramide*/
		Point_initGroup( &((pNewPyramid->tPoint)[0]), 0, dHeight/2, 0);

		/* Init du centre du repere de la figure (centre de gravité du Pyramid */
		Point_initGroup( &(pNewPyramid->Center), tCenter[0], tCenter[1], tCenter[2]);

		pNewPyramid->eType = SQUARE;
		pNewPyramid->estSelectionne = FALSE;
	}
	else
	{
		/* TODO : Implémenter fatalError*/
	}
	return pNewPyramid;
}

Pyramid* Pyramid_createTriangularPyramid(tCoord tCenter, tCoord2D tCoord1,tCoord2D tCoord2,tCoord2D tCoord3, double dHeight )
{
	Pyramid* pNewPyramid = NULL;

	if( (pNewPyramid = (Pyramid*)malloc(sizeof(Pyramid))) != NULL )
	{
		/* COnstruction de la base */
		Point_initGroup( &((pNewPyramid->tPoint)[1]), tCoord1[0], -dHeight/2, tCoord1[1]);
		Point_initGroup( &((pNewPyramid->tPoint)[2]), tCoord2[0], -dHeight/2, tCoord2[1]);
		Point_initGroup( &((pNewPyramid->tPoint)[3]), tCoord3[0], -dHeight/2, tCoord3[1]);

		/* Ajout du sommet de la pyramide*/
		Point_initGroup( &((pNewPyramid->tPoint)[0]), 0, dHeight/2, 0);

		/* Init du centre du repere de la figure (centre de gravité du Pyramid */
		Point_initGroup( &(pNewPyramid->Center), tCenter[0], tCenter[1], tCenter[2]);

		pNewPyramid->eType = TRIANGULAR;
		pNewPyramid->estSelectionne = FALSE;
	}
	else
	{
		/* TODO : Implémenter fatalError*/
	}
	return pNewPyramid;
}

GArray* Pyramid_facesOrder(Pyramid* pPyramid, InfoCamera* pCam)
{
	// sGravCenter -> structure qui va stocker le centre de gravité d'une face
	// sCamPoint -> point représentant les coordonnées du centre du repère de la caméra TODO : un peu la lose de devoir passer par cette structure, voir si on ne peut pas faire autrement
	Point sGravCenter, sCamPoint;
	GArray* gtIndexFaces=NULL; /* Tableau des index des faces à dessiner, c'est cette structure qui sera retournée*/
	GArray* gtDistances=NULL; /*Tableau des distance entre la caméra et le centre de gravité de chaque face, servira pour classer les indexs de face*/
	int iFaceIndex = 0;
	int iLoopInsert = 0;
	int iPoint1, iPoint2; //stockage les index des points du Pyramid pour calcul du centre de gravité
	double dDistance, dDistanceArray=0.0 ;
	int iFacesNb; /* Nombre de faces */
	Point sMedian;

	/*Allocation de GArray */
	gtIndexFaces = g_array_sized_new(FALSE,TRUE,sizeof(int),iFacesNb);
	gtDistances = g_array_sized_new(FALSE,TRUE,sizeof(double),iFacesNb);

	/* Initialisation du tableau des distances */
	for(iLoopInsert=0;iLoopInsert<=(iFacesNb-1);iLoopInsert++)
	{
		g_array_insert_val(gtIndexFaces,iLoopInsert,iFaceIndex);
		g_array_insert_val(gtDistances,iLoopInsert,dDistanceArray);
	}

	/* Création d'un point ayant pour coordonées le centre du repere de la caméra*/
	Point_init(&sCamPoint, pCam->CoordCam[0], pCam->CoordCam[1], pCam->CoordCam[2]);

	//On passe en revue les faces du Pyramid, pour chaque face on calcul la distance centre de gravite-centre repère caméra
	for(iFaceIndex=1;iFaceIndex<=iFacesNb;iFaceIndex++)
	{
		//recherche de centre de gravité de la face
		switch(iFaceIndex)
		{
			case 1:/*premiere face, afin de chercher le centre de gravité on prend la médiane du segment et le sommet*/
			{
				Point_middlePoint(&sMedian,&(pPyramid->tPoint)[1], &(pPyramid->tPoint)[2]); /*REcherche médiane*/
				Point_middlePoint(&sGravCenter, &sMedian, &(pPyramid->tPoint)[0]); /* Calcul centre de gravité*/
				iPoint1=0; iPoint2=2;
				break;
			}
			case 2:
			{
				Point_middlePoint(&sMedian,&(pPyramid->tPoint)[2], &(pPyramid->tPoint)[3]); /*REcherche médiane*/
				Point_middlePoint(&sGravCenter, &sMedian, &(pPyramid->tPoint)[0]); /* Calcul centre de gravité*/
				break;
			}
			case 3:
			{
				if(pPyramid->eType == TRIANGULAR)
				{
					Point_middlePoint(&sMedian,&(pPyramid->tPoint)[3], &(pPyramid->tPoint)[1]); /*REcherche médiane*/
					Point_middlePoint(&sGravCenter, &sMedian, &(pPyramid->tPoint)[0]); /* Calcul centre de gravité*/
				}
				else
				{
					Point_middlePoint(&sMedian,&(pPyramid->tPoint)[3], &(pPyramid->tPoint)[4]); /*REcherche médiane*/
					Point_middlePoint(&sGravCenter, &sMedian, &(pPyramid->tPoint)[0]); /* Calcul centre de gravité*/
				}
				break;
			}
			case 4:
			{
				if(pPyramid->eType == TRIANGULAR)
				{
					Point_middlePoint(&sMedian,&(pPyramid->tPoint)[1], &(pPyramid->tPoint)[2]); /*REcherche médiane*/
					Point_middlePoint(&sGravCenter, &sMedian, &(pPyramid->tPoint)[3]); /* Calcul centre de gravité*/
				}
				else
				{
					Point_middlePoint(&sMedian,&(pPyramid->tPoint)[4], &(pPyramid->tPoint)[1]); /*REcherche médiane*/
					Point_middlePoint(&sGravCenter, &sMedian, &(pPyramid->tPoint)[0]); /* Calcul centre de gravité*/
				}
				break;
			}
			case 5:
			{
				Point_middlePoint(&sGravCenter, &(pPyramid->tPoint)[2], &(pPyramid->tPoint)[4]); /* Calcul centre de gravité*/
				break;
			}
		}

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

void Pyramid_updateCoordWolrd(Objet* pObj)
{
	int i,j;
	tdMatrix tdMatPass;
	tCoord tdCoordBefore;
	tCoord tdCoordAfter; /* Va contenir les coordonnées de points màj après chaque itération */
	int iPointNb;

	/* On cherche à exprimer l'ensemble des coordonnées de points dans notre repere de la caméra --> pour projection */
	Groupe* pFatherGroup = NULL;
	Pyramid* pPyramid = pObj->type.pyramid; /* Récupération du pointeur sur notre objet d'un type plus précis que Objet* */

	if(pPyramid->eType == SQUARE)
		iPointNb = 5;
	else
		iPointNb = 4;

	for(i=0;i<iPointNb;i++ )  /* On passe tous les points de l'objet en revue */
	{
		pFatherGroup = pObj->pFatherGroup; /* on récupère un pointeur vers le groupe pere */
		for(j=0;j<4;j++)  /* initialisation du tableau de coordonnées avant tout changement de base */
			tdCoordBefore[j] = pPyramid->tPoint[i].tdCoordGroup[j];

		/* PREMIER CHANGEMENT DE BASE = PASSAGE REPERE OBJET --> GROUPE PERE */
		/* Passage des coordonnées du point dans le premier groupe pere */
		Matrix_initIdentityMatrix(tdMatPass); /* Initialisation de la matrice pour construction d'un matrice de passage */
		/* COnstruction de la nouvelle matrice de passage grâce aux coordonnées du repere objet dans son groupe pere*/
		tdMatPass[0][3] = pPyramid->Center.tdCoordGroup[0];
		tdMatPass[1][3] = pPyramid->Center.tdCoordGroup[1];
		tdMatPass[2][3] = pPyramid->Center.tdCoordGroup[2];

		Matrix_multiMatrixVect(tdMatPass, tdCoordBefore, tdCoordAfter); /* tdCoordAfter contient les coordonnées du point après le premier changement de base*/
		ProjectionTools_getCoordWorld(tdCoordAfter,pFatherGroup,&(pPyramid->tPoint[i]));
	}

	pFatherGroup = pObj->pFatherGroup;
	/* On met aussi à jour les coordonnées du centre de l'objet */
	ProjectionTools_getCoordWorld(pPyramid->Center.tdCoordGroup,pFatherGroup,&(pPyramid->Center));
}

void Pyramid_drawPyramid( Objet* pObj,cairo_t* cr,InfoCamera* pCam)
{
	GArray* gtTabFacesOrder=NULL; /* Tableau indiquant dans quel ordre il faut dessiner les faces du Pyramid */
	/* Coordonnées de points une fois projettés */
	tCoord2D* pPointProj0 = NULL;
	tCoord2D* pPointProj1 = NULL;
	tCoord2D* pPointProj2 = NULL;
	tCoord2D* pPointProj3 = NULL;
	tCoord2D* pPointProj4 = NULL;
	Pyramid* pPyramid = pObj->type.pyramid;
	int iFacesNb; /* Va contenir le nombre de faces*/

	Pyramid_updateCoordWolrd(pObj);

	/* Projection de tous les points de la Pyramid */
	pPointProj0 = ProjectionTools_getPictureCoord(&((pPyramid->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pPyramid->tPoint)[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pPyramid->tPoint)[2]),pCam);
	pPointProj3 = ProjectionTools_getPictureCoord(&((pPyramid->tPoint)[3]),pCam);

	if( pPyramid->eType == SQUARE )
		pPointProj4 = ProjectionTools_getPictureCoord(&((pPyramid->tPoint)[4]),pCam);


	/* Recherche de l'ordre dans lequel on doit dessiner les faces */
	gtTabFacesOrder = Pyramid_facesOrder(pPyramid,pCam);
	int iFaceIndex = 0;
	int iFace =0;
	/* Dessin face par face dans l'ordre*/
	for(iFaceIndex=0;iFaceIndex<iFacesNb;iFaceIndex++)  /* On parcourt toutes les faces */
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
				cairo_close_path(cr);
				//cairo_line_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);

				break;
			}
			case 2:
			{
				cairo_move_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
				cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
				cairo_line_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
				cairo_close_path(cr);

				break;
			}
			case 3:
			{
				if( pPyramid->eType == TRIANGULAR)
				{
					cairo_move_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
					cairo_line_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
					cairo_line_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
					cairo_close_path(cr);
				}
				else
				{
					cairo_move_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
					cairo_line_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
					cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
					cairo_close_path(cr);
				}
				break;
			}
			case 4:
			{
				if( pPyramid->eType == TRIANGULAR) /* Base triangulaire */
				{
					cairo_move_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
					cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
					cairo_line_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
					cairo_close_path(cr);
				}
				else
				{
					cairo_move_to( cr, (*pPointProj0)[0], (*pPointProj0)[1]);
					cairo_line_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
					cairo_line_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
					cairo_close_path(cr);
				}
				break;
			}
			case 5: /* Face 5 == Base pyramid square*/
			{
				cairo_move_to( cr, (*pPointProj1)[0], (*pPointProj1)[1]);
				cairo_line_to( cr, (*pPointProj2)[0], (*pPointProj2)[1]);
				cairo_line_to( cr, (*pPointProj3)[0], (*pPointProj3)[1]);
				cairo_move_to( cr, (*pPointProj4)[0], (*pPointProj4)[1]);
				cairo_close_path(cr);
				break;
			}
		}
		cairo_set_source_rgba (cr, pPyramid->tColor[0], pPyramid->tColor[1], pPyramid->tColor[2] , pPyramid->tColor[3]); /*Couleur */
		cairo_fill_preserve( cr );/*remplissage du rectangle avec path preservé*/
		cairo_set_line_width(cr,0.8);/* réglage taille de la ligne*/

		if(pPyramid->estSelectionne == TRUE)  /* réglage de la couleur du contour*/
			cairo_set_source_rgb ( cr, 1.0, 0, 0);
		else
			cairo_set_source_rgb ( cr, 0, 0, 0);

		cairo_stroke(cr); /* dessin contour, perte du path */
	}

	/* Libération mémoire */
	g_array_free(gtTabFacesOrder, TRUE);
	free(pPointProj0);
	free(pPointProj1);
	free(pPointProj2);
	free(pPointProj3);

	if(pPyramid->eType == SQUARE)
		free(pPointProj4);
}

void Pyramid_modSize(Pyramid* pPyramid, double dRatio)
{
	int iLoop;
	tdMatrix tdMatTransfo, tdMatPassRepObj;
	tCoord tdCoordRepObj, tdCoordApTransfo;

	/* Initialisation des coordonées*/
	Point_initCoord(tdCoordRepObj, 0, 0, 0);
	Point_initCoord(tdCoordApTransfo, 0, 0, 0);

	/* Construction de la matrice de passage World -> Repere objet */
	Matrix_initMatrix(tdMatPassRepObj); /* initialisation de la matrice de passage dans repere objet*/
	tdMatPassRepObj[0][3] = -pPyramid->Center.tdCoordWorld[0];
	tdMatPassRepObj[1][3] = -pPyramid->Center.tdCoordWorld[1];
	tdMatPassRepObj[2][3] = -pPyramid->Center.tdCoordWorld[2];

	if(dRatio != 1)
	{
		/*Récupération de la matrice d'homothétie*/
		Transformation_getMatrixHomothety(tdMatTransfo, dRatio);

		/* On effectue la transformation pour tous  les points du Pyramid */
		for(iLoop=0 ; iLoop<8 ;iLoop++)
		{
			/* Tout d'abord recherche des coordonnées dans le repere de l'objet*/
			Matrix_multiMatrixVect(tdMatPassRepObj, pPyramid->tPoint[iLoop].tdCoordWorld, tdCoordRepObj);
			/*Puis transformation, toujours dans le repere objet*/
			Matrix_multiMatrixVect(tdMatTransfo, tdCoordRepObj, tdCoordApTransfo);
			/* Modification des coordonnées dans le repere du monde !*/
			pPyramid->tPoint[iLoop].tdCoordWorld[0] = pPyramid->tPoint[iLoop].tdCoordWorld[0]
																+ (tdCoordApTransfo[0]-tdCoordRepObj[0]);
			pPyramid->tPoint[iLoop].tdCoordWorld[1] = pPyramid->tPoint[iLoop].tdCoordWorld[1]
																			+ (tdCoordApTransfo[1]-tdCoordRepObj[1]);
			pPyramid->tPoint[iLoop].tdCoordWorld[2] = pPyramid->tPoint[iLoop].tdCoordWorld[2]
																			+ (tdCoordApTransfo[2]-tdCoordRepObj[2]);

			/* on réinitialise les vecteurs contenant les infos sur la transformation */
			Point_initCoord(tdCoordRepObj, 0, 0, 0);
			Point_initCoord(tdCoordApTransfo, 0, 0, 0);
		}
	}
}



gboolean Pyramid_Contient_Point( Pyramid* pPyramid, double x, double y, InfoCamera* pCam)
{
    gboolean est_contenu = FALSE;
    tCoord2D* tCoordTab[4]; /* Tableauq qui va contenir les points relatifs à une face donnée */
	tCoord2D* pPointProj0 = NULL;tCoord2D* pPointProj4 = NULL;
	tCoord2D* pPointProj1 = NULL;tCoord2D* pPointProj5 = NULL;
	tCoord2D* pPointProj2 = NULL;tCoord2D* pPointProj6 = NULL;
	tCoord2D* pPointProj3 = NULL;tCoord2D* pPointProj7 = NULL;

	/* Projection de tous les point du Pyramid */
	pPointProj0 = ProjectionTools_getPictureCoord(&((pPyramid->tPoint)[0]),pCam);
	pPointProj1 = ProjectionTools_getPictureCoord(&((pPyramid->tPoint)[1]),pCam);
	pPointProj2 = ProjectionTools_getPictureCoord(&((pPyramid->tPoint)[2]),pCam);
	pPointProj3 = ProjectionTools_getPictureCoord(&((pPyramid->tPoint)[3]),pCam);
	pPointProj4 = ProjectionTools_getPictureCoord(&((pPyramid->tPoint)[4]),pCam);
	pPointProj5 = ProjectionTools_getPictureCoord(&((pPyramid->tPoint)[5]),pCam);
	pPointProj6 = ProjectionTools_getPictureCoord(&((pPyramid->tPoint)[6]),pCam);
	pPointProj7 = ProjectionTools_getPictureCoord(&((pPyramid->tPoint)[7]),pCam);

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

void Pyramid_transfoCenter(Pyramid* pPyramid, tdMatrix tdTransfoMat)
{
	tCoord tCoordApTransfo;

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);
	/*APplication de la transformation au centre du rectangle */
	Matrix_multiMatrixVect(tdTransfoMat, pPyramid->Center.tdCoordGroup, tCoordApTransfo);

	pPyramid->Center.tdCoordGroup[0] = tCoordApTransfo[0];
	pPyramid->Center.tdCoordGroup[1] = tCoordApTransfo[1];
	pPyramid->Center.tdCoordGroup[2] = tCoordApTransfo[2];
	pPyramid->Center.tdCoordGroup[3] = tCoordApTransfo[3];
}

void Pyramid_transfo(Pyramid* pPyramid, tdMatrix tdTransfoMat)
{
	tCoord tCoordApTransfo;
	int i;

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);
	for(i=0 ; i<8 ;i++)
	{
		Matrix_multiMatrixVect(tdTransfoMat, pPyramid->tPoint[i].tdCoordGroup, tCoordApTransfo);

		/* Modification des coordonées dans le repere objet */
		pPyramid->tPoint[i].tdCoordGroup[0] = tCoordApTransfo[0];
		pPyramid->tPoint[i].tdCoordGroup[1] = tCoordApTransfo[1];
		pPyramid->tPoint[i].tdCoordGroup[2] = tCoordApTransfo[2];
		pPyramid->tPoint[i].tdCoordGroup[3] = tCoordApTransfo[3];

		Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);  /* Reinitialisation de la matrice de coordonnées après transformation*/
	}
}
