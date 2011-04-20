#include "FenetreAjoutCube.h"

void initialier_FenetreAjoutCube( FenetreAjoutCube* fao )
{
    fao->fenetre = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    fao->comboBox = gtk_combo_box_new_text();
    fao->text = gtk_label_new("Selectionner le type d'objet : ");
    fao->vBox = gtk_vbox_new( TRUE, 20 );
    fao->hBox = gtk_hbox_new( TRUE, 20 );

    gtk_window_set_position( GTK_WINDOW( fao->fenetre ), GTK_WIN_POS_CENTER );
    gtk_window_set_modal( GTK_WINDOW( fao->fenetre ), TRUE );
    gtk_window_set_title( GTK_WINDOW( fao->fenetre ), "Création d'un nouvel objet" );

    gtk_combo_box_append_text( GTK_COMBO_BOX(fao->comboBox), "Cube" );
    gtk_combo_box_append_text( GTK_COMBO_BOX(fao->comboBox), "Test" );

    gtk_container_add( GTK_CONTAINER( fao->hBox ), fao->text );
    gtk_container_add( GTK_CONTAINER( fao->hBox ), fao->comboBox );
    gtk_container_add( GTK_CONTAINER( fao->vBox ), fao->hBox );
    gtk_container_add( GTK_CONTAINER( fao->fenetre ), fao->vBox  );

    g_signal_connect( G_OBJECT( fao->comboBox ), "changed", G_CALLBACK( selection_change ), fao);

    gtk_widget_show_all(fao->fenetre);
}

static gboolean selection_change( GtkWidget *comboBox, gpointer data )
{
    FenetreAjoutCube* fao = (FenetreAjoutCube*)data;

    if( strcmp( "Cube", gtk_combo_box_get_active_text( GTK_COMBO_BOX(comboBox) ) ) == 0 )
    {
        printf("coucou \n");
    }
}
