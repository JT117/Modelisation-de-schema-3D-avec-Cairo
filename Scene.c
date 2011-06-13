#include "Scene.h"
#include "Selection.h"

/** Fonction qui initialise une scene vierge
 * @param scene, un pointeur sur une scene vierge
 * @param window, la zone de dessin à garder
 **/
void Scene_initialiser_scene( Scene* scene, GtkWidget* window, GtkWidget* mainWindow  )
{
    scene->tObjet = g_array_new( FALSE, TRUE, sizeof( Objet* ) );
    scene->nbObjet = 0;

    scene->zoneDeDessin = window;
    scene->mainWindow = mainWindow;

    scene->selection = (Selection*)malloc( 1 * sizeof(Selection) );
    Selection_initialiser( scene->selection );

    Clavier* clavier = (Clavier*)malloc( 1 * sizeof( Clavier ) );
    Clavier_initialiser( clavier );
    scene->clavier = clavier;

    scene->modification = (Modification*)malloc( 1 * sizeof( Modification ) );
    Modification_initialiser( scene->modification );

    scene->groupeDeBase = (Groupe*)malloc( 1 * sizeof( Groupe ) );

    Groupe_initialiser( scene->groupeDeBase, NULL, GROUPE0, 0.0,0.0,0.0 );

    scene->tGroupe = g_array_new( FALSE, FALSE, sizeof( Groupe* ) ); //coucou =)
    g_array_append_val( scene->tGroupe, scene->groupeDeBase );
    scene->nbGroupe = 1;

    scene->tailleCreation = 50.0;
    scene->creation = (Point*)malloc( 1* sizeof( Point ) );
    scene->creation->x = 0.0;
    scene->creation->y = 0.0;
    scene->creation->z = 0.0;

    scene->fenetre = NULL;
    scene->CBajoutGroupe = NULL;
    scene->entryNom = NULL;
    scene->souris = NORMAL;

    scene->curseur = gdk_cursor_new_from_pixbuf( gdk_display_get_default(), gdk_pixbuf_new_from_file( "normal.png", NULL), 4, 4 );
}

void Scene_reconstruire( Scene* scene, GtkWidget* window )
{
    scene->tObjet = g_array_new( FALSE, TRUE, sizeof( Objet* ) );
    scene->nbObjet = 0;

    Groupe* groupeDeBase = (Groupe*)malloc( 1 * sizeof( Groupe ) );

    Groupe_initialiser( groupeDeBase, NULL, GROUPE0, 0.0,0.0,0.0 );

    scene->tGroupe = g_array_new( FALSE, FALSE, sizeof( Groupe* ) ); //coucou =)
    g_array_append_val( scene->tGroupe, groupeDeBase );
    scene->nbGroupe = 1;

    scene->zoneDeDessin = window;

    scene->selection = (Selection*)malloc( 1 * sizeof(Selection) );
    Selection_initialiser( scene->selection );

    Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, 0 );

    gtk_tree_store_append (scene->store, groupe->iter, NULL);
    gtk_tree_store_set (scene->store, groupe->iter, GROUPE, "Groupe 0", -1);
}

/** Fonction qui libère une scene initialisée
 * @param scene, un pointeur sur une scene initialisée
 * @warning scene et les pointeurs sur les modules ne sont pas liberée
 **/
void Scene_detruire( Scene* scene )
{
    int i = 0;

    gtk_tree_selection_unselect_all( scene->treeSelection );
    gtk_tree_store_clear( scene->store  );

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet_detruire( g_array_index( scene->tObjet, Objet*, i ) );
    }
    g_array_free( scene->tObjet, TRUE );

    for( i = 0; i < scene->nbGroupe; i++ )
    {
        Groupe_detruire(g_array_index( scene->tGroupe, Groupe*, i ) );
    }
    g_array_free( scene->tGroupe, TRUE );

    Selection_detruire( scene->selection );
}

/** Fonction qui ajoute un objet de type Cube à la scene
 * @param scene, un pointeur sur une scene initialisée
 * @param cCube, le cube à ajouter
 **/
void Scene_ajouter_cube( Scene* scene, Cube* cCube, int idGroupe )
{
    Objet* objet = (Objet*)malloc( 1 * sizeof( Objet ) );
    Objet_est_un_Cube( objet, cCube );
    g_array_append_val( scene->tObjet, objet );
    scene->nbObjet++;

    Groupe* groupe = Groupe_trouver_ById( scene, idGroupe );
    objet->pFatherGroup = groupe;  /* On enregistre l'adresse du groupe pere */
    Groupe_ajouter_Objet( groupe, objet );

    gtk_tree_store_append (scene->store, objet->iter, groupe->iter );
    gtk_tree_store_set (scene->store, objet->iter, GROUPE, "Cube", -1);

}

