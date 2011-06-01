#include "FenetreAjoutCube.h"
#include "Config.h"

static gboolean nouvel_ajout( GtkButton* button, gpointer data );
static gboolean FenetreAjoutCube_change_affichage( GtkComboBox* comboBox, gpointer data );
void FenetreAjoutCube_enlever_layout( FenetreAjoutCube* fao );

void initialiser_FenetreAjoutCube( FenetreAjoutCube* fao, Scene* scene )
{
    fao->fenetre = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    fao->scene = scene;
    fao->dernierLayout = (char*)malloc( 50 * sizeof( char ) );
    fao->dernierLayout = strcpy( fao->dernierLayout, "Initialisation" );

    gtk_window_set_position( GTK_WINDOW( fao->fenetre ), GTK_WIN_POS_CENTER );
    gtk_window_set_modal( GTK_WINDOW( fao->fenetre ), TRUE );
    gtk_window_set_title( GTK_WINDOW( fao->fenetre ), "Création d'un nouvel objet" );

    //********* Barre de selection***************************
    fao->barreSelection = gtk_hbutton_box_new();
    GtkWidget* comboBox = gtk_combo_box_new_text();

    gtk_combo_box_append_text( GTK_COMBO_BOX( comboBox ), "Cube" );
    gtk_combo_box_append_text( GTK_COMBO_BOX( comboBox ), "Parallélépipède rectangle" );
    gtk_combo_box_append_text( GTK_COMBO_BOX( comboBox ), "Sphere" );
    gtk_combo_box_append_text( GTK_COMBO_BOX( comboBox ), "Rectangle" );
    gtk_widget_set_size_request( GTK_WIDGET( comboBox ), 250, -1 );

    GtkWidget* text = gtk_label_new("Type d'objet : ");

    fao->comboBoxGroupe = gtk_combo_box_text_new();

   int i = 0;

     for( i = 0; i < scene->nbGroupe; i++ )
     {
         Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, i );
         gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( fao->comboBoxGroupe ), groupe->nom );
     }

    gtk_widget_set_size_request( GTK_WIDGET( fao->comboBoxGroupe ), 100, -1 );

    GtkWidget* hbox11 = gtk_hbutton_box_new();
    GtkWidget* hbox13 = gtk_hbutton_box_new();
    GtkWidget* vbox12 = gtk_vbutton_box_new();

    gtk_container_add( GTK_CONTAINER( hbox11 ), text );
    gtk_container_add( GTK_CONTAINER( hbox11 ), comboBox );
    gtk_container_add( GTK_CONTAINER( vbox12 ), hbox11 );
    gtk_container_add( GTK_CONTAINER( hbox13 ), fao->comboBoxGroupe );
    gtk_container_add( GTK_CONTAINER( vbox12 ), hbox13 );

    gtk_button_box_set_layout( GTK_BUTTON_BOX( hbox13 ), GTK_BUTTONBOX_END );
    gtk_button_box_set_layout( GTK_BUTTON_BOX( hbox11 ), GTK_BUTTONBOX_END );

    gtk_container_add( GTK_CONTAINER( fao->barreSelection ), vbox12);

    gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->barreSelection ), GTK_BUTTONBOX_START );

    g_object_ref( fao->barreSelection );

    //******** Barre Position ************************************

    fao->entry1 = gtk_entry_new();
    fao->entry2 = gtk_entry_new();
    fao->entry3 = gtk_entry_new();

    gtk_widget_set_size_request(fao->entry1, 50, -1 );
    gtk_widget_set_size_request(fao->entry2, 50, -1 );
    gtk_widget_set_size_request(fao->entry3, 50, -1 );

    GtkWidget* text1 = gtk_label_new("Position X Y Z :");

    fao->barrePosition = gtk_hbutton_box_new();

    char buf[255];
    sprintf( buf, "%.3f", fao->scene->creation->tdCoordWorld[0] );
    fao->x = fao->scene->creation->tdCoordWorld[0];
    char buf1[255];
    sprintf( buf1, "%.3f", fao->scene->creation->tdCoordWorld[1]);
    fao->y = fao->scene->creation->tdCoordWorld[1];
    char buf2[255];
    sprintf( buf2, "%.3f", fao->scene->creation->tdCoordWorld[2] );
    fao->z = fao->scene->creation->tdCoordWorld[2];

    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), text1 );
    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), fao->entry1 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry1 ), buf );

    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), fao->entry2 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry2 ), buf1 );

    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), fao->entry3 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry3 ), buf2 );

    gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->barrePosition ), GTK_BUTTONBOX_END );

    g_object_ref( fao->barrePosition );

    //*************** Barre de couleur *******************************
    fao->entryR = gtk_entry_new();
    fao->entryG = gtk_entry_new();
    fao->entryB = gtk_entry_new();
    fao->entryA = gtk_entry_new();

    GtkWidget* lab = gtk_label_new("Couleur RGBA [0-255]:");

    gtk_widget_set_size_request( fao->entryR, 30, -1 );
    gtk_widget_set_size_request( fao->entryG, 30, -1 );
    gtk_widget_set_size_request( fao->entryB, 30, -1 );
    gtk_widget_set_size_request( fao->entryA, 30, -1 );

    gtk_entry_set_text( GTK_ENTRY( fao->entryR ), "0" );
    gtk_entry_set_text( GTK_ENTRY( fao->entryG ), "0" );
    gtk_entry_set_text( GTK_ENTRY( fao->entryB ), "0" );
    gtk_entry_set_text( GTK_ENTRY( fao->entryA ), "1" );

    fao->barreCouleur = gtk_hbutton_box_new();
    gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->barreCouleur ), GTK_BUTTONBOX_END );

    gtk_container_add( GTK_CONTAINER( fao->barreCouleur ), lab );
    gtk_container_add( GTK_CONTAINER( fao->barreCouleur ), fao->entryR );
    gtk_container_add( GTK_CONTAINER( fao->barreCouleur ), fao->entryG );
    gtk_container_add( GTK_CONTAINER( fao->barreCouleur ), fao->entryB );
    gtk_container_add( GTK_CONTAINER( fao->barreCouleur ), fao->entryA );

    g_object_ref( fao->barreCouleur );

    //*************** Barre Bouton ***********************************

    fao->boutonOk = gtk_button_new_with_label("OK");
    fao->boutonAnnuler = gtk_button_new_with_label("Annuler");

    fao->barreBouton = gtk_hbutton_box_new();
    gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->barreBouton ), GTK_BUTTONBOX_END );

    gtk_container_add( GTK_CONTAINER( fao->barreBouton ), fao->boutonOk );
    gtk_container_add( GTK_CONTAINER( fao->barreBouton ), fao->boutonAnnuler );

    g_object_ref( fao->barreBouton );

    //************ Signaux **********************

    g_signal_connect_object( G_OBJECT( fao->boutonAnnuler ), "clicked", G_CALLBACK( gtk_widget_destroy ), fao->fenetre, G_CONNECT_SWAPPED );
    g_signal_connect( G_OBJECT( fao->boutonOk ), "clicked", G_CALLBACK( nouvel_ajout ), fao );
    g_signal_connect( G_OBJECT( comboBox ), "changed", G_CALLBACK( FenetreAjoutCube_change_affichage ), fao );
    g_signal_connect( G_OBJECT( fao->fenetre ), "delete-event", G_CALLBACK( gtk_widget_destroy ), NULL );

    //Layout temporaire
    fao->layout = gtk_vbox_new( TRUE, 5 );

    gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreSelection );
    gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreBouton );

    gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->layout );

    gtk_combo_box_set_active( GTK_COMBO_BOX( comboBox ), 0 );
    gtk_combo_box_set_active( GTK_COMBO_BOX( fao->comboBoxGroupe ), 0 );
    g_signal_emit_by_name( GTK_OBJECT( comboBox ), "changed", NULL );
    g_signal_emit_by_name( GTK_OBJECT( fao->comboBoxGroupe ), "changed", NULL );

    gtk_widget_show_all(fao->fenetre);
}

