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

    Groupe* groupeDeBase = (Groupe*)malloc( 1 * sizeof( Groupe ) );
    Groupe_initialiser( groupeDeBase, NULL, GROUPE0, 0.0,0.0,0.0 );
    scene->tGroupe = g_array_new( FALSE, FALSE, sizeof( Groupe* ) ); //coucou =)
    g_array_append_val( scene->tGroupe, groupeDeBase );
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

    scene->curseur = gdk_cursor_new_from_pixbuf( gdk_display_get_default(), gtk_image_get_pixbuf( GTK_IMAGE( gtk_image_new_from_file( "normal.png") ) ), 4, 4 );
}

void Scene_reconstruire( Scene* scene, GtkWidget* window )
{
    scene->tObjet = g_array_new( FALSE, TRUE, sizeof( Objet* ) );
    scene->nbObjet = 0;

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

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet_detruire( g_array_index( scene->tObjet, Objet*, i ) );
    }
    g_array_free( scene->tObjet, TRUE );

    Selection_detruire( scene->selection );

    gtk_tree_selection_unselect_all( scene->treeSelection );
    gtk_tree_store_clear( scene->store  );
    //Clavier_detruire( scene->clavier );
}

/** Fonction qui ajoute un objet de type Cube à la scene
 * @param scene, un pointeur sur une scene initialisée
 * @param cCube, le cube à ajouter
 **/
void Scene_ajouter_cube( Scene* scene, Cube* cCube, int idGroupe )
{
    int i =0;

    Objet* objet = (Objet*)malloc( 1 * sizeof( Objet ) );
    Objet_est_un_Cube( objet, cCube );
    g_array_append_val( scene->tObjet, objet );
    scene->nbObjet++;

    Groupe* groupe = Groupe_trouver_ById( scene, idGroupe );

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
    gtk_tree_store_set (scene->store, objet->iter, GROUPE, "Pavé droit", -1);
}

void Scene_ajouter_sphere(Scene* scene, Sphere* sphere, int idGroupe )
{
    Objet* objet = (Objet*)malloc( 1 * sizeof( Objet ) );
    Objet_est_une_Sphere( objet, sphere );
    g_array_append_val( scene->tObjet, objet );
    scene->nbObjet++;

    Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, idGroupe );
    Groupe_ajouter_Objet( groupe, objet );

    gtk_tree_store_append (scene->store, objet->iter, groupe->iter );
    gtk_tree_store_set (scene->store, objet->iter, GROUPE, "Sphère", -1);
}


/**
 * Face enregistrer dans un nouveau tableau GArray alloué sur le tas les adresses des objets dans l'ordre dans lequel ils doivent être dessinés.
 * @param tObjects Les objets à dessiner
 * @return Le tableau des adresses sur objets dans l'ordre
 */
GArray* Scene_objectsOrder( GArray* tObjects, InfoCamera* pCam)
{
	Point sCamPoint;
	GArray* gtOrderedObjects=NULL; /* Tableau des index des faces à dessiner, c'est cette structure qui sera retournée*/
	GArray* gtDistances=NULL; /*Tableau des distance entre la caméra et le centre de gravité de chaque face, servira pour classer les indexs de face*/
	Objet* pObj = NULL;
	int iObjectIndex = 0;
	int iLoopInsert = 0;
	double dDistance= 0.0, dDistanceArray=0.0;

	/*Allocation des GArray */
	gtOrderedObjects = g_array_sized_new(FALSE,TRUE,sizeof(Objet),tObjects->len);
	gtDistances = g_array_sized_new(FALSE,TRUE,sizeof(double),tObjects->len);

	//g_array_insert_val(gtIndexFaces,,iFaceIndex);
	g_array_insert_val(gtDistances,0,dDistanceArray);

	/* Création d'un point ayant pour coordonées le centre du repere de la caméra*/
	Point_init(&sCamPoint, pCam->CoordCam[0], pCam->CoordCam[1], pCam->CoordCam[2]);
	for(iObjectIndex=0;iObjectIndex<tObjects->len;iObjectIndex++) /* Pour chaque objet */
	{
		pObj =  (Objet*)g_array_index(tObjects,Objet*,iObjectIndex);
		/* calcul de la distance entre le centre de gravité de l'objet et le centre de repere de la caméra */
		if( strcmp( pObj->typeObjet, "Cube") == 0 )  /* Si l'objet est un cube */
			dDistance = Point_euclideanDistance( &(pObj->type.cube->Center), &sCamPoint);
		else if(strcmp( pObj->typeObjet, "Rectangle") == 0 )
			dDistance = Point_euclideanDistance( &(pObj->type.rectangle->Center), &sCamPoint);
		else if(strcmp( pObj->typeObjet, "Sphere") == 0 )
			dDistance = Point_euclideanDistance( &(pObj->type.sphere->Center), &sCamPoint);


		/* Insertion là où il faut ! */
		iLoopInsert=0;
		dDistanceArray = g_array_index(gtDistances,double,iLoopInsert);
		/* Insertion de l'index de la face là où il faut*/
		while(dDistanceArray!= 0 && dDistanceArray>dDistance)
		{
			iLoopInsert++;
			dDistanceArray = g_array_index(gtDistances,double,iLoopInsert);
		}

		/*Emplacement d'insertion retrouvé...*/
		g_array_insert_val(gtOrderedObjects,iLoopInsert,pObj);
		g_array_insert_val(gtDistances,iLoopInsert,dDistance);
	}

	g_array_free(gtDistances, TRUE);
	return gtOrderedObjects;
}

/** Fonction qui dessiner tout les objets de la scene
 * @param scene, un pointeur sur une scene initialisée
 * @param cr, un contexte cairo créer sur la zoneDeDessin
 **/
void Scene_dessiner_scene( Scene* scene, cairo_t* cr )
{
    int i = 0;
    GArray* gtObjects = NULL;/* va contenir le tableau des objets dans l'ordre dans lequel il faut les dessiner */
/*
    if( scene->nbObjet != 0 )
    {
    	gtObjects = Scene_objectsOrder(scene->tObjet, scene->camera);

    	for( i = 0; i < scene->nbObjet; i++ )
    	{
    		Objet_dessiner_objet(g_array_index( gtObjects, Objet*, i ) , cr, scene->camera );
    	}

    	g_array_free(gtObjects,TRUE);
    }
    */

    for( i = 0; i < scene->nbObjet; i++ )
	{
		Objet_dessiner_objet(g_array_index( scene->tObjet, Objet*, i ) , cr, scene->camera );
	}
}

/**
 * Fonction qui dessine une scene vierge
 *
 * @param scene, un pointeur sur une scene initialisée
 * @param cr, un contexte cairo créer sur la zoneDeDessin
 **/
void Scene_clear_scene( Scene* scene, cairo_t* cr )
{
     cairo_set_source_rgb( cr, 0.190, 0.190, 0.190 );
     cairo_paint( cr );
}

/** Fonction qui indique si une selection multiple est en cours
 * @param scene, un pointeur sur une scene initialisée
 * @return TRUE si une selection multiple est en cours
 **/
/*gboolean Scene_selection_Multiple( Scene* scene )
{
    int i = 0;

    for( i = 0; i < scene->clavier->nbTouche; i++ )
    {
        printf(" %s \n", g_array_index( scene->clavier->tTouche, char*, i ) );
        if( strcmp( g_array_index( scene->clavier->tTouche, char*, i ), "Shift_L") == 0 )
        {
            return TRUE;
        }
    }

    return FALSE;
}*/

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