void Scene_ajouter_segment( Scene* scene, Segment* pSeg, int idGroupe )
{
    Objet* objet = (Objet*)malloc( 1 * sizeof( Objet ) );
    Objet_est_un_Segment( objet, pSeg );
    g_array_append_val( scene->tObjet, objet );
    scene->nbObjet++;

    Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, idGroupe );
    objet->pFatherGroup = groupe;  /* On enregistre l'adresse du groupe pere */
    Groupe_ajouter_Objet( groupe, objet );

    gtk_tree_store_append (scene->store, objet->iter, groupe->iter );
    gtk_tree_store_set (scene->store, objet->iter, GROUPE, "Segment", -1);
}

void Scene_ajouter_rectangle( Scene* scene, Rectangle* rect, int idGroupe )
{
    Objet* objet = (Objet*)malloc( 1 * sizeof( Objet ) );
    Objet_est_un_Rectangle( objet, rect );
    g_array_append_val( scene->tObjet, objet );
    scene->nbObjet++;

    Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, idGroupe );
    objet->pFatherGroup = groupe;  /* On enregistre l'adresse du groupe pere */
    Groupe_ajouter_Objet( groupe, objet );

    gtk_tree_store_append (scene->store, objet->iter, groupe->iter );
    gtk_tree_store_set (scene->store, objet->iter, GROUPE, "Rectangle", -1);
}

void Scene_ajouter_triangle( Scene* scene, Triangle* pTri, int idGroupe )
{
    Objet* objet = (Objet*)malloc( 1 * sizeof( Objet ) );
    Objet_est_un_Triangle( objet, pTri );
    g_array_append_val( scene->tObjet, objet );
    scene->nbObjet++;

    Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, idGroupe );
    objet->pFatherGroup = groupe;  /* On enregistre l'adresse du groupe pere */
    Groupe_ajouter_Objet( groupe, objet );

    gtk_tree_store_append (scene->store, objet->iter, groupe->iter );
    gtk_tree_store_set (scene->store, objet->iter, GROUPE, "Triangle", -1);
}

void Scene_ajouter_sphere(Scene* scene, Sphere* sphere, int idGroupe )
{
    Objet* objet = (Objet*)malloc( 1 * sizeof( Objet ) );
    Objet_est_une_Sphere( objet, sphere );
    g_array_append_val( scene->tObjet, objet );
    scene->nbObjet++;

    Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, idGroupe );
    objet->pFatherGroup = groupe;  /* On enregistre l'adresse du groupe pere */
    Groupe_ajouter_Objet( groupe, objet );

    gtk_tree_store_append (scene->store, objet->iter, groupe->iter );
    gtk_tree_store_set (scene->store, objet->iter, GROUPE, "Sphère", -1);
}

void Scene_ajouter_quadrilateral(Scene* scene, Quadrilateral* pQuadri, int idGroupe )
{
    Objet* objet = (Objet*)malloc( 1 * sizeof( Objet ) );
    Objet_est_un_Quadrilateral( objet, pQuadri);
    g_array_append_val( scene->tObjet, objet );
    scene->nbObjet++;

    Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, idGroupe );
    objet->pFatherGroup = groupe;  /* On enregistre l'adresse du groupe pere */
    Groupe_ajouter_Objet( groupe, objet );

    gtk_tree_store_append (scene->store, objet->iter, groupe->iter );
    gtk_tree_store_set (scene->store, objet->iter, GROUPE, "Sphère", -1);
}

/**
 * Face enregistrer dans un nouveau tableau GArray alloué sur le tas les adresses des objets dans l'ordre dans lequel ils doivent être dessinés.
 * @param tObjects Les objets à dessiner
 * @return Le tableau des adresses sur objets dans l'ordre
 */
