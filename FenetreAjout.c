#include "FenetreAjout.h"
#include "Config.h"

static gboolean nouvel_ajout( GtkButton* button, gpointer data );
static gboolean FenetreAjout_change_affichage( GtkComboBox* comboBox, gpointer data );
void FenetreAjout_enlever_layout( FenetreAjout* fao );

void initialiser_FenetreAjout( FenetreAjout* fao, Scene* scene )
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
    gtk_combo_box_append_text( GTK_COMBO_BOX( comboBox ), "Triangle" );
    gtk_combo_box_append_text( GTK_COMBO_BOX( comboBox ), "Quadrilatère" );
    gtk_combo_box_append_text( GTK_COMBO_BOX( comboBox ), "Segment" );

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
    g_signal_connect( G_OBJECT( comboBox ), "changed", G_CALLBACK( FenetreAjout_change_affichage ), fao );
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

static gboolean FenetreAjout_change_affichage( GtkComboBox* comboBox, gpointer data )
{
	int i;
    FenetreAjout* fao = (FenetreAjout*)data;

    if( strcmp( gtk_combo_box_get_active_text( comboBox ), "Cube" ) == 0 )
    {
        FenetreAjout_enlever_layout( fao );

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
        FenetreAjout_enlever_layout( fao );

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
        FenetreAjout_enlever_layout( fao );

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
    else if( strcmp( gtk_combo_box_get_active_text( comboBox ), "Triangle" ) == 0 )
    {

    	 FenetreAjout_enlever_layout( fao );
    	 fao->layout = gtk_vbutton_box_new(); /* Création nouveau layout */
    	 gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->layout ), GTK_BUTTONBOX_START ); /* Précision du style de layout */
    	 fao->hbox_point1 = gtk_hbutton_box_new();
    	 fao->hbox_point2 = gtk_hbutton_box_new();
    	 fao->hbox_point3 = gtk_hbutton_box_new();
    	 fao->hbox_point4 = gtk_hbutton_box_new();

    	 GtkWidget* wLabelP1 = gtk_label_new("Point 1 : ");
    	 GtkWidget* wLabelP2 = gtk_label_new("Point 2 : ");
    	 GtkWidget* wLabelP3 = gtk_label_new("Point 3 : ");
    	 /* Allocation des input necessaires */
    	 for(i=0;i<3;++i)
    	 {
    	     (fao->xCoord)[i] = gtk_entry_new();
    	     (fao->yCoord)[i] = gtk_entry_new();
    	 }


    	 /* COnfiguration des layouts*/
    	 gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->hbox_point1 ), GTK_BUTTONBOX_START );
    	 gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->hbox_point2 ), GTK_BUTTONBOX_START );
    	 gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->hbox_point3 ), GTK_BUTTONBOX_START );

    	 gtk_container_add( GTK_CONTAINER( fao->hbox_point3 ), wLabelP3 );
    	 gtk_container_add( GTK_CONTAINER( fao->hbox_point3 ), (fao->xCoord)[2] );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point3 ), (fao->yCoord)[2] );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point2 ), wLabelP2 );
    	 gtk_container_add( GTK_CONTAINER( fao->hbox_point2 ), (fao->xCoord)[1] );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point2 ), (fao->yCoord)[1] );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point1 ), wLabelP1 );
    	 gtk_container_add( GTK_CONTAINER( fao->hbox_point1 ), (fao->xCoord)[0] );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point1 ), (fao->yCoord)[0] );


    	 /* Ajout des elements à la fenêtre */
    	 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreSelection );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barrePosition );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreCouleur );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_point1 );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_point2 );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_point3 );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreBouton );

		 gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->layout );

		 fao->dernierLayout = strcpy( fao->dernierLayout, "Triangle" );

		 gtk_widget_draw( fao->fenetre, NULL );
		 gtk_widget_show_all(fao->fenetre);
    }
    else if( strcmp( gtk_combo_box_get_active_text( comboBox ), "Quadrilatère" ) == 0 )
	{
    	FenetreAjout_enlever_layout( fao );
		 fao->layout = gtk_vbutton_box_new(); /* Création nouveau layout */
		 gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->layout ), GTK_BUTTONBOX_START ); /* Précision du style de layout */
		 fao->hbox_point1 = gtk_hbutton_box_new();
		 fao->hbox_point2 = gtk_hbutton_box_new();
		 fao->hbox_point3 = gtk_hbutton_box_new();
		 fao->hbox_point4 = gtk_hbutton_box_new();

		 GtkWidget* wLabelP1 = gtk_label_new("Point 1 : ");
		 GtkWidget* wLabelP2 = gtk_label_new("Point 2 : ");
		 GtkWidget* wLabelP3 = gtk_label_new("Point 3 : ");
		 GtkWidget* wLabelP4 = gtk_label_new("Point 4 : ");
		 /* Allocation des input necessaires */
		 for(i=0;i<4;++i)
		 {
			 (fao->xCoord)[i] = gtk_entry_new();
			 (fao->yCoord)[i] = gtk_entry_new();
		 }


		 /* COnfiguration des layouts*/
		 gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->hbox_point1 ), GTK_BUTTONBOX_START );
		 gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->hbox_point2 ), GTK_BUTTONBOX_START );
		 gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->hbox_point3 ), GTK_BUTTONBOX_START );
		 gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->hbox_point4 ), GTK_BUTTONBOX_START );

		 gtk_container_add( GTK_CONTAINER( fao->hbox_point1 ), wLabelP1 );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point1 ), (fao->xCoord)[0] );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point1 ), (fao->yCoord)[0] );

		 gtk_container_add( GTK_CONTAINER( fao->hbox_point2 ), wLabelP2 );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point2 ), (fao->xCoord)[1] );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point2 ), (fao->yCoord)[1] );

		 gtk_container_add( GTK_CONTAINER( fao->hbox_point3 ), wLabelP3 );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point3 ), (fao->xCoord)[2] );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point3 ), (fao->yCoord)[2] );

		 gtk_container_add( GTK_CONTAINER( fao->hbox_point4 ), wLabelP4 );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point4 ), (fao->xCoord)[3] );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point4 ), (fao->yCoord)[3] );


		 /* Ajout des elements à la fenêtre */
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreSelection );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barrePosition );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreCouleur );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_point1 );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_point2 );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_point3 );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_point4 );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreBouton );
		 gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->layout );

		 fao->dernierLayout = strcpy( fao->dernierLayout, "Quadrilatere" );

		 gtk_widget_draw( fao->fenetre, NULL );
		 gtk_widget_show_all(fao->fenetre);

	}
    else if( strcmp( gtk_combo_box_get_active_text( comboBox ), "Segment" ) == 0 )
	{
    	FenetreAjout_enlever_layout( fao );
		 fao->layout = gtk_vbutton_box_new(); /* Création nouveau layout */
		 gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->layout ), GTK_BUTTONBOX_START ); /* Précision du style de layout */
		 fao->hbox_point1 = gtk_hbutton_box_new();
		 fao->hbox_checkSegment = gtk_hbutton_box_new();
		 gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->hbox_checkSegment ), GTK_BUTTONBOX_CENTER ); /* Précision du style de layout */

		 GtkWidget* wLabelP1 = gtk_label_new("Extrémité : ");
		 /* Allocation des input necessaires */
		 (fao->xCoord)[0] = gtk_entry_new();
		 (fao->yCoord)[0] = gtk_entry_new();
		 (fao->zCoord)[0] = gtk_entry_new();

		 /* COnfiguration des layouts*/
		 gtk_button_box_set_layout( GTK_BUTTON_BOX( fao->hbox_point1 ), GTK_BUTTONBOX_START );

		 gtk_container_add( GTK_CONTAINER( fao->hbox_point1 ), wLabelP1 );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point1 ), (fao->xCoord)[0] );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point1 ), (fao->yCoord)[0] );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_point1 ), (fao->zCoord)[0] );


		 fao->wDashed = gtk_check_button_new_with_label("Pointillé");
		 fao->wArrowed = gtk_check_button_new_with_label("Flêche");
		 gtk_container_add( GTK_CONTAINER( fao->hbox_checkSegment ), fao->wArrowed );
		 gtk_container_add( GTK_CONTAINER( fao->hbox_checkSegment ), fao->wDashed );

		 /* Ajout des elements à la fenêtre */
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreSelection );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barrePosition );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreCouleur );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_point1 );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_checkSegment );
		 gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreBouton );
		 gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->layout );

		 fao->dernierLayout = strcpy( fao->dernierLayout, "Segment" );

		 gtk_widget_draw( fao->fenetre, NULL );
		 gtk_widget_show_all(fao->fenetre);

	}
    return TRUE;
}

