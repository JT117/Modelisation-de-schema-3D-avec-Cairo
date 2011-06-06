#include "MouseInterface.h"


void MouseInterface_segment(Scene* scene,GdkEventButton* pEvent)
{
	/* La création d'un objet va fonctionner en 5 phases:
	 * 1 - Selection d'un premier objet
	 * 2 - Selection d'un point d'interêt de cet objet
	 * 3 - Selection d'un second objet
	 * 4 - Selection d'un point d'interêt de ce second objet
	 * 5 - construction du segment
	 */
	int i;
	Objet* objet = NULL;
	tCoord2D tClick;
	/*Reservation en static de la structure permettant de gérer la création d'un segment */
	static createSegment sCreateSeg;

	switch(sCreateSeg.iPhase) /*On adapate le code en fonction de la phase dans laquelle nous sommes */
	{
		case 0: /* Premier cas, aucun objet selectionné, on recherche si l'utilisateur a cliqué sur un objet */
		{
			/* On passe en revue tous les objets */
			for( i = 0; i < scene->tObjet->len; i++ )
			{
				objet = (Objet*)g_array_index( scene->tObjet, Objet*, i );
				gboolean estContenu = Objet_contient_point( objet,pEvent->x,pEvent->y, scene->camera );

				if( estContenu  )
				{
					/* Premier objet selectionné*/
					sCreateSeg.pObj1 = objet;
					objet->iSelectedForSegment = -1; /* on change le status de l'objet, objet selectionné en attente de selection de point */
					/* On passe à la phase 1*/
					sCreateSeg.iPhase++;
					gtk_widget_queue_draw( scene->zoneDeDessin );
					break; /* On met fain  la boucle */
				}
			}
			break;
		}
		case 1: /* 1er objet selectionné, choix du point */
		{
			Point_initCoord2D(tClick,pEvent->x,pEvent->y); /* sauvegarde coordonnées du clique */
			objet = sCreateSeg.pObj1;
			Point* pPoints;
			Point* pSelectedPoint = NULL;
			int iSelectedPoint = 0;
			double dDistance;

			int iNbPoints;

			if( objet->eType == CUBE)
			{
				iNbPoints = 8;
				pPoints = objet->type.cube->tPoint;
			}
			if( objet->eType == RECTANGLE)
			{
				iNbPoints = 4;
				pPoints = objet->type.rectangle->tPoint;
			}
			if( objet->eType == TRIANGLE)
			{
				iNbPoints = 3;
				pPoints = objet->type.triangle->tPoint;
			}
			if( objet->eType == SPHERE)
			{
				iNbPoints = 1;
				pPoints = objet->type.sphere->tPoint;
			}
			if( objet->eType == PYRAMID)
			{
				iNbPoints = 4;
				//pPoints = objet->type.pyramid->tPoint;
			}
			if( objet->eType == QUADRILATERAL)
			{
				iNbPoints = 4;
				pPoints = objet->type.quadrilateral->tPoint;
			}

			/* On passe en revue tous le points d'un face et on calcul la distance euclidienne à chaque fois */
			for(i=0;i<iNbPoints;++i)
			{
				dDistance = Point_euclideanDistance2D(tClick,pPoints[i].tCoordProjection) ;
				if( dDistance < THRESHOLD)  /* Calcul de la distance avec chaque objet concerné */
				{
					/* Si la distance est plus petit à un seuim près alors on choisi le point */
					pSelectedPoint = &(pPoints[i]);
					iSelectedPoint = i;
					break;
				}
			}

			if( pSelectedPoint != NULL )
			{
				sCreateSeg.pPoint1 = pSelectedPoint;
				sCreateSeg.pObj1->iSelectedForSegment = iSelectedPoint;
				/* Une fois le point selectionné on passe à la phase suivante */
				sCreateSeg.iPhase++;
			}
			else /* Pas de point trouvé... on annule tout !! */
			{
				sCreateSeg.iPhase = 0;
				sCreateSeg.pObj1->iSelectedForSegment = -2;
				sCreateSeg.pObj1 = NULL;
			}
			gtk_widget_queue_draw( scene->zoneDeDessin );
			break;
		}
		case 2: /* 1er point selectionné, choix du second Objet */
		{
			/* On passe en revue tous les objets */
			for( i = 0; i < scene->tObjet->len; i++ )
			{
				objet = (Objet*)g_array_index( scene->tObjet, Objet*, i );
				gboolean estContenu = Objet_contient_point( objet, pEvent->x, pEvent->y, scene->camera );

				if( estContenu  )
				{
					/* 2nd objet selectionné*/
					sCreateSeg.pObj2 = objet;
					objet->iSelectedForSegment = -1; /* on change le status de l'objet, objet selectionné en attente de selection de point */
					/* On passe à la phase 3*/
					sCreateSeg.iPhase++;
					break; /* On met fain  la boucle */
				}
			}

			if( sCreateSeg.iPhase != 3 ) /* Aucun objet a été trouvé, vu que c'est la lose on annule tout, un peu comme avec les projets de SE */
			{
				sCreateSeg.iPhase = 0;
				sCreateSeg.pObj1->iSelectedForSegment = -2;
				sCreateSeg.pPoint1 = NULL;
			}
			gtk_widget_queue_draw( scene->zoneDeDessin );
			break;
		}
		case 3: /* 2nd objet selectionné */
		{
			Point_initCoord2D(tClick,pEvent->x,pEvent->y); /* sauvegarde coordonnées du clique */
			objet = sCreateSeg.pObj2;
			Point* pPoints;
			Point* pSelectedPoint = NULL;

			int iNbPoints;

			if( objet->eType == CUBE)
			{
				iNbPoints = 8;
				pPoints = objet->type.cube->tPoint;
			}
			if( objet->eType == RECTANGLE)
			{
				iNbPoints = 4;
				pPoints = objet->type.rectangle->tPoint;
			}
			if( objet->eType == TRIANGLE)
			{
				iNbPoints = 3;
				pPoints = objet->type.triangle->tPoint;
			}
			if( objet->eType == SPHERE)
			{
				iNbPoints = 1;
				pPoints = objet->type.sphere->tPoint;
			}
			if( objet->eType == PYRAMID)
			{
				iNbPoints = 4;
				//pPoints = objet->type.pyramid->tPoint;
			}
			if( objet->eType == QUADRILATERAL)
			{
				iNbPoints = 4;
				pPoints = objet->type.quadrilateral->tPoint;
			}

			/* On passe en revue tous le points d'un face et on calcul la distance euclidienne à chaque fois */
			for(i=0;i<iNbPoints;++i)
			{
				if( Point_euclideanDistance2D(tClick,pPoints[i].tCoordProjection)< THRESHOLD)  /* Calcul de la distance avec chaque objet concerné */
				{
					/* Si la distance est plus petit à un seuim près alors on choisi le point */
					pSelectedPoint = &(pPoints[i]);
					break;
				}
			}

			if( pSelectedPoint != NULL )
			{
				sCreateSeg.pPoint2 = pSelectedPoint;
				/* Une fois le point selectionné on passe à la phase suivante */
				sCreateSeg.iPhase++;
			}
			else /* Pas de point trouvé... on annule tout !! */
			{
				sCreateSeg.iPhase = 0;
				sCreateSeg.pObj1->iSelectedForSegment = -2;
				sCreateSeg.pObj2->iSelectedForSegment = -2;
				sCreateSeg.pObj1 = NULL;
				sCreateSeg.pObj2 = NULL;
				sCreateSeg.pPoint1 = NULL;
			}
		}
	}/* FIN SWITCH */

	if( sCreateSeg.pPoint1 != NULL && sCreateSeg.pPoint2 != NULL) /* On a toutes les infos necessaires  la création du segemnt */
	{
		/* Invocation de la fenêtre des tenébres */
		MouseInterface_newSegment(&sCreateSeg,scene);
		/*Avant de dessiner on on deselectionne les objets qui nous ont servis à créer le segemnet */
		sCreateSeg.iPhase = 0;
		sCreateSeg.pObj1->iSelectedForSegment = -2;
		sCreateSeg.pObj2->iSelectedForSegment = -2;
		sCreateSeg.pObj1 = NULL;
		sCreateSeg.pObj2 = NULL;
		sCreateSeg.pPoint1 = NULL;
		sCreateSeg.pPoint2 = NULL;
		gtk_widget_queue_draw( scene->zoneDeDessin );
	}
}

gboolean MouseInterface_newSegment(createSegment*  pCreateSeg, Scene* scene)
{
	FenSeg* fseg = (FenSeg*)malloc( 1 * sizeof( FenSeg ) );
	fseg->pP1 = pCreateSeg->pPoint1;
	fseg->pP2 = pCreateSeg->pPoint2;
	FenSeg_init( fseg, scene);

	return TRUE;
}