static gboolean FenetreAjoutCube_change_affichage( GtkComboBox* comboBox, gpointer data )
{
    FenetreAjoutCube* fao = (FenetreAjoutCube*)data;

    if( strcmp( gtk_combo_box_get_active_text( comboBox ), "Cube" ) == 0 )
    {
        FenetreAjoutCube_enlever_layout( fao );

        fao->layout = gtk_vbutton_box_new();
        gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->layout ), GTK_BUTTONBOX_START );

        fao->hbox_cube = gtk_hbutton_box_new();
        GtkWidget* text = gtk_label_new("Taille des cotés : ");
        fao->longueur = gtk_entry_new();
        gtk_widget_set_size_request( fao->longueur, 70, -1 );
        gtk_widget_set_size_request( fao->fenetre, -1, -1 );

        gtk_container_add( GTK_CONTAINER( fao->hbox_cube ), text );
        gtk_container_add( GTK_CONTAINER( fao->hbox_cube ), fao->longueur );

        gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->hbox_cube ), GTK_BUTTONBOX_START );

        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreSelection );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barrePosition );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreCouleur );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_cube );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreBouton );

        gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->layout );

        fao->dernierLayout = strcpy( fao->dernierLayout, "Cube" );

        gtk_widget_draw( fao->fenetre, NULL );
        gtk_widget_show_all(fao->fenetre);
    }
    else if( (strcmp( gtk_combo_box_get_active_text( comboBox ), "Parallélépipède rectangle" ) == 0)
					|| (strcmp( gtk_combo_box_get_active_text( comboBox ), "Rectangle" ) == 0) )
    {
        FenetreAjoutCube_enlever_layout( fao );

        fao->layout = gtk_vbutton_box_new();
        gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->layout ), GTK_BUTTONBOX_START );

        fao->hbox_rect = gtk_hbox_new( FALSE, 10 );
        GtkWidget* text = gtk_label_new("Largeur : ");
        GtkWidget* text1 = gtk_label_new("Longueur : ");
        fao->longueur = gtk_entry_new();
        fao->largeur = gtk_entry_new();

        gtk_container_add( GTK_CONTAINER( fao->hbox_rect ), text1 );
        gtk_container_add( GTK_CONTAINER( fao->hbox_rect ), fao->longueur );
        gtk_container_add( GTK_CONTAINER( fao->hbox_rect ), text );
        gtk_container_add( GTK_CONTAINER( fao->hbox_rect ), fao->largeur );

        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreSelection );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barrePosition );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreCouleur );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_rect );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreBouton );

        gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->layout );

        fao->dernierLayout = strcpy( fao->dernierLayout, "Rectangle" );

        gtk_widget_draw( fao->fenetre, NULL );
        gtk_widget_show_all(fao->fenetre);
    }
    else if( strcmp( gtk_combo_box_get_active_text( comboBox ), "Sphere" ) == 0 )
    {
        FenetreAjoutCube_enlever_layout( fao );

        fao->layout = gtk_vbutton_box_new();
        gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->layout ), GTK_BUTTONBOX_START );

        fao->hbox_cube = gtk_hbutton_box_new();
        GtkWidget* text = gtk_label_new("Rayon : ");
        fao->longueur = gtk_entry_new();
        gtk_widget_set_size_request( fao->longueur, 70, -1 );
        gtk_widget_set_size_request( fao->fenetre, -1, -1 );

        gtk_container_add( GTK_CONTAINER( fao->hbox_cube ), text );
        gtk_container_add( GTK_CONTAINER( fao->hbox_cube ), fao->longueur );

        gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->hbox_cube ), GTK_BUTTONBOX_START );

        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreSelection );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barrePosition );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreCouleur );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_cube );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreBouton );

        gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->layout );

        fao->dernierLayout = strcpy( fao->dernierLayout, "Sphere" );

        gtk_widget_draw( fao->fenetre, NULL );
        gtk_widget_show_all(fao->fenetre);

    }
    return TRUE;
}

