#include "FenTrans.h"


void FenTrans_init( FenTrans* ft, Scene* scene )
{
    int i = 0;
    ft->unGroupeEstSelectionner = FALSE;

    ft->fenetre = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    ft->scene = scene;

    gtk_window_set_position( GTK_WINDOW( ft->fenetre ), GTK_WIN_POS_CENTER );
    gtk_window_set_modal( GTK_WINDOW( ft->fenetre ), TRUE );
    gtk_window_set_title( GTK_WINDOW( ft->fenetre ), "Fenetre Trasnformation" );


    for( i = 0; i < scene->nbGroupe; i++ )
    {
        Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, i );

        if( gtk_tree_selection_iter_is_selected( scene->treeSelection, groupe->iter ) )
        {
            ft->unGroupeEstSelectionner = TRUE;
            ft->groupeSelectionne = groupe;
        }
    }

    GtkWidget* hbox = gtk_hbutton_box_new( );
    gtk_button_box_set_layout( GTK_BUTTON_BOX( hbox ), GTK_BUTTONBOX_END );

    ft->radio1 = gtk_radio_button_new_with_label( NULL, "Rotation" );
    ft->radio2 = gtk_radio_button_new_with_label_from_widget( GTK_RADIO_BUTTON( ft->radio1 ), "Translation" );

    gtk_container_add( GTK_CONTAINER( hbox ), ft->radio1 );
    gtk_container_add( GTK_CONTAINER( hbox ), ft->radio2 );

    if( !ft->unGroupeEstSelectionner )
    {
        ft->radio3 = gtk_radio_button_new_with_label_from_widget( GTK_RADIO_BUTTON( ft->radio1 ), "Homothétie" );
        gtk_container_add( GTK_CONTAINER( hbox ), ft->radio3 );
    }

    GtkWidget* barreParam = gtk_hbutton_box_new( );
    gtk_button_box_set_layout( GTK_BUTTON_BOX( barreParam ), GTK_BUTTONBOX_END );

    ft->entry1 = gtk_entry_new();
    ft->entry2 = gtk_entry_new();
    ft->entry3 = gtk_entry_new();

    GtkWidget* text1 = gtk_label_new("X : ");
    GtkWidget* text2 = gtk_label_new("Y : ");
    GtkWidget* text3 = gtk_label_new("Z : ");

    gtk_container_add( GTK_CONTAINER( barreParam ), text1 );
    gtk_container_add( GTK_CONTAINER( barreParam ), ft->entry1 );
    gtk_container_add( GTK_CONTAINER( barreParam ), text2 );
    gtk_container_add( GTK_CONTAINER( barreParam ), ft->entry2 );
    gtk_container_add( GTK_CONTAINER( barreParam ), text3 );
    gtk_container_add( GTK_CONTAINER( barreParam ), ft->entry3 );

    GtkWidget* layout = gtk_vbox_new( 5, TRUE );
//*************************************************************
    GtkWidget* boutonOk = gtk_button_new_with_label("OK");
    GtkWidget* boutonAnnuler = gtk_button_new_with_label("Annuler");

    GtkWidget* barreBouton = gtk_hbutton_box_new();
    gtk_button_box_set_layout( GTK_BUTTON_BOX( barreBouton ), GTK_BUTTONBOX_END );

    gtk_container_add( GTK_CONTAINER( barreBouton ), boutonOk );
    gtk_container_add( GTK_CONTAINER( barreBouton ), boutonAnnuler );
//************************************************************

    gtk_container_add( GTK_CONTAINER( layout ), hbox );
    gtk_container_add( GTK_CONTAINER( layout ), barreParam );
    gtk_container_add( GTK_CONTAINER( layout ), barreBouton );
    gtk_container_add( GTK_CONTAINER( ft->fenetre ), layout );

    gtk_widget_show_all( ft->fenetre );

    g_signal_connect_object( G_OBJECT( boutonAnnuler ), "clicked", G_CALLBACK( gtk_widget_destroy ), ft->fenetre, G_CONNECT_SWAPPED );
    g_signal_connect( G_OBJECT( boutonOk ), "clicked", G_CALLBACK( FenTrans_validation ), ft );

}


void FenTrans_validation( GtkButton* button, gpointer data )
{
    FenTrans* ft = (FenTrans*)data;

    double dX, dY, dZ = 0;
    dX = atof( gtk_entry_get_text( GTK_ENTRY( ft->entry1 ) ) );
	dY = atof( gtk_entry_get_text( GTK_ENTRY( ft->entry2 ) ) );
	dZ = atof( gtk_entry_get_text( GTK_ENTRY( ft->entry3 ) ) );

    if( ft->unGroupeEstSelectionner &&  gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ft->radio1) ) )
    {
        /* TODO rotation sur un groupe */
    }
    else if( ft->unGroupeEstSelectionner && gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ft->radio2) ) )
    {
        /* TODO translation d'un groupe */
    }
    else if(  gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ft->radio3) ) )
    {
        /*Todo homothetie sur objet */
    }
    else if( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ft->radio1) ) )
    {
        int i = 0;

        for( i = 0; i < ft->scene->selection->nbSelection; i++ )
        {
            Objet* objet = g_array_index( ft->scene->selection->tSelection, Objet*, i );
            Objet_rotation( objet, dX, dY );
        }
    }
    else if( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ft->radio2 ) ) )
    {

    }
    gtk_widget_queue_draw( ft->scene->zoneDeDessin );
    gtk_widget_destroy( ft->fenetre );
}
