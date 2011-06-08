#include "ExportWindow.h"

void ExportWindow_init( ExportWindow* ew, Scene* scene )
{
    GtkWidget *dialog = gtk_file_chooser_dialog_new ("Exporter la scene", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
    GtkFileFilter* filtre = gtk_file_filter_new();
    gtk_file_filter_add_pattern( GTK_FILE_FILTER (filtre), "*.png");
    gtk_file_filter_add_pattern( GTK_FILE_FILTER (filtre), "*.pdf");
    gtk_file_filter_add_pattern( GTK_FILE_FILTER (filtre), "*.svg");
    gtk_file_chooser_set_filter( GTK_FILE_CHOOSER(dialog), GTK_FILE_FILTER(filtre) );

    ew->comboBox = gtk_combo_box_new_text();
	gtk_combo_box_append_text( GTK_COMBO_BOX( ew->comboBox ), "png" );
	gtk_combo_box_append_text( GTK_COMBO_BOX( ew->comboBox ), "svg" );
	gtk_combo_box_append_text( GTK_COMBO_BOX( ew->comboBox ), "pdf" );
    gtk_file_chooser_set_extra_widget( GTK_FILE_CHOOSER(dialog), ew->comboBox );
    gtk_combo_box_set_active( GTK_COMBO_BOX( ew->comboBox ), 0 );
    g_signal_emit_by_name( GTK_OBJECT( ew->comboBox ), "changed", NULL );

    if( gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT )
    {
        char *filename = NULL;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        filename = (char*)realloc( filename, ( strlen(filename) + 4 ) * sizeof( char ) );
        filename = strcat( filename,"." );
        filename = strcat( filename, gtk_combo_box_get_active_text( GTK_COMBO_BOX( ew->comboBox ) ) );

        printf( "Exportation : %s\n", filename );

        cairo_surface_t *surface = NULL; /*Surface sur laquelle on va dessiner*/
        cairo_t *cr = NULL; /*COntexte associé à la surface */

        if( strcmp( filename, "" ) != 0 ) /*On test si le nom a été renseigné */
        {
            if( strcmp( gtk_combo_box_get_active_text( GTK_COMBO_BOX( ew->comboBox ) ), "png" ) == 0 )
            {
                surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, scene->dWidth, scene->dHeight);
                cr = cairo_create (surface);
                Scene_clear_scene(scene , cr, 1, 1, 1); /* Nettoyage de la scene */
                Scene_dessiner_scene( scene, cr ); /*Dessin de tous les objets*/
                cairo_surface_write_to_png(surface, filename ); /* Projection sur une surfae PNG*/
                cairo_surface_destroy(surface);
                g_free (filename);
                gtk_widget_destroy (dialog);
            }
            else if(strcmp( gtk_combo_box_get_active_text( GTK_COMBO_BOX( ew->comboBox ) ), "pdf" ) == 0 )
            {
                surface  = cairo_pdf_surface_create( filename, scene->dWidth, scene->dHeight);
                cr = cairo_create(surface);
                Scene_clear_scene(scene , cr, 1, 1, 1); /* Nettoyage de la scene */
                Scene_dessiner_scene( scene, cr ); /*Dessin de tous les objets*/
                cairo_surface_destroy(surface);
                g_free (filename);
                gtk_widget_destroy (dialog);
            }
            else if(strcmp( gtk_combo_box_get_active_text( GTK_COMBO_BOX( ew->comboBox ) ), "svg" ) == 0 )
            {
                surface = cairo_svg_surface_create( filename, scene->dWidth, scene->dHeight );
                cr = cairo_create(surface);
                Scene_clear_scene(scene , cr, 1, 1, 1); /* Nettoyage de la scene */
                Scene_dessiner_scene( scene, cr ); /*Dessin de tous les objets*/
                cairo_surface_destroy(surface);
                g_free (filename);
                gtk_widget_destroy (dialog);
            }


            cairo_destroy( cr );
        }
        else
        {
            GtkWidget* avertissement =
            gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Veuillez entrer un nom de fichier" );

            if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
            {
                gtk_widget_destroy( avertissement );

            }
        }

    }
    else
    {
          gtk_widget_destroy (dialog);
    }

}