GArray* Scene_drawOrder( Scene* pScene, InfoCamera* pCam)
{
	Point sCamPoint;
	GArray* gtOrderedElements=NULL; /* Tableau des index des faces à dessiner, c'est cette structure qui sera retournée*/
	Objet* pObj = NULL;
	Groupe* pGroup = NULL;
	int i = 0;
	int iLoopInsert = 0;
	double dDistance= 0.0, dDistancePrev=0.0;
	GArray* tObj = NULL;
	tObj = pScene->tObjet;

	GArray* tGroup = NULL;
	tGroup = pScene->tGroupe;

	ClassifyObj* pToInsert;
	ClassifyObj* pPrevious;

	/*Allocation des GArray */
	gtOrderedElements = g_array_sized_new(FALSE,TRUE,sizeof(ClassifyObj*),(tObj->len+tGroup->len)+1 );

	if( (pToInsert = (ClassifyObj*)malloc(sizeof(ClassifyObj)) ) != NULL ) /* Initiliasation */
	{
		pToInsert->dDistance = 0.0;
		pToInsert->pGroup = NULL; /* ce qu'on enregistre n'est pas un groupe */
		pToInsert->pObj = NULL;
	}


	g_array_insert_val(gtOrderedElements,0,pToInsert);
	/* Création d'un point ayant pour coordonées le centre du repere de la caméra*/
	Point_init(&sCamPoint, pCam->CoordCam[0], pCam->CoordCam[1], pCam->CoordCam[2]);
	/* On commence par passer en revue les objets */
	for( i=0; i<tObj->len ; i++) /* Pour chaque objet */
	{
		pObj =  (Objet*)g_array_index(tObj,Objet*,i);
		/* calcul de la distance entre le centre de gravité de l'objet et le centre de repere de la caméra */
		if( pObj->eType == CUBE )  /* Si l'objet est un cube */
			dDistance = Point_euclideanDistance( &(pObj->type.cube->Center), &sCamPoint);
		else if( pObj->eType == RECTANGLE )
			dDistance = Point_euclideanDistance( &(pObj->type.rectangle->Center), &sCamPoint);
		else if( pObj->eType == SPHERE )
			dDistance = Point_euclideanDistance( &(pObj->type.sphere->Center), &sCamPoint);
		else if( pObj->eType == SEGMENT )
			dDistance = Point_euclideanDistance( &(pObj->type.segment->Center), &sCamPoint);
		else if( pObj->eType == TRIANGLE )
					dDistance = Point_euclideanDistance( &(pObj->type.triangle->Center), &sCamPoint);
		else if( pObj->eType == QUADRILATERAL )
					dDistance = Point_euclideanDistance( &(pObj->type.quadrilateral->Center), &sCamPoint);

		/* Insertion là où il faut ! */
		iLoopInsert=0;
		pPrevious = g_array_index(gtOrderedElements,ClassifyObj*,iLoopInsert);
		dDistancePrev = pPrevious->dDistance;
		/* Insertion de l'index de la face là où il faut*/
		while( ( dDistancePrev != 0.0 ) && ( dDistancePrev > dDistance ) )
		{
			iLoopInsert++;
			pPrevious = (ClassifyObj*)g_array_index(gtOrderedElements,ClassifyObj*,iLoopInsert);
			dDistancePrev = pPrevious->dDistance;
		}

		/*Emplacement d'insertion retrouvé...*/
		/* On insère une nouvelle structure */
		if( (pToInsert = (ClassifyObj*)malloc(sizeof(ClassifyObj)) ) != NULL )
		{
			pToInsert->dDistance = dDistance;
			pToInsert->pGroup =NULL; /* ce qu'on enregistre n'est pas un groupe */
			pToInsert->pObj = pObj;
		}

		g_array_insert_val(gtOrderedElements,iLoopInsert,pToInsert);
	}

	/* Une fois les objets classés on continue avec les groupes */
	for( i=0; i<tGroup->len ; i++) /* Pour chaque groupe */
	{
		pGroup =  (Groupe*)g_array_index(tGroup,Groupe*,i);
		/* calcul de la distance entre le centre du groupe et le centre de repere de la caméra */
		dDistance = Point_euclideanDistance( &(pGroup->tCenterGroup), &sCamPoint);

		/* Insertion là où il faut ! */
		iLoopInsert=0;
		pPrevious = g_array_index(gtOrderedElements,ClassifyObj*,iLoopInsert);
		dDistancePrev = pPrevious->dDistance;
		/* Insertion de l'index de la face là où il faut*/
		while(dDistancePrev != 0 && dDistancePrev>dDistance)
		{
			iLoopInsert++;
			pPrevious = (ClassifyObj*)g_array_index(gtOrderedElements,ClassifyObj*,iLoopInsert);
			dDistancePrev = pPrevious->dDistance;
		}

		/*Emplacement d'insertion retrouvé...*/
		/* On insère une nouvelle structure */
		if( (pToInsert = (ClassifyObj*)malloc(sizeof(ClassifyObj)) )  != NULL )
		{
			pToInsert->dDistance = dDistance;
			pToInsert->pGroup = pGroup; /* ce qu'on enregistre n'est pas un groupe */
			pToInsert->pObj = NULL;
		}
		else
			printf("Raaaaaah c'est le chaos !");

		g_array_insert_val(gtOrderedElements,iLoopInsert,pToInsert);
	}

	return gtOrderedElements;
}

