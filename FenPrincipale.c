#include "FenPrincipale.h"

int FenPrincipale_initialiser (int argc, char* argv[] )
 {
    GtkWidget* mainWindow = NULL;                                               // Instantation de la fenetre principal
    GtkWidget* zoneDeDessin = NULL;
    Scene* scene = NULL;

    gtk_init( &argc, &argv );

    mainWindow = gtk_window_new( GTK_WINDOW_TOPLEVEL );                         // appel au constructeur de la fenetre
    zoneDeDessin = gtk_drawing_area_new ();

    scene = (Scene*)malloc( 1 * sizeof( Scene) );
    Scene_initialiser_scene( scene, zoneDeDessin, mainWindow );

    GtkWidget* main_box = gtk_vbox_new( FALSE, 0 );
    GtkWidget* menuBarre = gtk_menu_bar_new();
    gtk_widget_set_size_request( menuBarre, -1, -1 );

//******************Creation de la barre de menu ********************************************
    GtkWidget* menu = gtk_menu_new();
    GtkWidget* fichier = gtk_menu_item_new_with_label( "Fichier" );
    GtkWidget* nouveau = gtk_menu_item_new_with_label( "Nouveau" );
    GtkWidget* ouvrir = gtk_menu_item_new_with_label( "Ouvrir" );
    GtkWidget* sauvegarder = gtk_menu_item_new_with_label( "Sauvegarder" );
    GtkWidget* wExport = gtk_menu_item_new_with_label( "Exporter" );
    GtkWidget* quitter = gtk_menu_item_new_with_label( "Quitter" );
    gtk_menu_item_set_submenu(GTK_MENU_ITEM (fichier), menu);

    GtkWidget* menu2 = gtk_menu_new();
    GtkWidget* edition = gtk_menu_item_new_with_label( "Edition" );
    scene->modification->annuler = gtk_menu_item_new_with_label( "Annuler" );
    scene->modification->refaire = gtk_menu_item_new_with_label( "Refaire" );
    gtk_menu_item_set_submenu(GTK_MENU_ITEM (edition), menu2);

    gtk_menu_attach( GTK_MENU(menu2), scene->modification->annuler, 0, 1, 0, 1 );
    gtk_menu_attach( GTK_MENU(menu2), scene->modification->refaire, 0, 1, 1, 2 );

    gtk_menu_attach( GTK_MENU(menu), nouveau, 0, 1, 0, 1 );
    gtk_menu_attach( GTK_MENU(menu), ouvrir, 0, 1, 1, 2 );
    gtk_menu_attach( GTK_MENU(menu), sauvegarder, 0, 1, 2, 3 );
    gtk_menu_attach( GTK_MENU(menu), wExport, 0, 1, 3, 4 );
    gtk_menu_attach( GTK_MENU(menu), quitter, 0, 1, 4, 5 );
    gtk_menu_bar_append(GTK_MENU_BAR (menuBarre), fichier);
    gtk_menu_bar_append(GTK_MENU_BAR (menuBarre), edition);

//***********************************Ajusteùent de la Zone de dessin************************************
    GdkScreen* screen = NULL;
    screen = gtk_window_get_screen(GTK_WINDOW(mainWindow));
    double width = gdk_screen_get_width(screen);
    double height = gdk_screen_get_height(screen);

    scene->camera = Camera_createCam(width-200,height-75);
    scene->dHeight = height-75;
    scene->dWidth = width-200;

    gtk_widget_set_size_request( zoneDeDessin, width-200, height-75 );
    gtk_container_add( GTK_CONTAINER( mainWindow ), main_box );
//*******************************************************************************************************

    scene->store = gtk_tree_store_new( 1, G_TYPE_STRING );
    Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, 0 );

    gtk_tree_store_append (scene->store, groupe->iter, NULL);
    gtk_tree_store_set (scene->store, groupe->iter, GROUPE, "Groupe 0", -1);

    scene->tree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (scene->store));

    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Groupe", renderer, "text", GROUPE, NULL );
    gtk_tree_view_append_column (GTK_TREE_VIEW (scene->tree), column);
    gtk_tree_view_set_level_indentation( GTK_TREE_VIEW (scene->tree), 0 );

    scene->treeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW( scene->tree ));
    gtk_tree_selection_set_mode( GTK_TREE_SELECTION(scene->treeSelection), GTK_SELECTION_MULTIPLE );

    gtk_widget_set_size_request( scene->tree, 200, height - (height/1.3 ));
//************************** Barre d'outils ***********************************************************
    GtkWidget* hbarre = gtk_hbutton_box_new();
    GtkWidget* vbarre = gtk_vbutton_box_new();

    GtkWidget* boutonMain = gtk_button_new_with_label("Main");
    GtkWidget* imageMain = gtk_image_new_from_file( "main.png" );
    gtk_button_set_image( GTK_BUTTON( boutonMain ), imageMain );


    GtkWidget* boutonMainWorld = gtk_button_new_with_label("MainWorld");
    GtkWidget* imageRotation = gtk_image_new_from_file( "rotation.png" );
    gtk_button_set_image( GTK_BUTTON( boutonMainWorld ), imageRotation );

    GtkWidget* boutonText = gtk_button_new_with_label("Texte");
    GtkWidget* imageText = gtk_image_new_from_file( "texte.png" );
    gtk_button_set_image( GTK_BUTTON( boutonText ), imageText );


    GtkWidget* boutonZoom = gtk_button_new_with_label("Zoom");
    GtkWidget* imageZoom = gtk_image_new_from_file( "loupe.png" );
    gtk_button_set_image( GTK_BUTTON( boutonZoom ), imageZoom );


    GtkWidget* boutonNormal = gtk_button_new_with_label("Normal");
    GtkWidget* imageNormal = gtk_image_new_from_file( "normal.png" );
    gtk_button_set_image( GTK_BUTTON( boutonNormal ), imageNormal );



    gtk_container_add( GTK_CONTAINER( hbarre ), boutonMain );
    gtk_container_add( GTK_CONTAINER( hbarre ), boutonZoom );
    gtk_container_add( GTK_CONTAINER( vbarre ), boutonNormal );
    gtk_container_add( GTK_CONTAINER( vbarre ), boutonMainWorld );
    gtk_container_add( GTK_CONTAINER( vbarre ), boutonText );
    gtk_container_add( GTK_CONTAINER( vbarre ), hbarre );

    gtk_button_box_set_layout( GTK_BUTTON_BOX( hbarre ), GTK_BUTTONBOX_CENTER );
    gtk_button_box_set_layout( GTK_BUTTON_BOX( vbarre ), GTK_BUTTONBOX_START );

//*******************************Layout****************************************************************

    GtkWidget* hbox = gtk_hbox_new( FALSE, 0 );
    GtkWidget* vbox = gtk_vbox_new( FALSE, 0 );
    gtk_container_add( GTK_CONTAINER( vbox ), scene->tree );
    gtk_container_add( GTK_CONTAINER( vbox ), vbarre );

    gtk_container_add( GTK_CONTAINER( hbox ), zoneDeDessin );
    gtk_container_add( GTK_CONTAINER( hbox ), vbox );
    gtk_container_add( GTK_CONTAINER( main_box ), menuBarre );
    gtk_container_add( GTK_CONTAINER( main_box ), hbox );

