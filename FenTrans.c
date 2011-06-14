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
    else
    {
        ft->radio4 = gtk_radio_button_new_with_label_from_widget( GTK_RADIO_BUTTON( ft->radio1 ), "Translation sur objet fils" );
        gtk_container_add( GTK_CONTAINER( hbox ), ft->radio4 );
        ft->radio5 = gtk_radio_button_new_with_label_from_widget( GTK_RADIO_BUTTON( ft->radio1 ), "Rotation sur objet fils" );
        gtk_container_add( GTK_CONTAINER( hbox ), ft->radio5 );
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
	Groupe* pSon = NULL; /* Va contenir les adresses des groupes fils d'un groupe */
	Groupe* pGroupe = NULL;
	Objet* pObj = NULL;
    int j=0;
    double dX = 0; double dY = 0; double dZ = 0;
    dX = atof( gtk_entry_get_text( GTK_ENTRY( ft->entry1 ) ) );
	dY = atof( gtk_entry_get_text( GTK_ENTRY( ft->entry2 ) ) );
	dZ = atof( gtk_entry_get_text( GTK_ENTRY( ft->entry3 ) ) );

    if( ft->unGroupeEstSelectionner &&  gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ft->radio1) ) ) //S'applique a un groupe et ses sous groupes
    {	/* rotation GROUPE ET SOUS GROUPES */
    	/* Calcul de l'angle de transformation en radian */
		dX = (dX*M_PI)/180;
		dY = (dY*M_PI)/180;
		dZ = (dZ*M_PI)/180;

        tdMatrix tdTransfoMat,tdNewTransfo;
        Matrix_initIdentityMatrix(tdTransfoMat); /* Initialisation de la matrice de rotation */
        if( dX != 0 )
        {
        	Transformation_getMatrixRotation(tdNewTransfo, dX, AXEX);
			Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
        }

        if( dY != 0 )
        {
            Transformation_getMatrixRotation( tdNewTransfo, dY, AXEY );
            Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
        }

        if( dZ != 0 )
		{
			Transformation_getMatrixRotation( tdNewTransfo, dZ, AXEZ );
			Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
		}


        if( ft->groupeSelectionne->id != GROUPE0 ) // on ne peut pas appliquer de transformations sur le centre du groupe repère du monde
        {
        	/* Application de la transformation au centre du groupe */
        	Groupe_transfoCenter(ft->groupeSelectionne, tdTransfoMat);

        	Transfo* transfo = (Transfo*)malloc( 1 * sizeof( Transfo ) );
			transfo->eTransfoType = ROTATION;
			transfo->x = dX;
			transfo->y = dY;
			transfo->z = dZ;

			g_array_append_val( ft->groupeSelectionne->aTransfo, transfo );
        }
    }
    else if(ft->unGroupeEstSelectionner && gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ft->radio5) )) //S'applique uniquement aux objet du groupe
    {

      	/* Calcul de l'angle de transformation en radian */
		dX = (dX*M_PI)/180;
		dY = (dY*M_PI)/180;
		dZ = (dZ*M_PI)/180;

        tdMatrix tdTransfoMat,tdNewTransfo;
        Matrix_initIdentityMatrix(tdTransfoMat); /* Initialisation de la matrice de rotation */
        if( dX != 0 )
        {
        	Transformation_getMatrixRotation(tdNewTransfo, dX, AXEX);
			Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
        }

        if( dY != 0 )
        {
            Transformation_getMatrixRotation( tdNewTransfo, dY, AXEY );
            Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
        }

        if( dZ != 0 )
		{
			Transformation_getMatrixRotation( tdNewTransfo, dZ, AXEZ );
			Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
		}

        /* Application de la rotation sur tous les groupes fils */
		for( j=0;j<ft->groupeSelectionne->tFils->len;++j)
		{
			pSon = g_array_index(ft->groupeSelectionne->tFils,Groupe*,j);
			Groupe_transfo( pSon, tdTransfoMat);
		}

		/* et pour les objets du groupe */
		for( j=0;j<ft->groupeSelectionne->tObjet->len;++j)
		{
			pObj = g_array_index(ft->groupeSelectionne->tObjet,Objet*,j);
			Objet_transfoCenter(pObj, tdTransfoMat);
			Objet_transfo( pObj , tdTransfoMat);
		}

		/*Une fois que tous les groupes sont visités on remet leur flag bVisited à false pour une prochaine utilisation*/
		for( j = 0; j < ft->groupeSelectionne->tFils->len;++j)
		{
			pSon = g_array_index(ft->groupeSelectionne->tFils,Groupe*,j); /*Récupération du groupe en question*/
			Groupe_unvisit(pSon);
		}

		Transfo* transfo = (Transfo*)malloc( 1 * sizeof( Transfo ) );
		transfo->eTransfoType = ROTATION_RECU;
		transfo->x = dX;
		transfo->y = dY;
		transfo->z = dZ;
		g_array_append_val( ft->groupeSelectionne->aTransfo, transfo );

    }
    else if( ft->unGroupeEstSelectionner && gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ft->radio2) ) )
    {	/* Translation sur centre du groupe */

    	tdMatrix tdTransfoMat;

		Transformation_getMatrixTranslation(tdTransfoMat, dX, dY, dZ);
		if( ft->groupeSelectionne != GROUPE0)
		{
			/* On applique la transfo le centre du groupe */
			Groupe_transfoCenter(ft->groupeSelectionne, tdTransfoMat);

			Transfo* transfo = (Transfo*)malloc( 1 * sizeof( Transfo ) );
			transfo->eTransfoType = ROTATION;
			transfo->x = dX;
			transfo->y = dY;
			transfo->z = dZ;

			g_array_append_val( ft->groupeSelectionne->aTransfo, transfo );
		}

    }
    else if( ft->unGroupeEstSelectionner && gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ft->radio4) ) )
    {/* Translation sur objets et groupes fils */
        tdMatrix tdTransfoMat;

        Transformation_getMatrixTranslation(tdTransfoMat, dX, dY, dZ );

        /* Application de la translation au sous groupes */
        for( j=0;j<ft->groupeSelectionne->tFils->len;++j)
		{
			pGroupe = g_array_index(ft->groupeSelectionne->tFils,Groupe*,j);
			Groupe_transfo(pGroupe, tdTransfoMat);
		}

        for( j=0;j<ft->groupeSelectionne->tObjet->len;++j)
        {
            pObj = g_array_index(ft->groupeSelectionne->tObjet,Objet*,j);
            Objet_transfoCenter(pObj, tdTransfoMat);   // on fait tourner le centre du repre objet
            Objet_transfo( pObj , tdTransfoMat);    // ainsi qu l'intégralité de ses points
        }
        Transfo* transfo = (Transfo*)malloc( 1 * sizeof( Transfo ) );
		transfo->eTransfoType = TRANSLATION_RECU;
		transfo->x = dX;
		transfo->y = dY;
		transfo->z = dZ;
		g_array_append_val( ft->groupeSelectionne->aTransfo, transfo );
    }
    else if(  gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ft->radio3) ) )
    {/* translation sur le objets du groupe*/
        Objet* objet = g_array_index( ft->scene->selection->tSelection, Objet*, 0 );
        tdMatrix tdTransfoMat;
        if( dX > 0 )
        {
            Transformation_getMatrixHomothety( tdTransfoMat, dX);
        }
        else if( dY > 0 )
        {
            Transformation_getMatrixHomothety( tdTransfoMat, dY );
        }

        //Objet_transfoCenter(objet, tdTransfoMat);   // on fait tourner le centre du repre objet
        Objet_transfo( objet , tdTransfoMat);    // ainsi qu l'intégralité de ses points

        Transfo* transfo = (Transfo*)malloc( 1 * sizeof( Transfo ) );
        transfo->eTransfoType = HOMOTHETIE;
        transfo->x = dX;
        transfo->y = dY;
        //transfo->y = dY;

        g_array_append_val( objet->aTransfo, transfo );
    }
    else if( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ft->radio1) ) )   // Rotation d'un objet sur lui même
    {
        Objet* objet = g_array_index( ft->scene->selection->tSelection, Objet*, 0 );
       	/* Calcul de l'angle de transformation en radian */
		dX = (dX*M_PI)/180;
		dY = (dY*M_PI)/180;
		dZ = (dZ*M_PI)/180;

        tdMatrix tdTransfoMat,tdNewTransfo;
        Matrix_initIdentityMatrix(tdTransfoMat); /* Initialisation de la matrice de rotation */
        if( dX != 0 )
        {
        	Transformation_getMatrixRotation(tdNewTransfo, dX, AXEX);
			Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
        }

        if( dY != 0)
        {
            Transformation_getMatrixRotation( tdNewTransfo, dY, AXEY );
            Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
        }

        if( dZ != 0)
		{
			Transformation_getMatrixRotation( tdNewTransfo, dZ, AXEZ );
			Matrix_multiMatrices(tdTransfoMat, tdNewTransfo);  /* Résutlat contenu dans tdTransfoMat */
		}

        //Objet_transfoCenter(objet, tdTransfoMat);   // on fait tourner le centre du repre objet
        Objet_transfo( objet , tdTransfoMat);    // ainsi qu l'intégralité de ses points

        Transfo* transfo = (Transfo*)malloc( 1 * sizeof( Transfo ) );
        transfo->eTransfoType = ROTATION;
        transfo->x = dX;
        transfo->y = dY;
        transfo->z = dZ;

        g_array_append_val( objet->aTransfo, transfo );
    }
    else if( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ft->radio2 ) ) )
    {
        Objet* objet = g_array_index( ft->scene->selection->tSelection, Objet*, 0 );

        tdMatrix tdTransfoMat;
        Transformation_getMatrixTranslation(tdTransfoMat, dX, dY, dZ );

        Objet_transfoCenter(objet, tdTransfoMat);   // on fait tourner le centre du repre objet
        //Objet_transfo( objet , tdTransfoMat);    // ainsi qu l'intégralité de ses points

        Transfo* transfo = (Transfo*)malloc( 1 * sizeof( Transfo ) );
        transfo->eTransfoType = TRANSLATION;
        transfo->x = dX;
        transfo->y = dY;

        g_array_append_val( objet->aTransfo, transfo );

    }

    gtk_widget_queue_draw( ft->scene->zoneDeDessin );
    gtk_widget_destroy( ft->fenetre );
    Selection_deselectionner_tout( ft->scene );
}
