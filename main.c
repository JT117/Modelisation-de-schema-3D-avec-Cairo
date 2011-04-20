#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>

#include "Cube.h"
#include "Objet.h"
#include "FenetreAjoutCube.h"
#include "Scene.h"

static gboolean realize_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data);
static gboolean expose_event_callback (GtkWidget *widget, GdkEventExpose *event,gpointer data);
static gboolean gestion_clavier(GtkWidget *window, GdkEventKey* event, gpointer data);
static gboolean gestion_souris_callback(GtkWidget *window, GdkEventButton* event, gpointer data);
static gboolean nouveau_cube( GtkWidget *menuItem, gpointer data );


 int main (int argc, char *argv[])
 {
    GtkWidget* mainWindow = NULL;                                               // Instantation de la fenetre principal
    GtkWidget* zoneDeDessin = NULL;
    Scene* scene = NULL;
    cairo_t* cr = NULL;

    gtk_init( &argc, &argv );                                                   // Initialisation de GTK

    mainWindow = gtk_window_new( GTK_WINDOW_TOPLEVEL );                         // appel au constructeur de la fenetre
    zoneDeDessin = gtk_drawing_area_new ();

    gtk_widget_set_size_request( zoneDeDessin, 1000, 900 );                      //taille minimum de la zone de dessin
    gtk_container_add( GTK_CONTAINER( mainWindow ), zoneDeDessin );             // Ajout de la zone de dessin dans le mainWindow

    gtk_widget_show_all( mainWindow );

    g_signal_connect( G_OBJECT( mainWindow ), "delete-event", G_CALLBACK( gtk_main_quit ), NULL );    // signal pour quitter

    Cube cube1;
    initialiser_Cube( &cube1, 200, 200, 0, 250 );

    Cube cube2;
    initialiser_Cube( &cube2, 300, 300, 0, 250 );


    scene = (Scene*)malloc( 1 * sizeof( Scene) );
    Scene_initialiser_scene( scene, zoneDeDessin );

    Scene_ajouter_cube( scene, &cube1 );
    Scene_ajouter_cube( scene, &cube2 );

    g_signal_connect( G_OBJECT( zoneDeDessin ), "realize", G_CALLBACK( realize_callback ), NULL );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "expose-event", G_CALLBACK( expose_event_callback ), scene );

    gtk_window_set_title( GTK_WINDOW( mainWindow), "Sch3Dma" );                 // Nom totalement provisiore ^^
    gtk_window_set_default_size( GTK_WINDOW( mainWindow ), 1000, 900 );
    gtk_window_set_position( GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER );
    gtk_widget_add_events( mainWindow, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events( mainWindow, GDK_BUTTON_RELEASE_MASK);   //active la detection de la souris
    gtk_widget_add_events( mainWindow, GDK_BUTTON1_MOTION_MASK);

    g_signal_connect( G_OBJECT( mainWindow ), "key-press-event", G_CALLBACK(gestion_clavier), scene);
    g_signal_connect( G_OBJECT( mainWindow ), "key-release-event", G_CALLBACK( gestion_clavier ), scene );
    g_signal_connect( G_OBJECT( mainWindow ), "button-press-event", G_CALLBACK( gestion_souris_callback ), scene );
    g_signal_connect( G_OBJECT( mainWindow ), "button-release-event", G_CALLBACK( gestion_souris_callback ), scene );
    g_signal_connect( G_OBJECT( mainWindow ), "motion-notify-event", G_CALLBACK( gestion_souris_callback ), scene );

    gtk_main();                                                                 // appel du main GTK

    return EXIT_SUCCESS;

 }

static gboolean expose_event_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    printf("Expose Event \n");
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

    FenetreAjoutCube fao;
    initialier_FenetreAjoutCube( &fao, scene );

    return TRUE;
}