//*****************************************************************************************************
    gtk_widget_show_all( mainWindow );
    gdk_window_set_cursor( gtk_widget_get_window( scene->zoneDeDessin ), scene->curseur );


    gtk_window_set_title( GTK_WINDOW( mainWindow), "Sch3Dma" );          // Nom totalement provisiore ^^ (mais qui envoie quand même du bois !)

    gtk_widget_add_events( zoneDeDessin, GDK_BUTTON_PRESS_MASK );
    gtk_widget_add_events( zoneDeDessin, GDK_BUTTON_RELEASE_MASK );   //active la detection de la souris
    gtk_widget_add_events( zoneDeDessin, GDK_BUTTON1_MOTION_MASK );
    gtk_widget_add_events( zoneDeDessin, GDK_BUTTON3_MOTION_MASK );
    gtk_widget_add_events( zoneDeDessin, GDK_POINTER_MOTION_HINT_MASK );

    gtk_widget_add_events( scene->tree, GDK_BUTTON_PRESS_MASK );
    g_signal_connect( G_OBJECT( scene->treeSelection ), "changed", G_CALLBACK(selectionChanged), scene );
    g_signal_connect( G_OBJECT( scene->tree ), "button-press-event", G_CALLBACK(clickDroitGroupe), scene );

    g_signal_connect( G_OBJECT( boutonMain ), "clicked", G_CALLBACK( changementCurseur ), scene);
    g_signal_connect( G_OBJECT( boutonMainWorld ), "clicked", G_CALLBACK( changementCurseur ), scene);
    g_signal_connect( G_OBJECT( boutonNormal ), "clicked", G_CALLBACK( changementCurseur ), scene);
    g_signal_connect( G_OBJECT( boutonZoom ), "clicked", G_CALLBACK( changementCurseur ), scene);
    g_signal_connect( G_OBJECT( boutonText ), "clicked", G_CALLBACK( changementCurseur ), scene);

    g_signal_connect( G_OBJECT( mainWindow ), "delete-event", G_CALLBACK( main_quitter ), NULL );
    g_signal_connect( G_OBJECT( mainWindow ), "key-press-event", G_CALLBACK(gestion_clavier), scene);
    g_signal_connect( G_OBJECT( zoneDeDessin ), "realize", G_CALLBACK( realize_callback ), NULL );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "expose-event", G_CALLBACK( expose_event_callback ), scene );
    g_signal_connect( G_OBJECT( mainWindow ), "key-release-event", G_CALLBACK( gestion_clavier ), scene );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "button-press-event", G_CALLBACK( gestion_souris_callback ), scene );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "button-release-event", G_CALLBACK( gestion_souris_callback ), scene );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "motion-notify-event", G_CALLBACK( gestion_souris_callback ), scene );

    g_signal_connect( G_OBJECT( nouveau ), "activate", G_CALLBACK( main_nouveau ), scene);
    g_signal_connect( G_OBJECT( ouvrir ), "activate", G_CALLBACK( main_ouvrir ), scene);
    g_signal_connect( G_OBJECT( sauvegarder ), "activate", G_CALLBACK( main_sauvegarder ), scene);
    g_signal_connect( G_OBJECT( wExport ), "activate", G_CALLBACK( main_export ), scene);
    g_signal_connect( G_OBJECT( quitter ), "activate", G_CALLBACK( main_quitter ), NULL );
    g_signal_connect( G_OBJECT( scene->modification->annuler ), "activate", G_CALLBACK( main_annuler ), scene);
    g_signal_connect( G_OBJECT( scene->modification->refaire ), "activate", G_CALLBACK( main_refaire ), scene);

    gtk_widget_set_sensitive(scene->modification->annuler, FALSE);
    gtk_widget_set_sensitive(scene->modification->refaire, FALSE);

    gtk_window_maximize( GTK_WINDOW( mainWindow ) );

    gtk_main();

    Modification_detruire_temporaire( scene->modification );
    Scene_detruire( scene );
    Clavier_detruire( scene->clavier );
    free( scene->selection );
    free( scene->clavier );
    free( scene->creation );
    free( scene->modification );

    int i =0;
    for( i =0; i < scene->nbGroupe; i++)
    {
        Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, i );
        Groupe_detruire( groupe );
        free( groupe );
    }
    g_array_free( scene->tGroupe, FALSE );
    free( scene->camera );
    free( scene );

    return EXIT_SUCCESS;
 }

/** Fonction dessinant la scene, partie principale du programme, gérant aussi le dessin du rectangle de selection
 * @param widget, la fenetre contenant la zone de dessin à dessiner
 * @param event, l'evenement associer à l'appel du CallBack, ici inutiliser contenant "expose-event"
 * @param data, pointer générique contenant la scene qui elle contient toutes les informations necessaire au dessin
 * @return TRUE pour que l'expose event soit propagé dans l'application
 */
static gboolean expose_event_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    Scene* scene = (Scene*)data;                                     //Cast du pointeur generique en Scene*

    cairo_t* cr = gdk_cairo_create( widget->window );               //Creation du contexte pour pouvoir dessiner

    Scene_clear_scene( scene , cr );                                 //Nettoyage de la scene

    Scene_dessiner_scene( scene, cr );                               //On dessine tous les objets

    Selection_dessiner_rectangle( scene->selection, cr );            //Si selection de zone en cours, on dessine le rectangle de selection

    cairo_destroy( cr );                                             //Destruction du contexte

    return TRUE;
}

static gboolean realize_callback( GtkWidget *widget, GdkEventExpose *event, gpointer data )
{
    printf("realize event \n");
    return TRUE;
}

/** Fonction gérant les entrées clavier, associant aux touches/combinaison de touches les fonctions associées
 * @param window, le widget ayant reçu l'evenement "key-press-event" ou "key-release-event"
 * @param event, le type d'evenement ici deux evenements nous interessent : "key-press-event" ou "key-release-event"
 * @param data, pointeur générique sur la scene, qui contient toutes les informations sur l'etat du clavier
 * @return TRUE pour propager les evenements dans l'application
 */