void FenetreAjout_enlever_layout( FenetreAjout* fao )
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
    else if( strcmp( fao->dernierLayout, "Triangle" ) == 0 )
	{
    	gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreSelection );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barrePosition );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreCouleur );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_point1 );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_point2 );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_point3 );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreBouton );
		 gtk_container_remove( GTK_CONTAINER( fao->fenetre ), fao->layout );
	}
    else if( strcmp( fao->dernierLayout, "Quadrilatere" ) == 0 )
	{
    	gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreSelection );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barrePosition );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreCouleur );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_point1 );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_point2 );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_point3 );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_point4 );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreBouton );
		 gtk_container_remove( GTK_CONTAINER( fao->fenetre ), fao->layout );
	}
    else if( strcmp( fao->dernierLayout, "Segment" ) == 0 )
	{
    	gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreSelection );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barrePosition );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreCouleur );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_point1 );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreBouton );
		 gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->hbox_checkSegment );
		 gtk_container_remove( GTK_CONTAINER( fao->fenetre ), fao->layout );
	}
}

static gboolean nouvel_ajout( GtkButton* button, gpointer data )
{
	double dWidth = 0;
	double dHeight = 0;/* TODO : modifier les noms pour avoir hauteur + profondeur */

	tCoord tdCenter;
    FenetreAjout* fao = (FenetreAjout*)data;
    Scene* scene = (Scene*)fao->scene;


    double dR,dG,dB,dA = 0;
	double dX, dY,dZ; /*Coordonnées du centre de gravité du nouvel objet dans le repere pere*/
    /* Récupération des coudes couleurs */
	dR = atof( gtk_entry_get_text( GTK_ENTRY( fao->entryR ) ) );
	dG = atof( gtk_entry_get_text( GTK_ENTRY( fao->entryG ) ) );
	dB = atof( gtk_entry_get_text( GTK_ENTRY( fao->entryB ) ) );
	char test[5];

	strcpy(test,gtk_entry_get_text( GTK_ENTRY( fao->entryA ) ));

	//dA = atof( gtk_entry_get_text( GTK_ENTRY( fao->entryA ) ) );
	dA = (double)atof( test);

    //Coordonnées de création de l'objet
	dX = atof( gtk_entry_get_text( GTK_ENTRY( fao->entry1) ) );
	dY = atof( gtk_entry_get_text( GTK_ENTRY( fao->entry2) ) );
	dZ = atof( gtk_entry_get_text( GTK_ENTRY( fao->entry3) ) );


    if( strcmp( fao->dernierLayout, "Cube" ) == 0 )
    {
        dWidth = atof( gtk_entry_get_text( GTK_ENTRY( fao->longueur ) ) ); /*Récupération de la longueur du cube*/

        if( dWidth > 0 )
        {
        	Cube* pNewCube = NULL;
            Groupe* groupe = Groupe_trouver( scene, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(fao->comboBoxGroupe) ) ) ;

            Point_initCoord( tdCenter, dX-groupe->tCenterGroup.tdCoordWorld[0], dY-groupe->tCenterGroup.tdCoordWorld[1], dZ-groupe->tCenterGroup.tdCoordWorld[2]); /* Récupération des coordonées du centre (par rapport au groupe)*/

            pNewCube = Cube_createCube(tdCenter, dWidth, dWidth, dWidth);
            Color_setColor(pNewCube->tColor,(dR/255),(dG/255),(dB/255),dA);

            Scene_ajouter_cube( fao->scene, pNewCube, groupe->id );

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
            tCoord coord;
            tCoord coord1;

            /* On recupere le groupe concerné par la création */
            Groupe* groupe = Groupe_trouver( scene, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(fao->comboBoxGroupe) ) ) ;

            Point_initCoord( tdCenter, dX-groupe->tCenterGroup.tdCoordWorld[0], dY-groupe->tCenterGroup.tdCoordWorld[1], dZ-groupe->tCenterGroup.tdCoordWorld[2]); /* Récupération des coordonées du centre (par rapport au groupe)*/
            /* Initialisation des points aux coordonées qui vont bien pour le repere objet */
            Point_initCoord( coord, -dWidth/2, dHeight/2, 0 );         //Point_initCoord( coord, dX-dWidth/2, dY+dHeight/2, dZ );
            Point_initCoord( coord1, dWidth/2, -dHeight/2, 0 );    // Point_initCoord( coord1, dX + dWidth/2, dY - dHeight/2, dZ );
            pNewRect = Rectangle_createRectangle( coord, coord1, tdCenter );
			Color_setColor(pNewRect->tColor,(dR/255),(dG/255),(dB/255),dA);

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
        	Sphere* pNewSph =NULL;

            Groupe* groupe = Groupe_trouver( scene, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(fao->comboBoxGroupe) ) ) ;

            Point_initCoord( tdCenter, dX-groupe->tCenterGroup.tdCoordWorld[0], dY-groupe->tCenterGroup.tdCoordWorld[1], dZ-groupe->tCenterGroup.tdCoordWorld[2]); /* Récupération des coordonées du centre (par rapport au groupe)*/
            pNewSph = Sphere_createSphere( tdCenter, dWidth );
            Color_setColor(pNewSph->tColor,(dR/255),(dG/255),(dB/255),dA);

            Scene_ajouter_sphere( fao->scene, pNewSph, groupe->id );

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
    else if( strcmp( fao->dernierLayout, "Triangle" ) == 0 )
    {
    	if( 1 )
		{
			Triangle* pNewTri =NULL;
			/* Declaration de chaque coord */
			tCoord2D tCoord1,tCoord2,tCoord3;
			Point_initCoord2D(tCoord1,-100,-100);
			Point_initCoord2D(tCoord2,0,100);
			Point_initCoord2D(tCoord3,100,-100);

			Groupe* groupe = Groupe_trouver( scene, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(fao->comboBoxGroupe) ) ) ;

			Point_initCoord( tdCenter, dX-groupe->tCenterGroup.tdCoordWorld[0], dY-groupe->tCenterGroup.tdCoordWorld[1], dZ-groupe->tCenterGroup.tdCoordWorld[2]); /* Récupération des coordonées du centre (par rapport au groupe)*/
			pNewTri = Triangle_createTriangle(tdCenter,tCoord1,tCoord2,tCoord3);
			Color_setColor(pNewTri->tColor,(dR/255),(dG/255),(dB/255),dA);

			Scene_ajouter_triangle( fao->scene, pNewTri, groupe->id );

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
    else if( strcmp( fao->dernierLayout, "Quadilatère" ) == 0 )
    {
    	if( dWidth > 0  )
		{
			Quadrilateral* pNewQuadri =NULL;

			double x1 = atof( gtk_entry_get_text( GTK_ENTRY( fao->xCoord[0])));
			/*xCoord[1]
			xCoord[2]
			xCoord[3]*/
			/* Declaration de chaque coord */
			tCoord2D tCoord1,tCoord2,tCoord3,tCoord4;
			Point_initCoord2D(tCoord1,-100,-100);
			Point_initCoord2D(tCoord2,0,100);
			Point_initCoord2D(tCoord3,100,-100);
			Point_initCoord2D(tCoord4,0,-100);

			Groupe* groupe = Groupe_trouver( scene, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(fao->comboBoxGroupe) ) ) ;

			Point_initCoord( tdCenter, dX-groupe->tCenterGroup.tdCoordWorld[0], dY-groupe->tCenterGroup.tdCoordWorld[1], dZ-groupe->tCenterGroup.tdCoordWorld[2]); /* Récupération des coordonées du centre (par rapport au groupe)*/
			pNewQuadri = Quadrilateral_createQuadrilateral(tdCenter,tCoord1,tCoord2,tCoord3,tCoord4);
			Color_setColor(pNewQuadri->tColor,(dR/255),(dG/255),(dB/255),dA);

			Scene_ajouter_quadrilateral( fao->scene, pNewQuadri, groupe->id );

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
    else if( strcmp( fao->dernierLayout, "Segment" ) == 0 )
    {
    	FenetreAjout_addSegment(fao);
    }

    return TRUE;
}


void FenetreAjout_addSegment(FenetreAjout* pFao )
{
	double dR,dG,dB,dA;
	double dX1,dY1,dZ1;
	double dX2,dY2,dZ2;
	Segment* pNewSeg;
	Scene* scene = pFao->scene;
	tCoord tCoord1,tCoord2,tCenter;
	Point sP1,sP2,sMiddle;
	/* Récupération des codes couleurs */
	dR = atof( gtk_entry_get_text( GTK_ENTRY( pFao->entryR ) ) );
	dG = atof( gtk_entry_get_text( GTK_ENTRY( pFao->entryG ) ) );
	dB = atof( gtk_entry_get_text( GTK_ENTRY( pFao->entryB ) ) );
	dA = atof( gtk_entry_get_text( GTK_ENTRY( pFao->entryA ) ) );

	dX1 = atof( gtk_entry_get_text( GTK_ENTRY( pFao->entry1) ));
	dY1 = atof( gtk_entry_get_text( GTK_ENTRY( pFao->entry2) ));
	dZ1 = atof( gtk_entry_get_text( GTK_ENTRY( pFao->entry3) ));

	dX2 = atof( gtk_entry_get_text( GTK_ENTRY( (pFao->xCoord)[0] ) ));
	dY2 = atof( gtk_entry_get_text( GTK_ENTRY( (pFao->yCoord)[0] ) ));
	dZ2 = atof( gtk_entry_get_text( GTK_ENTRY( (pFao->zCoord)[0] ) ));

	Groupe* groupe = Groupe_trouver( scene, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(pFao->comboBoxGroupe) ) ) ;

	Point_initWorld(&sP1,dX1,dY1,dZ1);
	Point_initWorld(&sP2,dX2,dY2,dZ2);
	/* Recherche du mileu du segment dans le repere du monde -> centre du segment */
	Point_middlePoint(&sMiddle,&sP1, &sP2);

	/* Initialisation des coordonnées dans son propre repère*/
	Point_initCoord(tCoord1,dX1 - sMiddle.tdCoordWorld[0],dY1 - sMiddle.tdCoordWorld[1],
							dZ1 - sMiddle.tdCoordWorld[2]);
	Point_initCoord(tCoord2,dX2 - sMiddle.tdCoordWorld[0],dY2 - sMiddle.tdCoordWorld[1],
							dZ2 - sMiddle.tdCoordWorld[2]);

	/* Renseignement du tableau tdCenter */
	Point_initCoord( tCenter,  sMiddle.tdCoordWorld[0]-groupe->tCenterGroup.tdCoordWorld[0],
								sMiddle.tdCoordWorld[1]-groupe->tCenterGroup.tdCoordWorld[1],
								sMiddle.tdCoordWorld[2]-groupe->tCenterGroup.tdCoordWorld[2]);

	pNewSeg = Segment_createSegment(tCenter,tCoord1,tCoord2);
	Color_setColor(pNewSeg->tColor,(dR/255),(dG/255),(dB/255),dA);

	if(gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( pFao->wArrowed ) ) )
		Segment_setArrowed(pNewSeg);

	if(gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( pFao->wDashed ) ) )
		Segment_setDashed(pNewSeg);


	Scene_ajouter_segment( pFao->scene, pNewSeg, groupe->id );
	gtk_widget_queue_draw( scene->zoneDeDessin );
	Modification_modification_effectuer( scene );
	g_signal_emit_by_name( G_OBJECT(pFao->boutonAnnuler), "clicked" );
}