void FenetreAjoutCube_enlever_layout( FenetreAjoutCube* fao )
{
    if( strcmp( fao->dernierLayout, "Initialisation" ) == 0 )
    {
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreSelection );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreBouton );
        gtk_container_remove( GTK_CONTAINER( fao->fenetre ), fao->layout );
    }
    else if( strcmp( fao->dernierLayout, "Cube" ) == 0 )
    {
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreSelection );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreBouton );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barrePosition );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreCouleur );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_cube );
        gtk_container_remove( GTK_CONTAINER( fao->fenetre ), fao->layout );
    }
    else if( strcmp( fao->dernierLayout, "Rectangle" ) == 0 )
    {
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreSelection );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreBouton );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barrePosition );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreCouleur );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_rect );
        gtk_container_remove( GTK_CONTAINER( fao->fenetre ),fao->layout );
    }
    else if( strcmp( fao->dernierLayout, "Sphere" ) == 0 )
    {
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreSelection );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreBouton );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barrePosition );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreCouleur );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_cube );
        gtk_container_remove( GTK_CONTAINER( fao->fenetre ), fao->layout );
    }
}

static gboolean nouvel_ajout( GtkButton* button, gpointer data )
{
	double dWidth, dHeight ;/* TODO : modifier les noms pour avoir hauteur + profondeur */
	double dX, dY,dZ; /*Coordonnées du centre de gravité du nouvel objet*/
	tdCoord tdCenter;
    FenetreAjoutCube* fao = (FenetreAjoutCube*)data;
    Scene* scene = (Scene*)fao->scene;
    Cube* pNewCube = NULL;

    double dR,dG,dB,dA = 0;
    /* Récupération des coudes couleurs */
	dR = atof( gtk_entry_get_text( GTK_ENTRY( fao->entryR ) ) );
	dG = atof( gtk_entry_get_text( GTK_ENTRY( fao->entryG ) ) );
	dB = atof( gtk_entry_get_text( GTK_ENTRY( fao->entryB ) ) );
	dA = atof( gtk_entry_get_text( GTK_ENTRY( fao->entryA ) ) );

    //Coordonnées de création de l'objet
	dX = atof( gtk_entry_get_text( GTK_ENTRY( fao->entry1) ) );
	dY = atof( gtk_entry_get_text( GTK_ENTRY( fao->entry2) ) );
	dZ = atof( gtk_entry_get_text( GTK_ENTRY( fao->entry3) ) );


    if( strcmp( fao->dernierLayout, "Cube" ) == 0 )
    {
        dWidth = atof( gtk_entry_get_text( GTK_ENTRY( fao->longueur ) ) ); /*Récupération de la longueur du cube*/

        if( dWidth > 0 )
        {
            Point_initCoord( tdCenter, dX, dY, dZ);
            pNewCube = Cube_createCube(tdCenter, dWidth, dWidth, dWidth);

            Groupe* groupe = Groupe_trouver( scene, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(fao->comboBoxGroupe) ) ) ;

            Scene_ajouter_cube( fao->scene, pNewCube, groupe->id );
            Color_setColor(pNewCube->tColor,(dR/255),(dG/255),(dB/255),dA);

            gtk_widget_queue_draw( scene->zoneDeDessin );
            Modification_modification_effectuer( scene );
            g_signal_emit_by_name( G_OBJECT(fao->boutonAnnuler), "clicked" );
        }
        else
        {
            GtkWidget* avertissement =
            gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez entrer une taille !" );

            if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
            {
                gtk_widget_destroy( avertissement );

            }
        }
    }
    else if( strcmp( fao->dernierLayout, "Rectangle" ) == 0 )
    {
        dWidth = atof( gtk_entry_get_text( GTK_ENTRY( fao->longueur ) ) ); /*Récupération de la longueur du cube*/
        dHeight = atof( gtk_entry_get_text( GTK_ENTRY( fao->largeur ) ) );

        if( dWidth > 0 && dHeight > 0 )
        {
            Rectangle* pNewRect;
            tdCoord coord;
            tdCoord coord1;

            Point_initCoord( tdCenter, dX, dY, dZ); /* Récupération des coordonées du centre */
            Point_initCoord( coord, dX-dWidth/2, dY+dHeight/2, dZ );
            Point_initCoord( coord1, dX + dWidth/2, dY - dHeight/2, dZ );
            pNewRect = Rectangle_createRectangle( coord, coord1, tdCenter );
			Color_setColor(pNewRect->tColor,(dR/255),(dG/255),(dB/255),dA);

            Groupe* groupe = Groupe_trouver( scene, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(fao->comboBoxGroupe) ) ) ;

            Scene_ajouter_rectangle( fao->scene, pNewRect, groupe->id );

            gtk_widget_queue_draw( scene->zoneDeDessin );
            Modification_modification_effectuer( scene );
            g_signal_emit_by_name( G_OBJECT(fao->boutonAnnuler), "clicked" );

        }
        else
        {
            GtkWidget* avertissement =
            gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez entrer une taille !" );

            if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
                gtk_widget_destroy( avertissement );
        }
    }
    else if( strcmp( fao->dernierLayout, "Sphere" ) == 0 )
    {
        dWidth = atof( gtk_entry_get_text( GTK_ENTRY( fao->longueur ) ) ); /*Récupération de la longueur du cube*/

        if( dWidth > 0  )
        {
            Point_initCoord( tdCenter, dX, dY, dZ);
            Groupe* groupe = Groupe_trouver( scene, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(fao->comboBoxGroupe) ) ) ;

            Sphere* pNewSph;
            pNewSph = Sphere_createSphere( tdCenter, dWidth );

            Scene_ajouter_sphere( fao->scene, pNewSph, groupe->id );

            Color_setColor(pNewSph->tColor,(dR/255),(dG/255),(dB/255),dA);

            gtk_widget_queue_draw( scene->zoneDeDessin );
            Modification_modification_effectuer( scene );
            g_signal_emit_by_name( G_OBJECT(fao->boutonAnnuler), "clicked" );
        }
        else
        {
            GtkWidget* avertissement =
            gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez entrer une taille !" );

            if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
            {
                gtk_widget_destroy( avertissement );

            }
        }
    }

    return TRUE;
}