static gboolean gestion_clavier(GtkWidget *window, GdkEventKey* event, gpointer data)
{
    Scene* scene = (Scene*) data;                                                               //Cast du pointeur generique en Scene*

    if( event->type == GDK_KEY_PRESS )
    {
        Clavier_touche_appuyer( scene, gdk_keyval_name(event->keyval) );                        //On stocke toutes les touches qui ont ete appuyés et non relachés

        if( strcmp( gdk_keyval_name(event->keyval), "Right") == 0 )
        {
            // Fonction de rotation scene/objet
        	//Cube_rotateCube(g_array_index(scene->tObjet,objet*,0)->type.Cube,0.0,1.47,0.0);
            gtk_widget_queue_draw( window );
        }
        else if( strcmp( gdk_keyval_name(event->keyval), "Shift_L") == 0 )
        {
            scene->selection->selection_multiple = TRUE;
        }
        else if( strcmp( gdk_keyval_name(event->keyval), "a") == 0 )
        {
            if( Clavier_est_appuyer( scene, "Control_L" ) )
            {
                Selection_selectionner_tout( scene );                                           //Raccourci Ctrl_L + a = selectionner tout
            }
            gtk_widget_queue_draw( window );
        }
        else if( strcmp( gdk_keyval_name(event->keyval), "o") == 0 )
        {
            if( Clavier_est_appuyer( scene, "Control_L" ) )
            {
                main_ouvrir( NULL, scene );
            }
        }
        else if( strcmp( gdk_keyval_name(event->keyval), "s") == 0 )
        {
            if( Clavier_est_appuyer( scene, "Control_L" ) )
            {
                main_sauvegarder( NULL, scene );
            }
        }
        else if( strcmp( gdk_keyval_name(event->keyval), "Escape") == 0 )
        {
            Selection_deselectionner_tout( scene );
                                                //Echap = Tout deselectionner
            gtk_widget_queue_draw( window );
        }
        else if( strcmp( gdk_keyval_name( event->keyval), "z" ) == 0 || strcmp( gdk_keyval_name( event->keyval), "Z" ) == 0 )
        {
            if( Clavier_est_appuyer( scene, "Control_L" ) && !Clavier_est_appuyer( scene, "Shift_L") && gtk_widget_get_sensitive( scene->modification->annuler ) )
            {
                Modification_annuler( scene );                                                  //Ctrl_L + z = annuler si annuler disponible
            }
            else if( Clavier_est_appuyer( scene, "Control_L") && Clavier_est_appuyer( scene, "Shift_L") && gtk_widget_get_sensitive( scene->modification->refaire ) )
            {
                Modification_refaire( scene );
            }
            gtk_widget_queue_draw( window );
        }
    }
    else if( event->type == GDK_KEY_RELEASE )
    {
        Clavier_touche_relacher( scene, gdk_keyval_name(event->keyval) );                       //On eneleve les touches relachées

        if( strcmp( gdk_keyval_name(event->keyval), "Shift_L") == 0 )
        {
            scene->selection->selection_multiple = FALSE;
        }
    }
    return TRUE;
}

static gboolean newText(gpointer data)
{
    Scene* scene = (Scene*)data;

	FenText* ft = (FenText*)malloc( 1 * sizeof( FenText ) );
	FenText_init(ft, scene);

	return TRUE;
}

static gboolean main_export(GtkWidget *menuItem, gpointer data )
{
	Scene* scene = (Scene*)data;

	ExportWindow* pEw = (ExportWindow*)malloc( 1 * sizeof( ExportWindow ) );
	ExportWindow_init(pEw, scene);
	return TRUE;
}

