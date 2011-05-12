#include "FenetrePropriete.h"

void FenetrePropriete_initialiser( FenetrePropriete* fp, Scene* scene )
{
    fp->fenetre = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    fp->scene = scene;
    fp->dernierLayout = (char*)malloc( 50 * sizeof( char ) );
    fp->dernierLayout = strcpy( fp->dernierLayout, "Initialisation" );

    gtk_window_set_position( GTK_WINDOW( fp->fenetre ), GTK_WIN_POS_CENTER );
    gtk_window_set_modal( GTK_WINDOW( fp->fenetre ), TRUE );
    gtk_window_set_title( GTK_WINDOW( fp->fenetre ), "Propriete de l'objet" );

    fp->entry1 = gtk_entry_new();
    fp->entry2 = gtk_entry_new();
    fp->entry3 = gtk_entry_new();

    gtk_widget_set_size_request(fp->entry1, 50, -1 );
    gtk_widget_set_size_request(fp->entry2, 70, -1 );
    gtk_widget_set_size_request(fp->entry3, 70, -1 );

    GtkWidget* text1 = gtk_label_new("Position X : ");
    GtkWidget* text2 = gtk_label_new("Position Y : ");
    GtkWidget* text3 = gtk_label_new("Position Z : ");

//***************************************************************************

    fp->barreObjet = gtk_hbutton_box_new();

    Objet* objet = g_array_index( scene->selection->tSelection, Objet*, 0 );


    char buf[255];
    sprintf( buf, "%.3f", objet->type.cube->tPoint[0].x );
    char buf1[255];
    sprintf( buf1, "%.3f", objet->type.cube->tPoint[0].y );
    char buf2[255];
    sprintf( buf2, "%.3f", objet->type.cube->tPoint[0].z );

    gtk_container_add( GTK_CONTAINER( fp->barreObjet ), text1 );
    gtk_container_add( GTK_CONTAINER( fp->barreObjet ), fp->entry1 );
    gtk_entry_set_text( GTK_ENTRY( fp->entry1 ), buf );

    gtk_container_add( GTK_CONTAINER( fp->barreObjet ), text2 );
    gtk_container_add( GTK_CONTAINER( fp->barreObjet ), fp->entry2 );
    gtk_entry_set_text( GTK_ENTRY( fp->entry2 ), buf1 );

    gtk_container_add( GTK_CONTAINER( fp->barreObjet ), text3 );
    gtk_container_add( GTK_CONTAINER( fp->barreObjet ), fp->entry3 );
    gtk_entry_set_text( GTK_ENTRY( fp->entry3 ), buf2 );

    gtk_button_box_set_layout( GTK_BUTTON_BOX( fp->barreObjet ), GTK_BUTTONBOX_START );

    g_object_ref( fp->barreObjet );

    //*************** Barre Bouton ***********************************

    fp->boutonOk = gtk_button_new_with_label("OK");

    fp->barreBouton = gtk_hbutton_box_new();
    gtk_button_box_set_layout( GTK_BUTTON_BOX( fp->barreBouton ), GTK_BUTTONBOX_END );

    gtk_container_add( GTK_CONTAINER( fp->barreBouton ), fp->boutonOk );

    g_object_ref( fp->barreBouton );

    //*******************************************************************

    fp->layout = gtk_vbox_new( FALSE, 10 );

    gtk_container_add( GTK_CONTAINER( fp->layout ), fp->barreObjet );
    //gtk_container_add( GTK_CONTAINER( fp->layout ), fp->barreGroupe );
    gtk_container_add( GTK_CONTAINER( fp->layout ), fp->barreBouton );

    gtk_container_add( GTK_CONTAINER( fp->fenetre ), fp->layout );

    gtk_widget_show_all(fp->fenetre);

}

