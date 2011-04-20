#include "FenetreAjoutCube.h"

void initialier_FenetreAjoutCube( FenetreAjoutCube* fao, Scene* scene )
{
    fao->fenetre = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    fao->scene = scene;

    GtkWidget* entry = gtk_entry_new();
    GtkWidget* text = gtk_label_new("Taille des cotés en pixel : ");

    GtkWidget* entry1 = gtk_entry_new();
    GtkWidget* text1 = gtk_label_new("Position X : ");

    GtkWidget* entry2 = gtk_entry_new();
    GtkWidget* text2 = gtk_label_new("Position Y : ");

    GtkWidget* entry3 = gtk_entry_new();
    GtkWidget* text3 = gtk_label_new("Position Z : ");

    GtkWidget* boutonOk = gtk_button_new_with_label("OK");
    GtkWidget* boutonAnnuler = gtk_button_new_with_label("Annuler");

    GtkWidget* vBox = gtk_vbox_new( TRUE, 20 );
    GtkWidget* hBox = gtk_hbox_new( TRUE, 20 );
    GtkWidget* hBox1 = gtk_hbox_new( TRUE, 20 );
    GtkWidget* hBox2 = gtk_hbox_new( TRUE, 20 );
    GtkWidget* hBox3 = gtk_hbox_new( TRUE, 20 );
    GtkWidget* hBox4 = gtk_hbox_new( TRUE, 20 );

    gtk_window_set_position( GTK_WINDOW( fao->fenetre ), GTK_WIN_POS_CENTER );
    gtk_window_set_modal( GTK_WINDOW( fao->fenetre ), TRUE );
    gtk_window_set_title( GTK_WINDOW( fao->fenetre ), "Création d'un nouvel objet" );

    gtk_container_add( GTK_CONTAINER( hBox ), text );
    gtk_container_add( GTK_CONTAINER( hBox ), entry );

    char buf[255];
    sprintf( buf, "%.3f", scene->creation.x );
    char buf1[255];
    sprintf( buf1, "%.3f", scene->creation.y );
    char buf2[255];
    sprintf( buf2, "%.3f", scene->creation.z );

    gtk_container_add( GTK_CONTAINER( hBox1 ), text1 );
    gtk_container_add( GTK_CONTAINER( hBox1 ), entry1 );
    gtk_entry_set_text( entry1, buf );

    gtk_container_add( GTK_CONTAINER( hBox2 ), text2 );
    gtk_container_add( GTK_CONTAINER( hBox2 ), entry2 );
    gtk_entry_set_text( entry2, buf1 );

    gtk_container_add( GTK_CONTAINER( hBox3 ), text3 );
    gtk_container_add( GTK_CONTAINER( hBox3 ), entry3 );
    gtk_entry_set_text( entry3, buf2 );

    scene->tailleCreation = atof( gtk_entry_get_text( entry ) );

    gtk_container_add( GTK_CONTAINER( hBox4 ), boutonOk );
    gtk_container_add( GTK_CONTAINER( hBox4 ), boutonAnnuler );

    gtk_container_add( GTK_CONTAINER( vBox ), hBox );
    gtk_container_add( GTK_CONTAINER( vBox ), hBox1 );
    gtk_container_add( GTK_CONTAINER( vBox ), hBox2 );
    gtk_container_add( GTK_CONTAINER( vBox ), hBox3 );
    gtk_container_add( GTK_CONTAINER( vBox ), hBox4 );

    gtk_container_add( GTK_CONTAINER( fao->fenetre ), vBox  );

    g_signal_connect_object( G_OBJECT( boutonAnnuler ), "clicked", G_CALLBACK( gtk_widget_destroy ), fao->fenetre, G_CONNECT_SWAPPED );
    g_signal_connect( G_OBJECT( boutonOk ), "clicked", G_CALLBACK( nouvel_ajout ), scene );

    gtk_widget_show_all(fao->fenetre);
}

static gboolean nouvel_ajout( GtkWidget *widget, GdkEvent *event, gpointer data )
{
    Scene* scene = (Scene*)data;
    printf("cocou\n");
    printf( "%f\n", scene->tailleCreation );
    if( scene->tailleCreation > 0 )
    {
        Cube cube;
        initialiser_Cube( &cube, scene->creation.x, scene->creation.y, scene->creation.z, scene->tailleCreation );
        Scene_ajouter_cube( scene, &cube );

    }


}