/** Fonction gérant les clics/deplacements souris, associant aux clics les differentes fonctions
 * @param window, le widget ayant reçu l'evenement
 * @param event, le type d'evenement ici plusieurs evenements nous interessent : "button-press-event", "button-release-event", "motion-notify-event"
 * @param data, pointeur générique sur la scene, qui contient toutes les informations sur l'etat de la souris
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean gestion_souris_callback(GtkWidget *widget, GdkEventButton* event, gpointer data)
{
	int i,j;
	Objet* pObj; /* Va pointer sur les objets d'un groupe */
    Scene* scene = (Scene*)data;
	tdMatrix tdTransfoMat; /* Matrix de transformation */
	tdMatrix tdNewTransfo; /* Matrix utilisée dans le cas de transformations multiples */
	Groupe* pSon; /* Pointeur vers un fils de groupe */
	Groupe* pGroupe; /* Pointeur vers un groupe sleectionné par l'utilisateur */

    if( scene->souris == NORMAL )
    {
        if( event->type == GDK_BUTTON_PRESS && event->button == 1 )
        {
            scene->selection->departSelection.x = event->x;                                       //Click gauche, on stocke le point clické au cas ou l'evenement soit suivi d'un motion-notify-event
            scene->selection->departSelection.y = event->y;
            Selection_selectionner_objet( scene, event->x, event->y );                            //On regarde si le click ne dois pas selectionner un objet

            gtk_widget_queue_draw( widget );
        }
        else if( event->type == GDK_BUTTON_PRESS && event->button == 3 )                          //Click droit on affiche le menu contextuel
        {
            GtkWidget *menu = gtk_menu_new();
            GtkWidget *pItem = gtk_menu_item_new_with_label("Nouvel objet");

            GtkWidget *pItem2 = gtk_menu_item_new_with_label("Propriete");
            GtkWidget *pItem3 = gtk_menu_item_new_with_label("Supprimer");
            GtkWidget *pItem4 = gtk_menu_item_new_with_label("Nouveau Groupe");
            GtkWidget *pItem5 = gtk_menu_item_new_with_label("Transformation");

            gtk_menu_attach( GTK_MENU(menu), pItem, 0, 1, 0, 1 );
            gtk_menu_attach( GTK_MENU(menu), pItem2, 0, 1, 1, 2 );
            gtk_menu_attach( GTK_MENU(menu), pItem3, 0, 1, 2, 3 );
            gtk_menu_attach( GTK_MENU(menu), pItem4, 0, 1, 3, 4 );
            gtk_menu_attach( GTK_MENU(menu), pItem5, 0, 1, 4, 5 );

            gtk_widget_show_all(menu);

            /* Modif des coordonnées du pixel sur lequel l'utilisateur a cliqué pour créer son objet*/
            Scene_creation_objet( scene, event->x, event->y );

            gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event->button, event->time);

            /*Mise en place des signaux*/
            g_signal_connect( G_OBJECT( pItem ), "activate", G_CALLBACK(nouveau_cube), scene);
            g_signal_connect( G_OBJECT( pItem3 ), "activate", G_CALLBACK(main_supprimer), scene);
            g_signal_connect( G_OBJECT( pItem2 ), "activate", G_CALLBACK(nouveau_propriete), scene);
            g_signal_connect( G_OBJECT( pItem4 ), "activate", G_CALLBACK(nouveau_groupe), scene);
            g_signal_connect( G_OBJECT( pItem5 ), "activate", G_CALLBACK(nouvelle_transformation), scene);

            gboolean temp = scene->selection->selection_multiple;
            scene->selection->selection_multiple = TRUE;
            Selection_selectionner_objet( scene, event->x, event->y );
            scene->selection->selection_multiple = temp;
            gtk_widget_queue_draw( widget );

        }
        else if( event->type == GDK_MOTION_NOTIFY && event->button == 1)                                                 //Prob lag normalement resolu grace à GDK_POINTER_MOTION_HINT_MASK
        {
            scene->selection->finSelection.x = event->x;                                            //Motion-notify-event on stocke le point d'arrive pour dessiner le rectangle
            scene->selection->finSelection.y = event->y;
            Selection_selectionner_click_drag( scene );
            scene->selection->selection_en_cours = TRUE;

            scene->curseur = gdk_cursor_new_from_pixbuf( gdk_display_get_default(), gtk_image_get_pixbuf( GTK_IMAGE( gtk_image_new_from_file( "rectangle.png") ) ), 12, 12 );
            gdk_window_set_cursor( gtk_widget_get_window( scene->zoneDeDessin ), scene->curseur );

            gtk_widget_queue_draw( widget );
        }
        else if( event->type == GDK_BUTTON_RELEASE && event->button == 1 )
        {
            scene->selection->selection_en_cours = FALSE;

            scene->curseur = gdk_cursor_new_from_pixbuf( gdk_display_get_default(), gtk_image_get_pixbuf( GTK_IMAGE( gtk_image_new_from_file( "normal.png") ) ), 4, 4 );
            gdk_window_set_cursor( gtk_widget_get_window( scene->zoneDeDessin ), scene->curseur );

            gtk_widget_queue_draw( widget );
        }
    }
    else if( scene->souris == MAIN )
    {
        if(event->type == GDK_MOTION_NOTIFY && event->button == 1 )
        {
        	int i = 0;
			tCoord2D tMove;  /* Mouvement = différence entre l'endroit ou l'utilisateur a commencé son clique et la position actuelle du curseur */
			Point_initCoord2D(tMove, scene->rotation.x-event->x, scene->rotation.y-event->y);
			Matrix_initIdentityMatrix(tdTransfoMat); /* Initialisation de la matrice de rotation */
			if(tMove[0] != 0)
			{
				Transformation_getMatrixRotation(tdNewTransfo, tMove[1]/200, AXEX);
				Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
			}

			if(tMove[1] != 0)
			{
				Transformation_getMatrixRotation(tdNewTransfo,tMove[0]/200, AXEY);
				Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
			}


			for( i = 0; i < scene->nbGroupe; i++ )
			{
				pGroupe = g_array_index( scene->tGroupe, Groupe*, i ); /*Récupération du groupe en question*/

				if( gtk_tree_selection_iter_is_selected( scene->treeSelection, pGroupe->iter ) ) /* On test si le gorupe est selectionné*/
				{
					if( !pGroupe->bVisited)
					{
						for( j=0;j<pGroupe->tFils->len;++j) /* Modification uniquement du centre des groupes fils */
						{
							pSon = g_array_index(pGroupe->tFils,Groupe*,j);
							Groupe_transfoCenter( pSon, tdTransfoMat);
						}

						/* MOdification sur les objets */
						for( j=0;j<pGroupe->tObjet->len;++j)
						{
							pObj = g_array_index(pGroupe->tObjet,Objet*,j);
							Objet_transfoCenter(pObj, tdTransfoMat);
						}
					}
				}
			}

			gboolean bSelected = FALSE;
			/* On considère tous les objets, si un objet est selectionné et que nous n'avons pas déjà appliqué la transfo on applique la rotation*/
			for(i=0; i<scene->tObjet->len;++i)
			{
				pObj = g_array_index(scene->tObjet,Objet*,i);
				bSelected = FALSE;
				if( pObj->eType == CUBE && pObj->type.cube->estSelectionne)
					bSelected = TRUE;
				else if( pObj->eType == RECTANGLE && pObj->type.rectangle->estSelectionne)
					bSelected = TRUE;
				else if( pObj->eType == SPHERE && pObj->type.sphere->estSelectionne)
					bSelected = TRUE;
				else if( pObj->eType == TRIANGLE && pObj->type.triangle->estSelectionne)
					bSelected = TRUE;
				else if( pObj->eType == QUADRILATERAL && pObj->type.quadrilateral->estSelectionne)
					bSelected = TRUE;

				if( bSelected && !pObj->pFatherGroup->bVisited) /* Si l'objet est selectionné et qu'il n'est pas dans un groupe qui a été visité */
				{
					/* On applique la rotation à l'objet*/
					Objet_transfo(pObj,tdTransfoMat);
				}
			}

			/*Une fois que tous les groupes sont visités on remet leur flag bVisited à false pour une prochaine utilisation*/
			for( i = 0; i < scene->nbGroupe; i++ )
			{
				pGroupe = g_array_index( scene->tGroupe, Groupe*, i ); /*Récupération du groupe en question*/
				Groupe_unvisit(pGroupe);
			}
			scene->rotation.x = event->x;
			scene->rotation.y = event->y;

			gtk_widget_queue_draw( widget );

        }
        else if( event->type == GDK_BUTTON_PRESS && event->button == 1 )                          //Click droit on affiche le menu contextuel
        {
            scene->rotation.x = event->x;
            scene->rotation.y = event->y;

            scene->curseur = gdk_cursor_new_from_pixbuf( gdk_display_get_default(), gtk_image_get_pixbuf( GTK_IMAGE( gtk_image_new_from_file( "main_fermer.png") ) ), 4, 4 );
            gdk_window_set_cursor( gtk_widget_get_window( scene->zoneDeDessin ), scene->curseur );
        }
        else if( event->type == GDK_BUTTON_RELEASE && event->button == 1 )
        {
            scene->curseur = gdk_cursor_new_from_pixbuf( gdk_display_get_default(), gtk_image_get_pixbuf( GTK_IMAGE( gtk_image_new_from_file( "main.png") ) ), 4, 4 );
            gdk_window_set_cursor( gtk_widget_get_window( scene->zoneDeDessin ), scene->curseur );
        }
    }
    else if( scene->souris == MAINWORLD )
	{
		if(event->type == GDK_MOTION_NOTIFY && event->button == 1 )
		{
			tCoord2D tMove;

			tMove[0] = scene->rotation.x - event->x;
			tMove[1] = scene->rotation.y - event->y;

			Matrix_initIdentityMatrix(tdTransfoMat); /* Initialisation de la matrice de rotation */
			/* On recherche la matrice de rotation qui va bien */
			if(tMove[0] != 0)
			{
				Transformation_getMatrixRotation(tdNewTransfo, tMove[1]/200, AXEX);
				Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
			}

			if(tMove[1] != 0)
			{
				Transformation_getMatrixRotation(tdNewTransfo,tMove[0]/200, AXEY);
				Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
			}

			/* On passe en revue tous les groupes de notre scene */
			for( i = 0; i < scene->nbGroupe; i++ )
			{
				pGroupe = g_array_index( scene->tGroupe, Groupe*, i ); /*Récupération du groupe en question*/

				if( gtk_tree_selection_iter_is_selected( scene->treeSelection, pGroupe->iter ) ) /* On test si le gorupe est selectionné*/
				{
					if( !pGroupe->bVisited)
					{
						/* TRANSFORMATIIIOOOONNN RECURSIVE !!! */
						/* On applique la transfo pour tous les groupes fils */
						for( j=0;j<pGroupe->tFils->len;++j)
						{
							pSon = g_array_index(pGroupe->tFils,Groupe*,j);
							Groupe_transfo( pSon, tdTransfoMat);
						}

						/* et pour les objets du groupe */
						for( j=0;j<pGroupe->tObjet->len;++j)
						{
							pObj = g_array_index(pGroupe->tObjet,Objet*,j);
							Objet_transfoCenter(pObj, tdTransfoMat);
							Objet_transfo( pObj , tdTransfoMat);
						}
					}
				}
			}

			gboolean bSelected = FALSE;
			/* On considère tous les objets, si un objet est selectionné et que nous n'avons pas déjà appliqué la transfo on applique la rotation*/
			for(i=0; i<scene->tObjet->len;++i)
			{
				pObj = g_array_index(scene->tObjet,Objet*,i);
				bSelected = FALSE;
				if( pObj->eType == CUBE && pObj->type.cube->estSelectionne)
					bSelected = TRUE;
				else if( pObj->eType == RECTANGLE && pObj->type.rectangle->estSelectionne)
					bSelected = TRUE;
				else if( pObj->eType == SPHERE && pObj->type.sphere->estSelectionne)
					bSelected = TRUE;
				else if( pObj->eType == TRIANGLE && pObj->type.triangle->estSelectionne)
					bSelected = TRUE;
				else if( pObj->eType == QUADRILATERAL && pObj->type.quadrilateral->estSelectionne)
					bSelected = TRUE;

				if( bSelected && !pObj->pFatherGroup->bVisited) /* Si l'objet est selectionné et qu'il n'est pas dans un groupe qui a été visité */
				{
					/* On applique la rotation à l'objet*/
					Objet_transfoCenter(pObj,tdTransfoMat);
				}
			}

			/*Une fois que tous les groupes sont visités on remet leur flag bVisited à false pour une prochaine utilisation*/
			for( i = 0; i < scene->nbGroupe; i++ )
			{
				pGroupe = g_array_index( scene->tGroupe, Groupe*, i ); /*Récupération du groupe en question*/
				Groupe_unvisit(pGroupe);
			}
			scene->rotation.x = event->x;
			scene->rotation.y = event->y;

			gtk_widget_queue_draw( widget );
		}
		else if( event->type == GDK_BUTTON_PRESS && event->button == 1 )                          //Click droit on affiche le menu contextuel
		{
			scene->rotation.x = event->x;
			scene->rotation.y = event->y;
		}
	}
    else if( scene->souris == LOUPE )
    {
        if( event->type == GDK_MOTION_NOTIFY && event->button == 1 )
        {
        	Camera_setCoordCam(scene->camera, (scene->camera->CoordCam)[0], (scene->camera->CoordCam)[1],
									(scene->camera->CoordCam)[2]+(scene->rotation.tdCoordWorld[1]-event->y));
        	gtk_widget_queue_draw( scene->zoneDeDessin );
        }
        else if( event->type == GDK_BUTTON_PRESS && event->button == 1 )                          //Click droit on affiche le menu contextuel
		{
        	Point_initCoord2D(scene->rotation.tdCoordWorld, event->x, event->y);
				/* TODO : modifier le nom de la variable rotation*/
		}

        if( event->type == GDK_BUTTON_PRESS && event->button == 3 )                          //Click droit on affiche le menu contextuel
        {
            int i = 0;

            for( i = 0; i < scene->selection->nbSelection; i++ )
            {
                Objet* objet = g_array_index( scene->selection->tSelection, Objet*, i );

                Objet_homothetie( objet, -2 );
                gtk_widget_queue_draw( widget );
            }
        }
    }
    else if( scene->souris == TEXT )
    {
        if( event->type == GDK_BUTTON_PRESS && event->button == 1 ) /* Clique gauche utilisateur */
        {
            /* Création fenetre de création de texte */
            newText(scene);
        }
        if( event->type == GDK_BUTTON_PRESS && event->button == 3 )                          //Click droit on affiche le menu contextuel
        {
            int i = 0;

            for( i = 0; i < scene->selection->nbSelection; i++ )
            {
                Objet* objet = g_array_index( scene->selection->tSelection, Objet*, i );

                Objet_homothetie( objet, -2 );
                gtk_widget_queue_draw( widget );
            }
        }
    }
    return TRUE;
}

