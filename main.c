#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>

#include "Cube.h"
#include "Objet.h"
#include "FenetreAjoutCube.h"
#include "Scene.h"
#include "Config.h"

static gboolean realize_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data);
static gboolean expose_event_callback (GtkWidget *widget, GdkEventExpose *event,gpointer data);
static gboolean gestion_clavier(GtkWidget *window, GdkEventKey* event, gpointer data);
static gboolean gestion_souris_callback(GtkWidget *window, GdkEventButton* event, gpointer data);
static gboolean nouveau_cube( GtkWidget *menuItem, gpointer data );
static gboolean main_ouvrir( GtkWidget *menuItem, gpointer data );
static gboolean main_sauvegarder( GtkWidget *menuItem, gpointer data );


 int main (int argc, char *argv[])
 {
    GtkWidget* mainWindow = NULL;                                               // Instantation de la fenetre principal
    GtkWidget* zoneDeDessin = NULL;
    Scene* scene = NULL;

    gtk_init( &argc, &argv );

    GtkWidget* main_box = gtk_vbox_new(FALSE, 0);

    GtkWidget* menuBarre = gtk_menu_bar_new();
    GtkWidget* menu = gtk_menu_new();
    GtkWidget* fichier = gtk_menu_item_new_with_label( "Fichier" );
    GtkWidget *ouvrir = gtk_menu_item_new_with_label( "Ouvrir" );
    GtkWidget *sauvegarder = gtk_menu_item_new_with_label( "Sauvegarder" );
    gtk_menu_item_set_submenu(GTK_MENU_ITEM (fichier), menu);

    gtk_menu_attach( GTK_MENU(menu), ouvrir, 0, 1, 0, 1 );                                             // Initialisation de GTK
    gtk_menu_attach( GTK_MENU(menu), sauvegarder, 0, 1, 1, 2 );
    gtk_menu_bar_append(GTK_MENU_BAR (menuBarre), fichier);

    mainWindow = gtk_window_new( GTK_WINDOW_TOPLEVEL );                         // appel au constructeur de la fenetre
    zoneDeDessin = gtk_drawing_area_new ();

    gtk_container_add( GTK_CONTAINER( main_box ), menuBarre );
    gtk_container_add( GTK_CONTAINER( main_box ), zoneDeDessin );

    gtk_widget_set_size_request( zoneDeDessin, 1000, 900 );//taille minimum de la zone de dessin
    gtk_container_add( GTK_CONTAINER( mainWindow ), main_box );             // Ajout de la zone de dessin dans le mainWindow

    gtk_widget_show_all( mainWindow );



    //*****************************************************************************************
    Cube* cube1 = (Cube*)malloc( 1 * sizeof( Cube ) );
    initialiser_Cube( cube1, 200, 200, 0, 250 );

    Cube* cube2 = (Cube*)malloc( 1 * sizeof( Cube ) );
    initialiser_Cube( cube2, 300, 300, 0, 250 );


    scene = (Scene*)malloc( 1 * sizeof( Scene) );
    Scene_initialiser_scene( scene, zoneDeDessin );

    Scene_ajouter_cube( scene, cube1 );
    Scene_ajouter_cube( scene, cube2 );

    //*******************************************************************************************

    gtk_window_set_title( GTK_WINDOW( mainWindow), "Sch3Dma" );          // Nom totalement provisiore ^^
    gtk_window_set_default_size( GTK_WINDOW( mainWindow ), 1000, 900 );
    gtk_window_set_position( GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER );

    gtk_widget_add_events( zoneDeDessin, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events( zoneDeDessin, GDK_BUTTON_RELEASE_MASK);   //active la detection de la souris
    gtk_widget_add_events( zoneDeDessin, GDK_BUTTON1_MOTION_MASK);

    g_signal_connect( G_OBJECT( zoneDeDessin ), "realize", G_CALLBACK( realize_callback ), NULL );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "expose-event", G_CALLBACK( expose_event_callback ), scene );
    g_signal_connect( G_OBJECT( mainWindow ), "delete-event", G_CALLBACK( gtk_main_quit ), NULL );
    g_signal_connect( G_OBJECT( mainWindow ), "key-press-event", G_CALLBACK(gestion_clavier), scene);
    g_signal_connect( G_OBJECT( mainWindow ), "key-release-event", G_CALLBACK( gestion_clavier ), scene );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "button-press-event", G_CALLBACK( gestion_souris_callback ), scene );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "button-release-event", G_CALLBACK( gestion_souris_callback ), scene );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "motion-notify-event", G_CALLBACK( gestion_souris_callback ), scene );

    g_signal_connect( G_OBJECT( ouvrir ), "activate", G_CALLBACK( main_ouvrir ), scene);
    g_signal_connect( G_OBJECT( sauvegarder ), "activate", G_CALLBACK( main_sauvegarder ), scene);
    printf("coucou 6\n");

    gtk_main();

    Scene_detruire( scene );                                                             // appel du main GTK

    return EXIT_SUCCESS;

 }

