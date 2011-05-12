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

    gtk_widget_set_size_request( comboBox, 100, -1 );

    GtkWidget* text = gtk_label_new("Type d'objet : ");

    gtk_container_add( GTK_CONTAINER( fao->barreSelection ), text );
    gtk_container_add( GTK_CONTAINER( fao->barreSelection ), comboBox );
    gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->barreSelection ), GTK_BUTTONBOX_START );

    g_object_ref( fao->barreSelection );

    //******** Barre Position ************************************

    fao->entry1 = gtk_entry_new();
    fao->entry2 = gtk_entry_new();
    fao->entry3 = gtk_entry_new();

    gtk_widget_set_size_request(fao->entry1, 50, -1 );
    gtk_widget_set_size_request(fao->entry2, 70, -1 );
    gtk_widget_set_size_request(fao->entry3, 70, -1 );

//    GtkWidget* text0 = gtk_label_new("Taille des cotés en pixel : ");
    GtkWidget* text1 = gtk_label_new("Position X : ");
    GtkWidget* text2 = gtk_label_new("Position Y : ");
    GtkWidget* text3 = gtk_label_new("Position Z : ");

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

    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), text2 );
    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), fao->entry2 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry2 ), buf1 );

    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), text3 );
    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), fao->entry3 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry3 ), buf2 );

    gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->barrePosition ), GTK_BUTTONBOX_START );

    g_object_ref( fao->barrePosition );

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
    fao->layout = gtk_vbox_new( FALSE, 10 );

    gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreSelection );
    gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreBouton );

    gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->layout );

    gtk_combo_box_set_active( GTK_COMBO_BOX( comboBox ), 0 );
    g_signal_emit_by_name( GTK_OBJECT( comboBox ), "changed", NULL );

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
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_cube );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreBouton );

        gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->layout );

        fao->dernierLayout = strcpy( fao->dernierLayout, "Cube" );

        gtk_widget_draw( fao->fenetre, NULL );
        gtk_widget_show_all(fao->fenetre);
    }
    else if( strcmp( gtk_combo_box_get_active_text( comboBox ), "Parallélépipède rectangle" ) == 0 )
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
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_rect );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreBouton );

        gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->layout );

        fao->dernierLayout = strcpy( fao->dernierLayout, "Rectangle" );

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
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_cube );
        gtk_container_remove( GTK_CONTAINER( fao->fenetre ), fao->layout );
    }
    else if( strcmp( fao->dernierLayout, "Rectangle" ) == 0 )
    {
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreSelection );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreBouton );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barrePosition );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_rect );
        gtk_container_remove( GTK_CONTAINER( fao->fenetre ), fao->layout );
    }
}

static gboolean nouvel_ajout( GtkButton* button, gpointer data )
{
	double dWidth;//, largeur; /* TODO : modifier les noms pour avoir hauteur + profondeur */
	double dX, dY,dZ; /*Coordonnées du centre de gravité du nouvel objet*/
	tdCoord tdCenter;
    FenetreAjoutCube* fao = (FenetreAjoutCube*)data;
    Scene* scene = (Scene*)fao->scene;
    Cube* pNewCube = NULL;

	dX = atof( gtk_entry_get_text( GTK_ENTRY( fao->entry1) ) );
	dY = atof( gtk_entry_get_text( GTK_ENTRY( fao->entry2) ) );
	dZ = atof( gtk_entry_get_text( GTK_ENTRY( fao->entry3) ) );

    if( fao->scene->tailleCreation > 0 )
    {
        if( strcmp( fao->dernierLayout, "Cube" ) == 0 )
        {
        	dWidth = atof( gtk_entry_get_text( GTK_ENTRY( fao->longueur ) ) ); /*Récupération de la longueur du cube*/
            Point_initCoord( tdCenter, dX, dY, dZ);
            pNewCube = Cube_createCube(tdCenter, dWidth, dWidth, dWidth);
            Scene_ajouter_cube( fao->scene, pNewCube );

            //TODO : temporaire
            //creation de segment pour test
            tdCoord coordSeg1, coordSeg2;
            Segment* seg = NULL;
            Point_initCoord(coordSeg1,0,0,0);
            Point_initCoord(coordSeg2,683,0,0);
            seg = Segment_createSegment(coordSeg1, coordSeg2);
            Segment_setColor(seg,0.0,1.0,0.0);
            Scene_ajouter_segment(fao->scene,seg);

            Point_initCoord(coordSeg1,0,0,0);
			Point_initCoord(coordSeg2,0,50,0);
			seg = Segment_createSegment(coordSeg1, coordSeg2);
			Segment_setColor(seg,1.0,0.0,0.0);
            Scene_ajouter_segment(fao->scene,seg);

            Point_initCoord(coordSeg1,0,0,0);
			Point_initCoord(coordSeg2,0,0,50);
			seg = Segment_createSegment(coordSeg1, coordSeg2);
			Segment_setColor(seg,0.0,0.0,1.0);
			Scene_ajouter_segment(fao->scene,seg);
        }
        else if( strcmp( fao->dernierLayout, "Rectangle" ) == 0 )
        {
            Rectangle rect;
            tdCoord coord;
            tdCoord coord1;
            Point_initCoord( coord, fao->scene->creation->x, fao->scene->creation->y, fao->scene->creation->z );
            // Largeur non prise en compte pour le test
            Point_initCoord( coord1, fao->scene->creation->x + fao->scene->tailleCreation , fao->scene->creation->y + fao->scene->tailleCreation, fao->scene->creation->z );
            printf("Apres init :\n x = %f | x1 = %f\n y = %f | y1 = %f\n", coord[0], coord1[0], coord[1], coord1[1] );
            Rectangle_createRectangle( coord, coord1 );
            Scene_ajouter_rectangle( fao->scene, &rect );
        }

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

    return TRUE;
}