/** Fonction créant la fenetre d'ajout d'un nouvel objet dans la scene
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, pointeur générique sur la scene, qui contient la represantation de la scene a laquelle il faudra ajouter le nouvel objet
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean nouveau_cube( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data; /*recupération de la scene courante*/
    FenetreAjout* fao = (FenetreAjout*)malloc( 1 *sizeof( FenetreAjout) );
    initialiser_FenetreAjout( fao, scene );/* création de la fenêtre*/

    return TRUE;
}

static gboolean nouveau_rectangle( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;
    FenetreAjout* fao = (FenetreAjout*)malloc( 1 *sizeof( FenetreAjout) );
    initialiser_FenetreAjout( fao, scene );/* création de la fenêtre*/

    return TRUE;
}

/** Fonction gérant l'ouverture d'un fichier de sauvegarde
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, pointeur générique sur la scene, qui sera reecrite selon le ficier de sauvegarde lu
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean main_ouvrir( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;
    Scene_reset( scene, scene->zoneDeDessin );

    GtkWidget* opener = gtk_file_chooser_dialog_new ("Ouvrir un fichier de sauvegarde ...", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

    GtkFileFilter* filtre = gtk_file_filter_new();
    gtk_file_filter_add_pattern( GTK_FILE_FILTER (filtre), "*.txt");
    gtk_file_chooser_set_filter( GTK_FILE_CHOOSER(opener), GTK_FILE_FILTER(filtre) );

    if (gtk_dialog_run (GTK_DIALOG (opener)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (opener));

        FILE* fichier = fopen( filename, "r+");
        g_free (filename);

        if( fichier != NULL )
        {
            char deb[5] = "    ";
            fscanf( fichier, "%s", deb );

            if( strcmp( deb, "DEBUT" ) == 0 )
            {
                int i = 0;
                int nbGroupe = 0;
                int nbObjet = 0;
                fscanf( fichier, "%d", &nbGroupe );

                for( i = 0; i < nbGroupe; i++ )
                {
                    Groupe_restaure( fichier, scene );
                }

                fscanf( fichier, "%d", &nbObjet );

                for( i = 0; i < nbObjet; i++ )
                {
                    Objet_restaure( fichier, scene );
                }
            }
             else
            {
                GtkWidget* avertissement =
                gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Le fichier n'est pas un fichier de sauvegarde Sch3Dma" );

                if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
                {
                    gtk_widget_destroy( avertissement );

                }
            }
            fclose( fichier );
        }
        else
        {
            GtkWidget* avertissement =
            gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Le fichier n'a pas pu etre ouvert" );

            if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
            {
                gtk_widget_destroy( avertissement );

            }
        }
    }
    gtk_widget_destroy (opener);
    gtk_widget_queue_draw( scene->zoneDeDessin );
    return TRUE;
}

/** Fonction gérant la sauvegarde d'une scene
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, pointeur générique sur la scene, qui sera lu et ecrite dans un fichier
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean main_sauvegarder( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;

    if( scene->nbObjet > 0 )
    {
        GtkWidget *dialog = gtk_file_chooser_dialog_new ("Sauvegarder la scene", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
        gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
        GtkFileFilter* filtre = gtk_file_filter_new();
        gtk_file_filter_add_pattern( GTK_FILE_FILTER (filtre), "*.txt");
        gtk_file_chooser_set_filter( GTK_FILE_CHOOSER(dialog), GTK_FILE_FILTER(filtre) );

        if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
        {
            char *filename;
            FILE* fichier = NULL;
            filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
            filename = (char*)realloc( filename, ( strlen(filename) + 4 ) * sizeof( char ) );
            filename = strcat( filename, ".txt" );

            fichier = fopen( filename, "w+" );
            g_free (filename);

            if( fichier != NULL )
            {
                int i = 0;
                fprintf( fichier, "DEBUT\n");
                fprintf( fichier, "%d\n", scene->nbGroupe-1 );

                for( i = 1; i < scene->nbGroupe; i++ )
                {
                    Groupe* groupe = (Groupe*)g_array_index( scene->tGroupe, Groupe*, i );
                    Groupe_sauvegarde( groupe, fichier );
                }

                fprintf( fichier, "%d\n", scene->nbObjet );

                for( i = 0; i < scene->nbObjet; i++ )
                {
                    Objet* objet = (Objet*)g_array_index( scene->tObjet, Objet*, i );
                    Objet_sauvegarde( objet, fichier );
                }
                fprintf( fichier, "FIN");
                fclose( fichier );
            }

        }
        gtk_widget_destroy (dialog);

    }
    else
    {
        GtkWidget* avertissement =
        gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "La scene est vide sauvegarde inutile !" );

        if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
        {
            gtk_widget_destroy( avertissement );

        }
    }
    return TRUE;
}

/** Fonction gérant le click sur l'element annuler
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, pointeur générique sur la scene, qui contient un pointeur sur le module Modification
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean main_annuler( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;

    Modification_annuler( scene );

    return TRUE;
}

/** Fonction gérant le click sur l'element refaire
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, pointeur générique sur la scene, qui contient un pointeur sur le module Modification
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean main_refaire( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;

    Modification_refaire( scene );

    return TRUE;
}

/** Fonction gérant la destruction de la fenetre principale
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, NULL
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean main_quitter( GtkWidget *menuItem, gpointer data )
{
    GtkWidget* avertissement =
    gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, "Etes-vous sur de vouloir quitter ? Tout travail non sauvegardé sera perdu !" );

    if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
    {
        gtk_widget_destroy( avertissement );
        gtk_main_quit();
    }
    else
    {
        gtk_widget_destroy( avertissement );
    }
    return TRUE;
}

/** Fonction gérant la demande d'une nouvelle scene
 * @warning l'ancienne scene sera detruite, à l'exception de la zone de dessin qui sera conservé
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, pointeur générique sur la scene, qui sera detruite et reconstruite vierge
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean main_nouveau( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;
    GtkWidget* avertissement =
    gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, "Tout travail non sauvegardé sera perdu ! Etes-vous sur de vouloir continuer ?" );

    if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
    {
        gtk_widget_destroy( avertissement );
        Scene_reset( scene, scene->zoneDeDessin );
        gtk_widget_queue_draw( scene->zoneDeDessin );
    }
    else
    {
        gtk_widget_destroy( avertissement );
    }
    return TRUE;
}


/** Fonction gérant l'appel à la suppression d'éléments
 *  @param menuItem, le menu ayant ete cliqué
 *  @param data, la scene contenant le/les elements à supprimer
 *  @return TRUE pour propager les evenements dans l'application
 **/
