#include "FenetreAjoutCube.h"
#include "Config.h"

static gboolean nouvel_ajout( GtkButton* button, gpointer data );

void initialier_FenetreAjoutCube( FenetreAjoutCube* fao, Scene* scene )
{
    fao->fenetre = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    fao->scene = scene;

    fao->entry = gtk_entry_new();
    fao->entry1 = gtk_entry_new();
    fao->entry2 = gtk_entry_new();
    fao->entry3 = gtk_entry_new();

    fao->boutonOk = gtk_button_new_with_label("OK");
    fao->boutonAnnuler = gtk_button_new_with_label("Annuler");

    gtk_window_set_position( GTK_WINDOW( fao->fenetre ), GTK_WIN_POS_CENTER );
    gtk_window_set_modal( GTK_WINDOW( fao->fenetre ), TRUE );
    gtk_window_set_title( GTK_WINDOW( fao->fenetre ), "Création d'un nouvel objet" );

    GtkWidget* text = gtk_label_new("Taille des cotés en pixel : ");
    GtkWidget* text1 = gtk_label_new("Position X : ");
    GtkWidget* text2 = gtk_label_new("Position Y : ");
    GtkWidget* text3 = gtk_label_new("Position Z : ");

    GtkWidget* vBox = gtk_vbox_new( TRUE, 20 );
    GtkWidget* hBox = gtk_hbox_new( TRUE, 20 );
    GtkWidget* hBox1 = gtk_hbox_new( TRUE, 20 );
    GtkWidget* hBox2 = gtk_hbox_new( TRUE, 20 );
    GtkWidget* hBox3 = gtk_hbox_new( TRUE, 20 );
    GtkWidget* hBox4 = gtk_hbox_new( TRUE, 20 );

    gtk_container_add( GTK_CONTAINER( hBox ), text );
    gtk_container_add( GTK_CONTAINER( hBox ), fao->entry );

    char buf[255];
    sprintf( buf, "%.3f", fao->scene->creation->x );
    char buf1[255];
    sprintf( buf1, "%.3f", fao->scene->creation->y );
    char buf2[255];
    sprintf( buf2, "%.3f", fao->scene->creation->z );

    gtk_container_add( GTK_CONTAINER( hBox1 ), text1 );
    gtk_container_add( GTK_CONTAINER( hBox1 ), fao->entry1 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry1 ), buf );

    gtk_container_add( GTK_CONTAINER( hBox2 ), text2 );
    gtk_container_add( GTK_CONTAINER( hBox2 ), fao->entry2 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry2 ), buf1 );

    gtk_container_add( GTK_CONTAINER( hBox3 ), text3 );
    gtk_container_add( GTK_CONTAINER( hBox3 ), fao->entry3 );
    gtk_entry_set_text( GTK_ENTRY( fao->entry3 ), buf2 );

    gtk_container_add( GTK_CONTAINER( hBox4 ), fao->boutonOk );
    gtk_container_add( GTK_CONTAINER( hBox4 ), fao->boutonAnnuler );

    gtk_container_add( GTK_CONTAINER( vBox ), hBox );
    gtk_container_add( GTK_CONTAINER( vBox ), hBox1 );
    gtk_container_add( GTK_CONTAINER( vBox ), hBox2 );
    gtk_container_add( GTK_CONTAINER( vBox ), hBox3 );
    gtk_container_add( GTK_CONTAINER( vBox ), hBox4 );

    gtk_container_add( GTK_CONTAINER( fao->fenetre ), vBox  );

    g_signal_connect_object( G_OBJECT( fao->boutonAnnuler ), "clicked", G_CALLBACK( gtk_widget_destroy ), fao->fenetre, G_CONNECT_SWAPPED );
    g_signal_connect( G_OBJECT( fao->boutonOk ), "clicked", G_CALLBACK( nouvel_ajout ), fao );
    g_signal_connect( G_OBJECT( fao->fenetre ), "delete-event", G_CALLBACK( gtk_widget_destroy ), NULL );

    gtk_widget_show_all(fao->fenetre);
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

    return TRUE;
}





