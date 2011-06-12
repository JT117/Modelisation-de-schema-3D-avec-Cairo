#include "FenetrePropriete.h"
#include "Selection.h"

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

    fp->objet = g_array_index( scene->selection->tSelection, Objet*, 0 );

    char buf[255];
    sprintf( buf, "%.3f", fp->objet->type.cube->tPoint[0].x );
    char buf1[255];
    sprintf( buf1, "%.3f", fp->objet->type.cube->tPoint[0].y );
    char buf2[255];
    sprintf( buf2, "%.3f", fp->objet->type.cube->tPoint[0].z );

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
    fp->boutonAnnuler = gtk_button_new_with_label("Annuler");

    fp->barreBouton = gtk_hbutton_box_new();
    gtk_button_box_set_layout( GTK_BUTTON_BOX( fp->barreBouton ), GTK_BUTTONBOX_END );

    gtk_container_add( GTK_CONTAINER( fp->barreBouton ), fp->boutonOk );
    gtk_container_add( GTK_CONTAINER( fp->barreBouton ), fp->boutonAnnuler );

    g_object_ref( fp->barreBouton );

    //*******************************************************************

    fp->comboBoxGroupe = gtk_combo_box_text_new();

    int i = 0;
    int numGroupe = 0;

    for( i = 0; i < scene->nbGroupe; i++ )
    {
        Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, i );
        gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( fp->comboBoxGroupe ), groupe->nom );

        if( fp->objet->numeroGroupe == groupe->id )
        {
            numGroupe = i;
        }
    }

    gtk_widget_set_size_request( GTK_WIDGET( fp->comboBoxGroupe ), 100, -1 );
    gtk_combo_box_set_active( GTK_COMBO_BOX( fp->comboBoxGroupe ), numGroupe );
    g_signal_emit_by_name( GTK_OBJECT( fp->comboBoxGroupe ), "changed", NULL );

     //*************** Barre de couleur *******************************
    fp->entryR = gtk_entry_new();
    fp->entryG = gtk_entry_new();
    fp->entryB = gtk_entry_new();
    fp->entryA = gtk_entry_new();

    GtkWidget* lab = gtk_label_new("Couleur RGBA [0-255]:");

    gtk_widget_set_size_request( fp->entryR, 30, -1 );
    gtk_widget_set_size_request( fp->entryG, 30, -1 );
    gtk_widget_set_size_request( fp->entryB, 30, -1 );
    gtk_widget_set_size_request( fp->entryA, 30, -1 );

    Color tColor;
    Objet_get_Color( fp->objet, tColor );

    char buf4[255];
    sprintf( buf4, "%.0f", tColor[0]*255 );
    char buf5[255];
    sprintf( buf5, "%.0f", tColor[1]*255 );
    char buf6[255];
    sprintf( buf6, "%.0f", tColor[2]*255 );
    char buf7[255];
    sprintf( buf7, "%.0f", tColor[3]);

    gtk_entry_set_text( GTK_ENTRY( fp->entryR ), buf4 );
    gtk_entry_set_text( GTK_ENTRY( fp->entryG ), buf5 );
    gtk_entry_set_text( GTK_ENTRY( fp->entryB ), buf6 );
    gtk_entry_set_text( GTK_ENTRY( fp->entryA ), buf7 );

    fp->barreCouleur = gtk_hbutton_box_new();
    gtk_button_box_set_layout( GTK_BUTTON_BOX( fp->barreCouleur ), GTK_BUTTONBOX_END );

    gtk_container_add( GTK_CONTAINER( fp->barreCouleur ), lab );
    gtk_container_add( GTK_CONTAINER( fp->barreCouleur ), fp->entryR );
    gtk_container_add( GTK_CONTAINER( fp->barreCouleur ), fp->entryG );
    gtk_container_add( GTK_CONTAINER( fp->barreCouleur ), fp->entryB );
    gtk_container_add( GTK_CONTAINER( fp->barreCouleur ), fp->entryA );

    g_object_ref( fp->barreCouleur );

//*********************************************************************************************
    fp->layout = gtk_vbox_new( FALSE, 10 );

    gtk_container_add( GTK_CONTAINER( fp->layout ), fp->barreObjet );
    gtk_container_add( GTK_CONTAINER( fp->layout ), fp->barreCouleur );
    gtk_container_add( GTK_CONTAINER( fp->layout ), fp->comboBoxGroupe );
    gtk_container_add( GTK_CONTAINER( fp->layout ), fp->barreBouton );

    gtk_container_add( GTK_CONTAINER( fp->fenetre ), fp->layout );

    gtk_widget_show_all(fp->fenetre);

    g_signal_connect_object( G_OBJECT( fp->boutonAnnuler ), "clicked", G_CALLBACK( gtk_widget_destroy ), fp->fenetre, G_CONNECT_SWAPPED );
    g_signal_connect( G_OBJECT( fp->boutonOk ), "clicked", G_CALLBACK( FenetrePropriete_validation ), fp );

}

void FenetrePropriete_validation( GtkButton* button, gpointer data )
{
    FenetrePropriete* fp = (FenetrePropriete*)data;

    double dR,dG,dB,dA = 0;

    /* Maj des couleurs */
	dR = atof( gtk_entry_get_text( GTK_ENTRY( fp->entryR ) ) );
	dG = atof( gtk_entry_get_text( GTK_ENTRY( fp->entryG ) ) );
	dB = atof( gtk_entry_get_text( GTK_ENTRY( fp->entryB ) ) );
	dA = atof( gtk_entry_get_text( GTK_ENTRY( fp->entryA ) ) );

    Objet_set_Color( fp->objet, dR, dG, dB ,dA);

    Groupe* groupe = Groupe_trouver( fp->scene, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(fp->comboBoxGroupe) ) ) ;
    Groupe* ancien = Groupe_trouver_ById( fp->scene, fp->objet->numeroGroupe );

    if( groupe->id != fp->objet->numeroGroupe )
    {
        Groupe_enlever_objet( ancien, fp->objet );
        Groupe_ajouter_Objet( groupe, fp->objet );

        gtk_tree_selection_unselect_iter( fp->scene->treeSelection, fp->objet->iter );
        gtk_tree_store_remove( fp->scene->store, fp->objet->iter );
        gtk_tree_store_append( fp->scene->store, fp->objet->iter, groupe->iter );
        gtk_tree_store_set( fp->scene->store, fp->objet->iter, GROUPE, "Cube", -1);
        gtk_tree_selection_select_iter( fp->scene->treeSelection, fp->objet->iter );

    }

    gtk_widget_queue_draw( fp->scene->zoneDeDessin );
    Selection_deselectionner_tout( fp->scene );
    gtk_widget_destroy( fp->fenetre );

}