static gboolean main_supprimer( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;

    int i = 0;
    int nb = scene->selection->nbSelection;

    for( i = 0; i < nb; i++ )
    {
        Objet* objet = g_array_index( scene->selection->tSelection, Objet*, 0 );
        Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, objet->numeroGroupe );

        Groupe_enlever_objet( groupe, objet );
        gtk_tree_selection_unselect_iter( scene->treeSelection, objet->iter );
        gtk_tree_store_remove( scene->store, objet->iter );

        Selection_deselectionner( scene, objet );
        Scene_enlever_objet( scene, objet );
    }

    Modification_modification_effectuer( scene );

    gtk_widget_queue_draw( scene->zoneDeDessin );
    gtk_widget_queue_draw( scene->tree );

    return TRUE;
}

/** Fonction qui créer une nouvelle fenetre de propriete pour un objet
  * @param menuItem, l'element du menu ayant ete cliqué
  * @param data, la scene
  */
static gboolean nouveau_propriete( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;

    if( scene->selection->nbSelection == 1 )
    {
        FenetrePropriete* fp = (FenetrePropriete*)malloc( 1 * sizeof( FenetrePropriete ) );
        FenetrePropriete_initialiser( fp, scene );
    }
    return TRUE;
}


//static gboolean clickZoneGroupe(GtkWidget *widget, GdkEventButton* event, gpointer data)
/** Fonction synchronisant la selection d'elements dans le volet groupe et la scene */
static gboolean selectionChanged(GtkTreeSelection *selection, gpointer data)
{
    Scene* scene = (Scene*)data;
    gboolean modif = FALSE;

    int i = 0;

    for( i = 0; i < scene->nbGroupe; i++ )                                  // Test si un groupe est selectionné
    {
        Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, i );

        if( gtk_tree_selection_iter_is_selected( scene->treeSelection, groupe->iter ) )
        {
            int j = 0;

            for( j = 0; j < groupe->nbObjet; j++ )
            {
                Objet* objet = g_array_index( groupe->tObjet, Objet*, j );
                gtk_tree_selection_select_iter( scene->treeSelection, objet->iter );
                modif = TRUE;
            }
        }
    }

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet* objet = g_array_index( scene->tObjet, Objet*, i );

        if( gtk_tree_selection_iter_is_selected( scene->treeSelection, objet->iter ) )
        {
            Selection_selectionner( scene, objet );
            modif = TRUE;
        }
        else
        {
            Selection_deselectionner( scene, objet );
        }
    }

    gtk_widget_queue_draw( scene->zoneDeDessin );
    gtk_widget_queue_draw( scene->tree );
    return FALSE;
}

static gboolean clickDroitGroupe(GtkWidget *window, GdkEventButton* event, gpointer data)
{
     Scene* scene = (Scene*)data;

     if(event->type == GDK_BUTTON_PRESS && event->button == 3 )                          //Click droit on affiche le menu contextuel
     {
         GtkWidget *menu = gtk_menu_new();
         GtkWidget *pItem = gtk_menu_item_new_with_label("Ajouter un groupe");
         GtkWidget *pItem2 = gtk_menu_item_new_with_label("Supprimer un groupe");
         GtkWidget *pItem3 = gtk_menu_item_new_with_label("Appliquer une transformation");

         gtk_menu_attach( GTK_MENU(menu), pItem, 0, 1, 0, 1 );
         gtk_menu_attach( GTK_MENU(menu), pItem2, 0, 1, 1, 2 );
         gtk_menu_attach( GTK_MENU(menu), pItem3, 0, 1, 2, 3 );

         gtk_widget_show_all(menu);

         gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event->button, event->time);

         /*Mise en place des signaux*/
         g_signal_connect( G_OBJECT( pItem ), "activate", G_CALLBACK(nouveau_groupe), scene );
         g_signal_connect( G_OBJECT( pItem2 ), "activate", G_CALLBACK( supprimer_Groupe), scene );
         g_signal_connect( G_OBJECT( pItem3 ), "activate", G_CALLBACK( nouvelle_transformation ), scene );
         gtk_widget_queue_draw( window );

     }
     return FALSE;
}

