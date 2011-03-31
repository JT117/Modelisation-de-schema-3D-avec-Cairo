#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>

#include "Cube.h"
#include "Objet.h"
#include "Scene.h"


static gboolean realize_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data);
static gboolean expose_event_callback (GtkWidget *widget, GdkEventExpose *event,gpointer data);
static gboolean gestion_clavier(GtkWidget *window, GdkEventKey* event, gpointer data);
static gboolean gestion_souris_callback(GtkWidget *window, GdkEventButton* event, gpointer data);


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
    initialiser_Scene( scene, zoneDeDessin );

    ajouter_cube( scene, &cube1 );
    ajouter_cube( scene, &cube2 );

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

    dessiner_Scene( scene, cr );

    if( scene->selection_en_cours )
    {
        cairo_set_source_rgba( cr, 51.0, 255.0, 255.0, 0.25 );
        cairo_rectangle( cr, scene->departSelection.x, scene->departSelection.y, scene->finSelection.x - scene->departSelection.x, scene->finSelection.y - scene->departSelection.y );
        cairo_fill( cr );
    }
    cairo_destroy( cr );

printf("FIN - Expose Event \n");
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
        touche_appuyer( scene, gdk_keyval_name(event->keyval) );

        if( strcmp( gdk_keyval_name(event->keyval), "Right") == 0 )
        {
            //rotation_Cube( data, M_PI/2, 2 );
            gtk_widget_queue_draw( window );
        }
    }
    else if( event->type == GDK_KEY_RELEASE )
    {
        touche_relacher( scene, gdk_keyval_name(event->keyval) );
    }

    return TRUE;
}

static gboolean gestion_souris_callback(GtkWidget *widget, GdkEventButton* event, gpointer data)
{
    Scene* scene = (Scene*)data;

    if( event->type == GDK_BUTTON_PRESS && event->button == 1 )
    {
        scene->departSelection.x = event->x;
        scene->departSelection.y = event->y;
        selectionner_objet( scene, event->x, event->y );

        gtk_widget_queue_draw( widget );
    }
    else if( event->type == GDK_MOTION_NOTIFY )    // Attention probleme performances !!!!!!!!!!!!!!
    {
        printf("Motion Notify \n");
        scene->finSelection.x = event->x;
        scene->finSelection.y = event->y;
        selectionner_click_drag( scene );
        scene->selection_en_cours = TRUE;

        gtk_widget_queue_draw( widget );
        printf("FIN - Motion Notify \n");
    }
    else if( event->type == GDK_BUTTON_RELEASE && event->button == 1 )
    {
        scene->selection_en_cours = FALSE;

        gtk_widget_queue_draw( widget );
    }

    return TRUE;
}