void Scene_deleteClassifiedElements(GArray* tToDelete)
{
	int i;
	for( i = 0; i < tToDelete->len; i++ )
	{
		free( g_array_index(tToDelete,ClassifyObj*,i));
	}
	g_array_free(tToDelete,FALSE);
}

/** Fonction qui dessiner tout les objets de la scene
 * @param scene, un pointeur sur une scene initialisée
 * @param cr, un contexte cairo créer sur la zoneDeDessin
 **/
void Scene_dessiner_scene( Scene* scene, cairo_t* cr )
{
    int i = 0;
    GArray* gtOrderedElements = NULL;/* va contenir le tableau des objets dans l'ordre dans lequel il faut les dessiner */
    ClassifyObj* pElement = NULL;
    int iToDrawNb = (scene->tGroupe->len+scene->tObjet->len);

    /* Mise à jour des coordonnées dans le repere du monde de chaque objet et de chaque groupe*/
    if( (scene->tObjet->len + scene->tGroupe->len)!= 0)
	{
    	for(i=0; i<scene->tObjet->len; ++i)
    	{
    		Objet_updateCoordWorld(g_array_index(scene->tObjet,Objet*,i));
    	}

    	for(i=0; i<scene->tGroupe->len; ++i)
		{
			Groupe_updateCoordWorld(g_array_index(scene->tGroupe,Groupe*,i));
		}
	}

    gtOrderedElements = Scene_drawOrder(scene,scene->camera); /* Récupération du tableau ordonné des elements */

    for( i = 0; i < iToDrawNb; i++ )
    {
    	pElement = g_array_index(gtOrderedElements,ClassifyObj*,i); /* Récupération de l'element à dessiner*/

    	if( pElement->pGroup != NULL)
    		Groupe_drawMark(pElement->pGroup,cr,scene->camera);
    	else
			Objet_dessiner_objet(pElement->pObj,cr,scene->camera);
    }

    /* On libère la mémoire */
    Scene_deleteClassifiedElements(gtOrderedElements);
}

/**
 * Fonction qui dessine une scene vierge
 *
 * @param scene, un pointeur sur une scene initialisée
 * @param cr, un contexte cairo créer sur la zoneDeDessin
 **/
void Scene_clear_scene( Scene* scene, cairo_t* cr, double dR, double dG, double dB )
{
     cairo_set_source_rgb( cr, dR, dG, dB );
     cairo_paint( cr );
}

void Scene_creation_objet( Scene* scene, double x, double y )
{
	double dX, dY;
	InfoCamera* pCam = scene->camera;
	/* Creation définit les coordonnées du pixel sur lequel l'utilisateur a cliqué lors de la creation de son objet*/

	dX = (x - pCam->dRmax) * 2*pCam->dDMin/ pCam->dDMin;
	dY = (-y + pCam->dUmax) * 2*pCam->dDMin/ pCam->dDMin;

	/* On fait correspondre les coordonnées du curseur ave cles coordonées du modèle 3D */
	Point_init(scene->creation,dX, dY, 0);
}

/** Fonction qui remets la scene en etat d'initialisation
 * @param scene, un pointeur sur une scene initialisée
 * @param window, une pointeur sur la zoneDeDessin
 * @warning La zone de dessin est conservé
 **/
void Scene_reset( Scene* scene, GtkWidget* window )
{
    Scene_detruire( scene );
    free( scene->selection );
    Scene_reconstruire( scene, window );
}

/** Fonction qui supprime un objet de la scene
  * @param scene, un pointeur sur la scene initialisée
  * @param objet, un pointeur sur l'objet à enlever de la scene
  **/
void Scene_enlever_objet( Scene* scene, Objet* objet )
{
    int i = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet* objetDuTableau = g_array_index( scene->tObjet, Objet*, i );

        if( objet == objetDuTableau )
        {
            g_array_remove_index_fast( scene->tObjet, i );
            scene->nbObjet--;

            Objet_detruire( objet );
            free( objet );
        }
    }
}
