#include "FenSeg.h"
#include "MouseInterface.h"

void FenSeg_init( FenSeg* fseg, Scene* scene)
{
    fseg->fenetre = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_window_set_modal(fseg->fenetre,TRUE);
    fseg->scene = scene;
    gtk_window_set_position( GTK_WINDOW( fseg->fenetre ), GTK_WIN_POS_CENTER );
	gtk_window_set_title( GTK_WINDOW( fseg->fenetre ), "Type de segment" );

    fseg->layout = gtk_vbox_new( 5, TRUE );

    /* BARE COULEUR */
    fseg->hbox_color = gtk_hbutton_box_new();

    fseg->entryR = gtk_entry_new();
	fseg->entryG = gtk_entry_new();
	fseg->entryB = gtk_entry_new();
	fseg->entryA = gtk_entry_new();

    GtkWidget* lab = gtk_label_new("Couleur RGBA [0-255]:");
	gtk_widget_set_size_request( fseg->entryR, 30, -1 );
	gtk_widget_set_size_request( fseg->entryG, 30, -1 );
	gtk_widget_set_size_request( fseg->entryB, 30, -1 );
	gtk_widget_set_size_request( fseg->entryA, 30, -1 );

	gtk_entry_set_text( GTK_ENTRY( fseg->entryR ), "0" );
	gtk_entry_set_text( GTK_ENTRY( fseg->entryG ), "0" );
	gtk_entry_set_text( GTK_ENTRY( fseg->entryB ), "0" );
	gtk_entry_set_text( GTK_ENTRY( fseg->entryA ), "1" );

	gtk_button_box_set_layout( GTK_BUTTON_BOX( fseg->hbox_color ), GTK_BUTTONBOX_END );

	gtk_container_add( GTK_CONTAINER( fseg->hbox_color ), lab );
	gtk_container_add( GTK_CONTAINER( fseg->hbox_color ), fseg->entryR );
	gtk_container_add( GTK_CONTAINER( fseg->hbox_color ), fseg->entryG );
	gtk_container_add( GTK_CONTAINER( fseg->hbox_color ), fseg->entryB );
	gtk_container_add( GTK_CONTAINER( fseg->hbox_color ), fseg->entryA );


	/* BARRE DES TRUCS A COCHER */
	fseg->hbox_checkSegment = gtk_hbutton_box_new();
	gtk_button_box_set_layout( GTK_BUTTON_BOX( fseg->hbox_checkSegment ), GTK_BUTTONBOX_CENTER );

	fseg->wDashed = gtk_check_button_new_with_label("Pointillé");
	fseg->wArrowed = gtk_check_button_new_with_label("Flêche");
	gtk_container_add( GTK_CONTAINER( fseg->hbox_checkSegment ), fseg->wArrowed );
	gtk_container_add( GTK_CONTAINER( fseg->hbox_checkSegment ), fseg->wDashed );

	/* BARRE  BOUTONS */
	fseg->boutonOk = gtk_button_new_with_label("OK");
	fseg->boutonAnnuler = gtk_button_new_with_label("Annuler");

	fseg->hbox_button = gtk_hbutton_box_new();
	gtk_button_box_set_layout( GTK_BUTTON_BOX( fseg->hbox_button ), GTK_BUTTONBOX_END );

	gtk_container_add( GTK_CONTAINER( fseg->hbox_button ), fseg->boutonOk );
	gtk_container_add( GTK_CONTAINER( fseg->hbox_button ), fseg->boutonAnnuler );


	/* Ajout au container de la fenetre*/
	 gtk_container_add( GTK_CONTAINER( fseg->layout ), fseg->hbox_color );
	 gtk_container_add( GTK_CONTAINER( fseg->layout ), fseg->hbox_checkSegment );
	 gtk_container_add( GTK_CONTAINER( fseg->layout ), fseg->hbox_button );
	 gtk_container_add( GTK_CONTAINER( fseg->fenetre ), fseg->layout );

	gtk_widget_show_all( fseg->fenetre );

    g_signal_connect_object( G_OBJECT( fseg->boutonAnnuler ), "clicked", G_CALLBACK( gtk_widget_destroy ), fseg->fenetre, G_CONNECT_SWAPPED );
    g_signal_connect( G_OBJECT( fseg->boutonOk ), "clicked", G_CALLBACK( FenSeg_newSegment ), fseg );
}

void FenSeg_newSegment( GtkButton* button, gpointer data)
{
	FenSeg* fseg = (FenSeg*)data;
	Scene* scene = fseg->scene;
	Segment* pNewSeg = NULL;
	tCoord tCoord1,tCoord2,tCenter;
	Point sMiddle; /* va contenir un point situé entre les points selectionnés */
	Point *pP1,*pP2;
	double dR,dG,dB,dA;

	pP1 = fseg->pP1;
	pP2 = fseg->pP2;

	/* Récupération des codes couleurs */
	dR = atof( gtk_entry_get_text( GTK_ENTRY( fseg->entryR ) ) );
	dG = atof( gtk_entry_get_text( GTK_ENTRY( fseg->entryG ) ) );
	dB = atof( gtk_entry_get_text( GTK_ENTRY( fseg->entryB ) ) );
	dA = atof( gtk_entry_get_text( GTK_ENTRY( fseg->entryA ) ) );

	/* Création du segment en fonction des points renseignés dans la structure pCreateSeg */
	Point_middlePoint(&sMiddle,pP1, pP2); /* on recupère le point centrale */

	/* Initialisation des coordonnées dans son propre repère*/
	Point_initCoord(tCoord1, pP1->tdCoordWorld[0]-sMiddle.tdCoordWorld[0],pP1->tdCoordWorld[1]-sMiddle.tdCoordWorld[1],
							pP1->tdCoordWorld[2]-sMiddle.tdCoordWorld[2]);
	Point_initCoord(tCoord2,pP2->tdCoordWorld[0]-sMiddle.tdCoordWorld[0],pP2->tdCoordWorld[1]-sMiddle.tdCoordWorld[1],
							pP2->tdCoordWorld[2]-sMiddle.tdCoordWorld[2]);
	Point_initCoord(tCenter,sMiddle.tdCoordWorld[0],sMiddle.tdCoordWorld[1],sMiddle.tdCoordWorld[2]); /* Groupe par défaut -> 0 */
	pNewSeg = Segment_createSegment(tCenter,tCoord1,tCoord2);

	Color_setColor(pNewSeg->tColor,(dR/255),(dG/255),(dB/255),dA);

	if(gtk_toggle_button_get_active(fseg->wArrowed))
		Segment_setArrowed(pNewSeg);

	if(gtk_toggle_button_get_active(fseg->wDashed))
		Segment_setDashed(pNewSeg);

	Scene_ajouter_segment( fseg->scene, pNewSeg, GROUPE0 );

	Modification_modification_effectuer( scene );
	g_signal_emit_by_name( G_OBJECT(fseg->boutonAnnuler), "clicked" );
}