static gboolean expose_event_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    //printf("Expose Event \n");
    Scene* scene = (Scene*)data;
    cairo_t* cr = gdk_cairo_create ( widget->window );

    Scene_clear_scene( scene , cr ); // A voir si necessaire

    Scene_dessiner_scene( scene, cr );

    Selection_dessiner_rectangle( scene->selection, cr );

    cairo_destroy( cr );

    return TRUE;
}

static gboolean realize_callback( GtkWidget *widget, GdkEventExpose *event, gpointer data )
{
    printf("realize event \n");
    return TRUE;
}

static gboolean gestion_clavier(GtkWidget *window, GdkEventKey* event, gpointer data)
{
    Scene* scene = (Scene*) data;

    if( event->type == GDK_KEY_PRESS )
    {
        Scene_touche_appuyer( scene, gdk_keyval_name(event->keyval) );

        if( strcmp( gdk_keyval_name(event->keyval), "Right") == 0 )
        {
            //rotation_Cube( data, M_PI/2, 2 );
            gtk_widget_queue_draw( window );
        }
        else if( strcmp( gdk_keyval_name(event->keyval), "a") == 0 )
        {
            int i = 0;

            for( i = 0; i < scene->nbTouche; i++ )
            {
                if( strcmp( g_array_index( scene->tTouche, char*, i ), "Control_L") == 0 )
                {
                    Selection_selectionner_tout( scene );
                }
            }
            gtk_widget_queue_draw( window );
        }
        else if( strcmp( gdk_keyval_name(event->keyval), "Escape") == 0 )
        {
            Selection_deselectionner_tout( scene->selection );
            gtk_widget_queue_draw( window );
        }
    }
    else if( event->type == GDK_KEY_RELEASE )
    {
        Scene_touche_relacher( scene, gdk_keyval_name(event->keyval) );
    }

    return TRUE;
}

static gboolean gestion_souris_callback(GtkWidget *widget, GdkEventButton* event, gpointer data)
{
    Scene* scene = (Scene*)data;

    if( event->type == GDK_BUTTON_PRESS && event->button == 1 )
    {
        scene->selection->departSelection.x = event->x;
        scene->selection->departSelection.y = event->y;
        Selection_selectionner_objet( scene, event->x, event->y );

        gtk_widget_queue_draw( widget );
    }
    else if( event->type == GDK_BUTTON_PRESS && event->button == 3 )
    {
        GtkWidget *menu = gtk_menu_new();
        GtkWidget *pItem = gtk_menu_item_new_with_label("Nouvel objet");
        GtkWidget *pItem2 = gtk_menu_item_new_with_label("Propriete");

        GtkWidget *sousMenu1 = gtk_menu_new();
        GtkWidget *pItem3 = gtk_menu_item_new_with_label("Cube");
        gtk_menu_attach( GTK_MENU(sousMenu1), pItem3, 0, 1, 0, 1 );

        gtk_menu_item_set_submenu( GTK_MENU_ITEM(pItem), sousMenu1 );

        gtk_menu_attach( GTK_MENU(menu), pItem, 0, 1, 0, 1 );
        gtk_menu_attach( GTK_MENU(menu), pItem2, 0, 1, 1, 2 );

        gtk_widget_show_all(menu);

        Scene_creation_objet( scene, event->x, event->y );

        gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event->button, event->time);
        g_signal_connect( G_OBJECT( pItem3 ), "activate", G_CALLBACK(nouveau_cube), scene);
    }
    else if( event->type == GDK_MOTION_NOTIFY )    // Attention probleme performances !!!!!!!!!!!!!!
    {
        scene->selection->finSelection.x = event->x;
        scene->selection->finSelection.y = event->y;
        Selection_selectionner_click_drag( scene );
        scene->selection->selection_en_cours = TRUE;

        gtk_widget_queue_draw( widget );
    }
    else if( event->type == GDK_BUTTON_RELEASE && event->button == 1 )
    {
        scene->selection->selection_en_cours = FALSE;

        gtk_widget_queue_draw( widget );
    }

    return TRUE;
}