static gboolean nouvelle_transformation( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;

    if( scene->selection->nbSelection > 0 )
    {
        FenTrans* ft = (FenTrans*)malloc( 1 * sizeof( FenTrans ) );
        FenTrans_init( ft, scene );
    }
	return TRUE;
}

static gboolean nouveau_groupe(GtkWidget *menuItem, gpointer data )
{
     Scene* scene = (Scene*)data;

     GtkWidget* fenetre = gtk_window_new( GTK_WINDOW_TOPLEVEL );

     gtk_window_set_position( GTK_WINDOW( fenetre ), GTK_WIN_POS_CENTER );
     gtk_window_set_modal( GTK_WINDOW( fenetre ), TRUE );
     gtk_window_set_title( GTK_WINDOW( fenetre ), "Création d'un nouveau Groupe" );

     GtkWidget* text = gtk_label_new("Groupe Père :");
     GtkWidget* text2 = gtk_label_new("Nom du groupe :");

     scene->CBajoutGroupe = gtk_combo_box_text_new();
     scene->entryNom = gtk_entry_new();

     int i = 0;

     for( i = 0; i < scene->nbGroupe; i++ )
     {
         Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, i );

         gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( scene->CBajoutGroupe ), groupe->nom );
     }

     gtk_widget_set_size_request( GTK_WIDGET( scene->CBajoutGroupe ), 100, -1 );

     GtkWidget* hbox = gtk_hbox_new( FALSE, 10 );
     GtkWidget* hbox1 = gtk_hbox_new( FALSE, 10 );

     gtk_container_add( GTK_CONTAINER( hbox ), text );
     gtk_container_add( GTK_CONTAINER( hbox ), scene->CBajoutGroupe );

     gtk_container_add( GTK_CONTAINER( hbox1 ), text2 );
     gtk_container_add( GTK_CONTAINER( hbox1 ), scene->entryNom );

     GtkWidget* boutonOk = gtk_button_new_with_label("OK");
     GtkWidget* boutonAnnuler = gtk_button_new_with_label("Annuler");

     GtkWidget* barreBouton = gtk_hbutton_box_new();
     gtk_button_box_set_layout( GTK_BUTTON_BOX( barreBouton ), GTK_BUTTONBOX_END );

      //******** Barre Position ************************************

    scene->entry1 = gtk_entry_new();
    scene->entry2 = gtk_entry_new();
    scene->entry3 = gtk_entry_new();

    gtk_widget_set_size_request(scene->entry1, 50, -1 );
    gtk_widget_set_size_request(scene->entry2, 50, -1 );
    gtk_widget_set_size_request(scene->entry3, 50, -1 );

    GtkWidget* text1 = gtk_label_new("Position du centre X Y Z :");

    GtkWidget* barrePosition = gtk_hbox_new( 5, TRUE );

    gtk_container_add( GTK_CONTAINER( barrePosition ), text1 );
    gtk_container_add( GTK_CONTAINER( barrePosition ), scene->entry1 );
    gtk_entry_set_text( GTK_ENTRY( scene->entry1 ), "0" );

    gtk_container_add( GTK_CONTAINER( barrePosition ), scene->entry2 );
    gtk_entry_set_text( GTK_ENTRY( scene->entry2 ), "0" );

    gtk_container_add( GTK_CONTAINER( barrePosition ), scene->entry3 );
    gtk_entry_set_text( GTK_ENTRY( scene->entry3 ), "0" );

//*********************************************************************
     gtk_container_add( GTK_CONTAINER( barreBouton ), boutonOk );
     gtk_container_add( GTK_CONTAINER( barreBouton ), boutonAnnuler );

     GtkWidget* vbox = gtk_vbox_new( FALSE, 10 );
     gtk_container_add( GTK_CONTAINER( vbox ), hbox );
     gtk_container_add( GTK_CONTAINER( vbox ), hbox1 );
     gtk_container_add( GTK_CONTAINER( vbox ), barrePosition );
     gtk_container_add( GTK_CONTAINER( vbox ), barreBouton );

     gtk_container_add( GTK_CONTAINER( fenetre ), vbox );

     gtk_combo_box_set_active( GTK_COMBO_BOX( scene->CBajoutGroupe ), 0 );

     gtk_widget_show_all(fenetre);

     scene->fenetre = fenetre;

     g_signal_connect_object( G_OBJECT( boutonAnnuler ), "clicked", G_CALLBACK( gtk_widget_destroy ), fenetre, G_CONNECT_SWAPPED );
     g_signal_connect( G_OBJECT( boutonOk ), "clicked", G_CALLBACK( ajout_Groupe ), scene );

    return TRUE;
}

static gboolean ajout_Groupe( GtkButton* button, gpointer data )
{
    Scene* scene = (Scene*)data;

    Groupe* pere = Groupe_trouver( scene, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(scene->CBajoutGroupe) ) ) ;

    if( pere != NULL )
    {
        int max = 1;
        int i = 0;

        for( i = 0; i < scene->nbGroupe; i++ )
        {
            Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, i );

            if( groupe->id >= max )
            {
                max = groupe->id +1 ;
            }
        }

         double dX, dY, dZ = 0;
        //Coordonnées de création de l'objet
        dX = atof( gtk_entry_get_text( GTK_ENTRY( scene->entry1) ) );
        dY = atof( gtk_entry_get_text( GTK_ENTRY( scene->entry2) ) );
        dZ = atof( gtk_entry_get_text( GTK_ENTRY( scene->entry3) ) );

        Groupe* fils = (Groupe*)malloc( 1 * sizeof( Groupe ) );
        Groupe_initialiser(fils, pere, max, dX, dY, dZ );

        if( strcmp( gtk_entry_get_text( GTK_ENTRY(scene->entryNom) ), "" ) != 0 )
        {
            GtkWidget* label = gtk_label_new( gtk_entry_get_text( GTK_ENTRY(scene->entryNom) ) );
            fils->nom = (char*)gtk_label_get_text( GTK_LABEL(label) );
        }
        else
        {
            GtkWidget* label = gtk_label_new( "Anonyme" );
            fils->nom = (char*)gtk_label_get_text( GTK_LABEL(label) );
        }

        Groupe_ajouter_Fils( pere, fils );
        g_array_append_val( scene->tGroupe, fils );
        scene->nbGroupe++;

        gtk_tree_store_append (scene->store, fils->iter, pere->iter);
        gtk_tree_store_set (scene->store, fils->iter, GROUPE, fils->nom, -1);
        gtk_widget_destroy( scene->fenetre );
    }
    return TRUE;
}

