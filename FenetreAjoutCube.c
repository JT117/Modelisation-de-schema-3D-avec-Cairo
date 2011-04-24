#include "FenetreAjoutCube.h"
#include "Config.h"

static gboolean nouvel_ajout( GtkButton* button, gpointer data );
static gboolean FenetreAjoutCube_change_affichage( GtkComboBox* comboBox, gpointer data );

void initialier_FenetreAjoutCube( FenetreAjoutCube* fao, Scene* scene )
{
    fao->fenetre = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    fao->scene = scene;

    gtk_window_set_position( GTK_WINDOW( fao->fenetre ), GTK_WIN_POS_CENTER );
    gtk_window_set_modal( GTK_WINDOW( fao->fenetre ), TRUE );
    gtk_window_set_title( GTK_WINDOW( fao->fenetre ), "Création d'un nouvel objet" );

    //********* Barre de selection***************************
    fao->barreSelection = gtk_hbox_new( TRUE, 20 );
    GtkWidget* comboBox = gtk_combo_box_new_text();
    gtk_combo_box_append_text( comboBox, "Cube" );
    gtk_combo_box_append_text( comboBox, "Rectangle" );

    GtkWidget* text = gtk_label_new("Type d'objet : ");

    gtk_container_add( GTK_CONTAINER( fao->barreSelection ), text );
    gtk_container_add( GTK_CONTAINER( fao->barreSelection ), comboBox );

    g_object_ref( fao->barreSelection );

    //******** Barre Position ************************************

    fao->entry1 = gtk_entry_new();
    fao->entry2 = gtk_entry_new();
    fao->entry3 = gtk_entry_new();

    GtkWidget* text0 = gtk_label_new("Taille des cotés en pixel : ");
    GtkWidget* text1 = gtk_label_new("Position X : ");
    GtkWidget* text2 = gtk_label_new("Position Y : ");
    GtkWidget* text3 = gtk_label_new("Position Z : ");

    fao->barrePosition = gtk_hbox_new( FALSE, 20 );

    char buf[255];
    sprintf( buf, "%.3f", fao->scene->creation->x );
    char buf1[255];
    sprintf( buf1, "%.3f", fao->scene->creation->y );
    char buf2[255];
    sprintf( buf2, "%.3f", fao->scene->creation->z );

    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), text1 );
    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), fao->entry1 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry1 ), buf );

    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), text2 );
    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), fao->entry2 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry2 ), buf1 );

    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), text3 );
    gtk_container_add( GTK_CONTAINER( fao->barrePosition ), fao->entry3 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry3 ), buf2 );

    g_object_ref( fao->barrePosition );

    //*************** Barre Bouton ***********************************

    fao->boutonOk = gtk_button_new_with_label("OK");
    fao->boutonAnnuler = gtk_button_new_with_label("Annuler");

    fao->barreBouton = gtk_hbox_new( FALSE, 20 );

    gtk_container_add( GTK_CONTAINER( fao->barreBouton ), fao->boutonOk );
    gtk_container_add( GTK_CONTAINER( fao->barreBouton ), fao->boutonAnnuler );

    g_object_ref( fao->barreBouton );

    //************ Signaux **********************

    g_signal_connect_object( G_OBJECT( fao->boutonAnnuler ), "clicked", G_CALLBACK( gtk_widget_destroy ), fao->fenetre, G_CONNECT_SWAPPED );
    g_signal_connect( G_OBJECT( fao->boutonOk ), "clicked", G_CALLBACK( nouvel_ajout ), fao );
    g_signal_connect( G_OBJECT( comboBox ), "changed", G_CALLBACK( FenetreAjoutCube_change_affichage ), fao );
    g_signal_connect( G_OBJECT( fao->fenetre ), "delete-event", G_CALLBACK( gtk_widget_destroy ), NULL );

    //Layout temporaire
    fao->layout = gtk_vbox_new( TRUE, 20 );

    gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreSelection );
    gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreBouton );

    gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->layout );

    gtk_widget_show_all(fao->fenetre);
}

static gboolean FenetreAjoutCube_change_affichage( GtkComboBox* comboBox, gpointer data )
{
    FenetreAjoutCube* fao = (FenetreAjoutCube*)data;

    if( strcmp( gtk_combo_box_get_active_text( comboBox ), "Cube" ) == 0 )
    {
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreSelection );
        gtk_container_remove( GTK_CONTAINER( fao->layout ), fao->barreBouton );
        gtk_container_remove( GTK_CONTAINER( fao->fenetre ), fao->layout );

        fao->layout = gtk_vbox_new( FALSE, 20 );

        GtkWidget* hbox = gtk_hbox_new( FALSE, 20 );
        GtkWidget* text = gtk_label_new("Taille des cotés : ");
        fao->entry = gtk_entry_new();

        gtk_container_add( GTK_CONTAINER( hbox ), text );
        gtk_container_add( GTK_CONTAINER( hbox ), fao->entry );

        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreSelection );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barrePosition );
        gtk_container_add( GTK_CONTAINER( fao->layout ), hbox );
        gtk_container_add( GTK_CONTAINER( fao->layout ), fao->barreBouton );

        gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->layout );

        gtk_widget_draw( fao->fenetre, NULL );
        gtk_widget_show_all(fao->fenetre);
    }
}


static gboolean nouvel_ajout( GtkButton* button, gpointer data )
{
    FenetreAjoutCube* fao = (FenetreAjoutCube*)data;
    Scene* scene = (Scene*)fao->scene;

    scene->tailleCreation = atof( gtk_entry_get_text( GTK_ENTRY( fao->entry ) ) );

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





