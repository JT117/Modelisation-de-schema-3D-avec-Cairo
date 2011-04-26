#include "FenetreAjoutCube.h"
#include "Config.h"

static gboolean nouvel_ajout( GtkButton* button, gpointer data );
static gboolean FenetreAjoutCube_change_affichage( GtkComboBox* comboBox, gpointer data );

void initialier_FenetreAjoutCube( FenetreAjoutCube* fao, Scene* scene )
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
    gtk_combo_box_append_text( comboBox, "Cube" );
    gtk_combo_box_append_text( comboBox, "Rectangle" );

    gtk_widget_set_size_request( comboBox, 100, -1 );

    GtkWidget* text = gtk_label_new("Type d'objet : ");

    gtk_container_add( GTK_CONTAINER( fao->barreSelection ), text );
    gtk_container_add( GTK_CONTAINER( fao->barreSelection ), comboBox );
    gtk_button_box_set_layout( fao->barreSelection, GTK_BUTTONBOX_START );

    g_object_ref( fao->barreSelection );

    //******** Barre Position ************************************

    fao->entry1 = gtk_entry_new();
    fao->entry2 = gtk_entry_new();
    fao->entry3 = gtk_entry_new();

    gtk_widget_set_size_request(fao->entry1, 50, -1 );
    gtk_widget_set_size_request(fao->entry2, 70, -1 );
    gtk_widget_set_size_request(fao->entry3, 70, -1 );

    GtkWidget* text0 = gtk_label_new("Taille des cotés en pixel : ");
    GtkWidget* text1 = gtk_label_new("Position X : ");
    GtkWidget* text2 = gtk_label_new("Position Y : ");
    GtkWidget* text3 = gtk_label_new("Position Z : ");

    fao->barrePosition = gtk_hbutton_box_new();

    char buf[255];
    sprintf( buf, "%.3f", fao->scene->creation->x );
    fao->x = fao->scene->creation->x;
    char buf1[255];
    sprintf( buf1, "%.3f", fao->scene->creation->y );
    fao->y = fao->scene->creation->y;
    char buf2[255];
    sprintf( buf2, "%.3f", fao->scene->creation->z );
    fao->z = fao->scene->creation->z;

    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), text1 );
    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), fao->entry1 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry1 ), buf );

    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), text2 );
    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), fao->entry2 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry2 ), buf1 );

    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), text3 );
    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), fao->entry3 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry3 ), buf2 );

    gtk_button_box_set_layout( fao->barrePosition, GTK_BUTTONBOX_START );

    g_object_ref( fao->barrePosition );

    //*************** Barre Bouton ***********************************

    fao->boutonOk = gtk_button_new_with_label("OK");
    fao->boutonAnnuler = gtk_button_new_with_label("Annuler");

    fao->barreBouton = gtk_hbutton_box_new();
    gtk_button_box_set_layout( fao->barreBouton, GTK_BUTTONBOX_END );

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

    gtk_combo_box_set_active( comboBox, 0 );
    g_signal_emit( GTK_OBJECT( comboBox ), "changed", NULL );

    gtk_widget_show_all(fao->fenetre);
}

static gboolean FenetreAjoutCube_change_affichage( GtkComboBox* comboBox, gpointer data )
{
    FenetreAjoutCube* fao = (FenetreAjoutCube*)data;

    if( strcmp( gtk_combo_box_get_active_text( comboBox ), "Cube" ) == 0 )
    {
        FenetreAjoutCube_enlever_layout( fao );

        fao->layout = gtk_vbutton_box_new();
        gtk_button_box_set_layout( fao->layout, GTK_BUTTONBOX_START );

        fao->hbox_cube = gtk_hbutton_box_new();
        GtkWidget* text = gtk_label_new("Taille des cotés : ");
        fao->longueur = gtk_entry_new();
        gtk_widget_set_size_request( fao->longueur, 70, -1 );
        gtk_widget_set_size_request( fao->fenetre, -1, -1 );

        gtk_container_add( GTK_CONTAINER( fao->hbox_cube ), text );
        gtk_container_add( GTK_CONTAINER( fao->hbox_cube ), fao->longueur );

        gtk_button_box_set_layout( fao->hbox_cube, GTK_BUTTONBOX_START );

        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreSelection );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barrePosition );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->hbox_cube );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreBouton );

        gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->layout );

        fao->dernierLayout = strcpy( fao->dernierLayout, "Cube" );

        gtk_widget_draw( fao->fenetre, NULL );
        gtk_widget_show_all(fao->fenetre);
    }
    else if( strcmp( gtk_combo_box_get_active_text( comboBox ), "Rectangle" ) == 0 )
    {
        FenetreAjoutCube_enlever_layout( fao );

        fao->layout = gtk_vbutton_box_new();
        gtk_button_box_set_layout( fao->layout, GTK_BUTTONBOX_START );

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
    FenetreAjoutCube* fao = (FenetreAjoutCube*)data;
    Scene* scene = (Scene*)fao->scene;

    scene->tailleCreation = atof( gtk_entry_get_text( GTK_ENTRY( fao->longueur ) ) );

    if( fao->scene->tailleCreation > 0 )
    {
        Cube* cube = (Cube*)malloc( 1 * sizeof( Cube ) );
        initialiser_Cube( cube, fao->scene->creation->x, fao->scene->creation->y, fao->scene->creation->z, fao->scene->tailleCreation );
        Scene_ajouter_cube( fao->scene, cube );
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





