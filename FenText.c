#include "FenText.h"

void FenText_init( FenText* ft, Scene* scene )
{
    ft->fenetre = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    ft->scene = scene;
    ft->fontPerso = FALSE;

    gtk_window_set_position( GTK_WINDOW( ft->fenetre ), GTK_WIN_POS_CENTER );
    gtk_window_set_title( GTK_WINDOW( ft->fenetre ), "Ajout de texte" );

    GtkWidget* hbox = gtk_hbox_new( 5, TRUE );
    GtkWidget* boutonProp = gtk_button_new_with_label("Proprietés");
    ft->entry1 = gtk_entry_new();

    gtk_container_add( GTK_CONTAINER( hbox ), ft->entry1 );
    gtk_container_add( GTK_CONTAINER( hbox ), boutonProp );

    //**********************************************************************
    GtkWidget* boutonOk = gtk_button_new_with_label("OK");
    GtkWidget* boutonAnnuler = gtk_button_new_with_label("Annuler");

    GtkWidget* barreBouton = gtk_hbutton_box_new();
    gtk_button_box_set_layout( GTK_BUTTON_BOX( barreBouton ), GTK_BUTTONBOX_END );

    gtk_container_add( GTK_CONTAINER( barreBouton ), boutonOk );
    gtk_container_add( GTK_CONTAINER( barreBouton ), boutonAnnuler );
    //****************************************************************************

    GtkWidget* layout = gtk_vbox_new( 5, TRUE );
    gtk_container_add( GTK_CONTAINER( layout ), hbox );
    gtk_container_add( GTK_CONTAINER( layout ), barreBouton );
    gtk_container_add( GTK_CONTAINER( ft->fenetre ), layout );

    gtk_widget_show_all( ft->fenetre );

    g_signal_connect_object( G_OBJECT( boutonAnnuler ), "clicked", G_CALLBACK( gtk_widget_destroy ), ft->fenetre, G_CONNECT_SWAPPED );
    g_signal_connect( G_OBJECT( boutonOk ), "clicked", G_CALLBACK( FenText_validation ), ft );
    g_signal_connect( G_OBJECT( boutonProp ), "clicked", G_CALLBACK( FenText_propriete ), ft );
}


void FenText_propriete( GtkButton* button, gpointer data )
{
    FenText* ft = (FenText*)data;

    ft->fontSelection = gtk_font_selection_dialog_new( "Propriete du texte" );
    gtk_font_selection_dialog_set_preview_text( GTK_FONT_SELECTION_DIALOG( ft->fontSelection ), gtk_entry_get_text( GTK_ENTRY( ft->entry1 ) ) );

    g_signal_connect_object( G_OBJECT( gtk_font_selection_dialog_get_cancel_button( GTK_FONT_SELECTION_DIALOG(ft->fontSelection)) ), "clicked", G_CALLBACK( gtk_widget_destroy ), ft->fontSelection, G_CONNECT_SWAPPED );
    g_signal_connect( G_OBJECT(  gtk_font_selection_dialog_get_ok_button( GTK_FONT_SELECTION_DIALOG(ft->fontSelection) ) ), "clicked", G_CALLBACK( FenText_changement ), ft );

    gtk_widget_show_all( ft->fontSelection );
}

void FenText_changement( GtkButton* button, gpointer data )
{
    FenText* ft = (FenText*)data;
    ft->fontPerso = TRUE;
    ft->font = (char*)malloc( 60 * sizeof( char ) );
    ft->font = gtk_font_selection_dialog_get_font_name( GTK_FONT_SELECTION_DIALOG(ft->fontSelection) );
    gtk_widget_destroy( ft->fontSelection );
}

void FenText_validation( GtkButton* button, gpointer data )
{
    FenText* ft = (FenText*)data;
    Objet* objet = (Objet*)malloc( 1 * sizeof( Objet ) );
    objet->typeObjet = (char*)malloc( 5 * sizeof(char) );
    strcpy( objet->typeObjet, "Texte" );
    objet->doitEtreDeselectionner = TRUE;
    objet->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter ) );
    objet->texte = (char*)malloc( 60 * sizeof( char ) );
    objet->x = ft->scene->creation->x;
    objet->y = ft->scene->creation->y;

   if( ft->fontPerso )
    {
        objet->font = ft->font;
    }
    else
    {
         objet->font = NULL;
    }

    GtkWidget* label = gtk_label_new( gtk_entry_get_text( GTK_ENTRY(ft->entry1) ) );
    objet->texte = (char*)gtk_label_get_text( GTK_LABEL(label) );

    g_array_append_val( ft->scene->tObjet, objet );
    ft->scene->nbObjet++;

    Groupe* groupe = Groupe_trouver_ById( ft->scene, 0 );

    Groupe_ajouter_Objet( groupe, objet );

    gtk_tree_store_append (ft->scene->store, objet->iter, groupe->iter );
    gtk_tree_store_set (ft->scene->store, objet->iter, GROUPE, objet->texte, -1);

    gtk_widget_queue_draw( ft->scene->zoneDeDessin );
    gtk_widget_destroy( ft->fenetre );
}