static gboolean nouveau_cube( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;

    FenetreAjoutCube* fao = (FenetreAjoutCube*)malloc( 1 *sizeof( FenetreAjoutCube ) );
    initialier_FenetreAjoutCube( fao, scene );

    return TRUE;
}

static gboolean main_ouvrir( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;

    GtkWidget* opener = gtk_file_chooser_dialog_new ("Ouvrir un fichier de sauvegarde ...", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

    GtkFileFilter* filtre = gtk_file_filter_new();
    gtk_file_filter_add_pattern( GTK_FILE_FILTER (filtre), "*.txt");
    gtk_file_chooser_set_filter( GTK_FILE_CHOOSER(opener), GTK_FILE_FILTER(filtre) );

    if (gtk_dialog_run (GTK_DIALOG (opener)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (opener));

        FILE* fichier = fopen( filename, "r+");
        g_free (filename);

        if( fichier != NULL )
        {
            char deb[5] = "     ";
            fscanf( fichier, "%s", deb );

            if( strcmp( deb, "DEBUT" ) == 0 )
            {
                char typeObjet[20];
                fscanf( fichier, "%s", typeObjet );

                while( strcmp( typeObjet, "FIN") != 0 )
                {
                    int groupe = 0;
                    float x,y,z = 0;
                    float taille = 0;
                    float r,g,b,a = 0;

                    fscanf( fichier, "%s", typeObjet );
                    fscanf( fichier, "%f %f %f", &x, &y, &z );
                    fscanf( fichier, "%f", &taille );
                    fscanf( fichier, "%f %f %f %f", &r, &g, &b, &a );

                    Cube* cube = (Cube*)malloc( 1 * sizeof( Cube ) );
                    initialiser_Cube( cube, x, y, z, taille );
                    Scene_ajouter_cube( scene, cube );

                    fscanf( fichier, "%s", typeObjet );
                }
            }
            fclose( fichier );
        }
    }
    gtk_widget_destroy (opener);
    gtk_widget_queue_draw( scene->zoneDeDessin );
    return TRUE;
}

static gboolean main_sauvegarder( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;

    if( scene->nbObjet > 0 )
    {
        GtkWidget *dialog = gtk_file_chooser_dialog_new ("Sauvegarder la scene", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
        gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
        GtkFileFilter* filtre = gtk_file_filter_new();
        gtk_file_filter_add_pattern( GTK_FILE_FILTER (filtre), "*.txt");
        gtk_file_chooser_set_filter( GTK_FILE_CHOOSER(dialog), GTK_FILE_FILTER(filtre) );

        if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
        {
            char *filename;
            FILE* fichier = NULL;
            filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
            fichier = fopen( filename, "w+" );
            g_free (filename);

            if( fichier != NULL )
            {
                int i = 0;
                fprintf( fichier, "DEBUT\n");

                for( i = 0; i < scene->nbObjet; i++ )
                {
                    Objet* objet = (Objet*)g_array_index( scene->tObjet, Objet*, i );
                    fprintf( fichier, "%s\n%d\n", objet->typeObjet, 0 ); // A changer quand l'implementation du groupe sera faite

                    if( strcmp( objet->typeObjet, "Cube") == 0 )
                    {
                        fprintf( fichier, "%f %f %f\n", objet->type.cube->tPoint[0].x, objet->type.cube->tPoint[0].y, objet->type.cube->tPoint[0].z );
                        fprintf( fichier, "%f\n", objet->type.cube->tPoint[1].x - objet->type.cube->tPoint[0].x );
                        fprintf( fichier, "%f %f %f %f\n", 0.150, 0.150, 0.150, 0.150 );
                    }
                }
                fprintf( fichier, "FIN");
                fclose( fichier );
            }

        }
        gtk_widget_destroy (dialog);

    }
    else
    {
        printf("Scene vide : sauvegarde inutile\n");
    }
    return TRUE;
}