static gboolean supprimer_Groupe(GtkWidget *menuItem, gpointer data )
{
     Scene* scene = (Scene*)data;

     GtkWidget* fenetre = gtk_window_new( GTK_WINDOW_TOPLEVEL );

     gtk_window_set_position( GTK_WINDOW( fenetre ), GTK_WIN_POS_CENTER );
     gtk_window_set_modal( GTK_WINDOW( fenetre ), TRUE );
     gtk_window_set_title( GTK_WINDOW( fenetre ), "Suppression d'un Groupe" );

     GtkWidget* text = gtk_label_new("Groupe à supprimer ");

     scene->CBajoutGroupe = gtk_combo_box_text_new();

     int i = 0;

     for( i = 0; i < scene->nbGroupe; i++ )
     {
         Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, i );
         gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( scene->CBajoutGroupe ), groupe->nom );
     }

     gtk_widget_set_size_request( GTK_WIDGET( scene->CBajoutGroupe ), 100, -1 );
     gtk_widget_set_size_request( GTK_WIDGET( fenetre ), 320, -1 );

     GtkWidget* hbox = gtk_hbox_new( FALSE, 10 );

     gtk_container_add( GTK_CONTAINER( hbox ), text );
     gtk_container_add( GTK_CONTAINER( hbox ), scene->CBajoutGroupe );

     GtkWidget* boutonOk = gtk_button_new_with_label("OK");
     GtkWidget* boutonAnnuler = gtk_button_new_with_label("Annuler");

     GtkWidget* barreBouton = gtk_hbutton_box_new();
     gtk_button_box_set_layout( GTK_BUTTON_BOX( barreBouton ), GTK_BUTTONBOX_END );

     gtk_container_add( GTK_CONTAINER( barreBouton ), boutonOk );
     gtk_container_add( GTK_CONTAINER( barreBouton ), boutonAnnuler );

     GtkWidget* vbox = gtk_vbox_new( FALSE, 10 );
     gtk_container_add( GTK_CONTAINER( vbox ), hbox );
     gtk_container_add( GTK_CONTAINER( vbox ), barreBouton );

     gtk_container_add( GTK_CONTAINER( fenetre ), vbox );

     gtk_combo_box_set_active( GTK_COMBO_BOX( scene->CBajoutGroupe ), 0 );

     gtk_widget_show_all(fenetre);

     scene->fenetre = fenetre;

     g_signal_connect_object( G_OBJECT( boutonAnnuler ), "clicked", G_CALLBACK( gtk_widget_destroy ), fenetre, G_CONNECT_SWAPPED );
     g_signal_connect( G_OBJECT( boutonOk ), "clicked", G_CALLBACK( suppression_Groupe ), scene );

    return TRUE;
}

static gboolean suppression_Groupe( GtkButton* button, gpointer data )
{
    Scene* scene = (Scene*)data;

    Groupe* groupe = Groupe_trouver( scene, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(scene->CBajoutGroupe) ) ) ;

    if( groupe != NULL && groupe->id != 0 )
    {
        Groupe* pere = groupe->pere;

        int i = 0;

        for( i = 0; i < groupe->nbObjet; i++ )
        {
            Objet* objet = g_array_index( groupe->tObjet, Objet*, i );
            Groupe_ajouter_Objet( pere, objet );

            gtk_tree_store_remove( scene->store, objet->iter);
            gtk_tree_store_append (scene->store, objet->iter, pere->iter);
            gtk_tree_store_set (scene->store, objet->iter, GROUPE, objet->eType, -1); // TODO OULAH OULAH
            //gtk_tree_store_set (scene->store, objet->iter, GROUPE, objet->typeObjet, -1); // TODO OULAH OULAH
        }

        for( i = 0; i < groupe->nbFils; i++ )
        {
            Groupe* fils = g_array_index( groupe->tFils, Groupe*, i );

            gtk_tree_store_remove( scene->store, fils->iter);
            gtk_tree_store_append (scene->store, fils->iter, pere->iter);
            gtk_tree_store_set (scene->store, fils->iter, GROUPE, fils->nom, -1);
        }

        for( i = 0; i < scene->nbGroupe; i++ )
        {
            Groupe* current = g_array_index( scene->tGroupe, Groupe*, i );
            if( groupe == current )
            {
                g_array_remove_index( scene->tGroupe, i );
                scene->nbGroupe--;
            }
        }

        gtk_tree_store_remove( scene->store, groupe->iter);
        Groupe_detruire( groupe );
        gtk_widget_destroy( scene->fenetre );
    }
    else
    {
        GtkWidget* avertissement =
        gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, "Le groupe de base ne peut etre supprimé" );

        if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
        {
            gtk_widget_destroy( avertissement );

        }
        else
        {
            gtk_widget_destroy( avertissement );
        }
    }

    return TRUE;
}

static gboolean changementCurseur( GtkButton* bouton, gpointer data )
{
    Scene* scene = (Scene*)data;

    if( strcmp( gtk_button_get_label( bouton ), "Main" ) == 0 )
    {
        scene->souris = MAIN;
        scene->curseur = gdk_cursor_new_from_pixbuf( gdk_display_get_default(), gtk_image_get_pixbuf( GTK_IMAGE( gtk_image_new_from_file( "main.png") ) ), 4, 4 );
        gdk_window_set_cursor( gtk_widget_get_window( scene->zoneDeDessin ), scene->curseur );
    }
    else if( strcmp( gtk_button_get_label( bouton ), "Zoom" ) == 0 )
    {
        scene->souris = LOUPE;
        scene->curseur = gdk_cursor_new_from_pixbuf( gdk_display_get_default(), gtk_image_get_pixbuf( GTK_IMAGE( gtk_image_new_from_file( "loupe.png") ) ), 4, 4 );
        gdk_window_set_cursor( gtk_widget_get_window( scene->zoneDeDessin ), scene->curseur );
    }
    else if( strcmp( gtk_button_get_label( bouton ), "Normal" ) == 0 )
    {
        scene->souris = NORMAL;
        scene->curseur = gdk_cursor_new_from_pixbuf( gdk_display_get_default(), gtk_image_get_pixbuf( GTK_IMAGE( gtk_image_new_from_file( "normal.png") ) ), 4, 4 );
        gdk_window_set_cursor( gtk_widget_get_window( scene->zoneDeDessin ), scene->curseur );
    }
    else if( strcmp( gtk_button_get_label( bouton ), "MainWorld" ) == 0 )
	{
		scene->souris = MAINWORLD;
        scene->curseur = gdk_cursor_new_from_pixbuf( gdk_display_get_default(), gtk_image_get_pixbuf( GTK_IMAGE( gtk_image_new_from_file( "rotation.png") ) ), 4, 4 );
        gdk_window_set_cursor( gtk_widget_get_window( scene->zoneDeDessin ), scene->curseur );
	}
    else if( strcmp( gtk_button_get_label( bouton ), "Texte" ) == 0 )
	{
		scene->souris = TEXT;
        scene->curseur = gdk_cursor_new_from_pixbuf( gdk_display_get_default(), gtk_image_get_pixbuf( GTK_IMAGE( gtk_image_new_from_file( "texte.png") ) ), 4, 4 );
        gdk_window_set_cursor( gtk_widget_get_window( scene->zoneDeDessin ), scene->curseur );
	}
}
