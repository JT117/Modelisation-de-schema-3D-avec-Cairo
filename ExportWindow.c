#include "ExportWindow.h"
#include "Selection.h"

static gboolean Export_doExport( GtkButton* button, gpointer data )
{
	/* Récupération des données relatives la fenêtre d'exportation*/
	ExportWindow* ew = (ExportWindow*)data;
	Scene* scene = (Scene*)ew->scene;

	cairo_surface_t *surface = NULL; /*Surface sur laquelle on va dessiner*/
	cairo_t *cr=NULL; /*COntexte associé à la surface */

	if( strcmp(gtk_entry_get_text( GTK_ENTRY( ew->wName)),"") != 0) /*On test si le nom a été renseigné */
	{
		if( strcmp( gtk_combo_box_get_active_text( ew->comboBox ), "PNG" ) == 0 )
		{
			surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, scene->dHeight, scene->dWidth);
			cairo_t *cr = cairo_create (surface);
			Scene_clear_scene(scene , cr); /* Nettoyage de la scene */
			Scene_dessiner_scene( scene, cr ); /*Dessin de tous les objets*/
			cairo_surface_write_to_png (surface, gtk_entry_get_text( GTK_ENTRY( ew->wName)) ); /* Projection sur une surfae PNG*/
		}
		else if(strcmp( gtk_combo_box_get_active_text( ew->comboBox ), "PostScript" ) == 0 )
		{
			//TOUDOU
		}
	}
	else
	{
		GtkWidget* avertissement =
		gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez entrer un nom" );

		if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
		{
			gtk_widget_destroy( avertissement );

		}
	}
	cairo_destroy( cr );
	return TRUE;
}

void ExportWindow_init( ExportWindow* ew, Scene* scene )
{
    ew->fenetre = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    ew->scene = scene;

    gtk_window_set_position( GTK_WINDOW( ew->fenetre ), GTK_WIN_POS_CENTER );
    gtk_window_set_modal( GTK_WINDOW( ew->fenetre ), TRUE );
    gtk_window_set_title( GTK_WINDOW( ew->fenetre ), "Exportation" );

    /*Entry pour la saisie d'un nom */
    ew->wName = gtk_entry_new();
    gtk_widget_set_size_request(ew->wName, 50, -1 );

    /* Labels utilisés */
    GtkWidget* wLabelName = gtk_label_new("Nom : ");
    GtkWidget* wLabelFormat = gtk_label_new("Format : ");


    /** COMBO BOX SELECTION FORMAT **/
    ew->comboBox = gtk_combo_box_new_text();
	gtk_combo_box_append_text( GTK_COMBO_BOX( ew->comboBox ), "PNG" );
	gtk_combo_box_append_text( GTK_COMBO_BOX( ew->comboBox ), "PostScript" );

//***************************************************************************

    ew->barreObjet = gtk_hbutton_box_new();

    gtk_container_add( GTK_CONTAINER( ew->barreObjet ), wLabelName );
    gtk_container_add( GTK_CONTAINER( ew->barreObjet ), ew->wName );

    gtk_container_add( GTK_CONTAINER( ew->barreObjet ), wLabelFormat );
    gtk_container_add( GTK_CONTAINER( ew->barreObjet ), ew->comboBox );

    gtk_button_box_set_layout( GTK_BUTTON_BOX( ew->barreObjet ), GTK_BUTTONBOX_START );

    g_object_ref( ew->barreObjet );

    //*************** Barre Bouton ***********************************

    ew->wOKButton= gtk_button_new_with_label("OK");
    ew->wCancelButton = gtk_button_new_with_label("Annuler");

    ew->barreBouton = gtk_hbutton_box_new();
    gtk_button_box_set_layout( GTK_BUTTON_BOX( ew->barreBouton ), GTK_BUTTONBOX_END );

    gtk_container_add( GTK_CONTAINER( ew->barreBouton ), ew->wOKButton );
    gtk_container_add( GTK_CONTAINER( ew->barreBouton ), ew->wCancelButton );

    g_object_ref( ew->barreBouton );

    //*******************************************************************

    ew->layout = gtk_vbox_new( FALSE, 10 );

    gtk_container_add( GTK_CONTAINER( ew->layout ), ew->barreObjet );
    gtk_container_add( GTK_CONTAINER( ew->layout ), ew->barreBouton );

    gtk_container_add( GTK_CONTAINER( ew->fenetre ), ew->layout );

    /** SIGNAUX **/
    g_signal_connect_object( G_OBJECT( ew->wCancelButton ), "clicked", G_CALLBACK( gtk_widget_destroy ), ew->fenetre, G_CONNECT_SWAPPED );
	g_signal_connect( G_OBJECT( ew->wOKButton ), "clicked", G_CALLBACK( Export_doExport ), ew);

    gtk_widget_show_all(ew->fenetre);
}
