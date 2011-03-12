#include <stdlib.h>
#include <gtk/gtk.h>

#include "Cube.h"

static gboolean realize_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data);
static gboolean expose_event_callback (GtkWidget *widget, GdkEventExpose *event,gpointer data);

 int main (int argc, char *argv[])
 {
    GtkWidget* mainWindow = NULL;                                               // Instantation de la fenetre principal
    GtkWidget* zoneDeDessin = NULL;

    gtk_init( &argc, &argv );                                                   // Initialisation de GTK

    mainWindow = gtk_window_new( GTK_WINDOW_TOPLEVEL );                         // appel au constructeur de la fenetre
    zoneDeDessin = gtk_drawing_area_new ();                                     // appel au constructeur de la zone de dessin

    gtk_widget_set_size_request( zoneDeDessin, 1000, 900 );                      //taille minimum de la zone de dessin
    gtk_container_add( GTK_CONTAINER( mainWindow ), zoneDeDessin );             // Ajout de la zone de dessin dans le mainWindow

    g_signal_connect( G_OBJECT( mainWindow ), "delete-event", G_CALLBACK( gtk_main_quit ), NULL );    // signal pour quitter

    gtk_widget_show_all( mainWindow );                                          // Fonction d'affichage

    g_signal_connect( G_OBJECT( zoneDeDessin ), "realize", G_CALLBACK( realize_callback ), NULL );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "expose-event", G_CALLBACK( expose_event_callback ), NULL );

    gtk_window_set_title( GTK_WINDOW( mainWindow), "Sch3Dma" );                 // Nom totalement provisiore ^^
    gtk_window_set_default_size( GTK_WINDOW( mainWindow ), 1000, 900 );
    gtk_window_set_position( GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER );

    gtk_main();                                                                 // appel du main GTK

    return EXIT_SUCCESS;

 }

static gboolean expose_event_callback (GtkWidget *widget, GdkEventExpose *event,gpointer data)
{
    cairo_t *cr = NULL;
    cr = gdk_cairo_create (widget->window);
    printf("Expose event \n");

    cairo_move_to (cr, 0, 0);

    /*cairo_rectangle_t rect1;   // Structure contenant les infos sur un rectangle
    rect1.x = 200;
    rect1.y = 200;
    rect1.width = 250;
    rect1.height = 250;

    cairo_rectangle_t rect2;    // Structure contenant les infos sur un rectangle
    rect2.x = 250;
    rect2.y = 250;
    rect2.width = 250;
    rect2.height = 250;

    cairo_rectangle (cr, rect1.x, rect1.y, rect1.width, rect1.height); // On dessine le rectangle sur la surface
    cairo_rectangle (cr, rect2.x, rect2.y, rect2.width, rect2.height); // On dessine le rectangle sur la surface

    cairo_line_to( cr, rect1.x, rect1.y );                             // On dessine à partir des coordonnées du cr jusqu'au point donné

    cairo_move_to( cr, rect2.x + rect2.width, rect2.y + rect2.height ); // On deplace le curseur de cr
    cairo_line_to( cr, rect1.x + rect1.width, rect1.y + rect2.height ); // On dessine à partir des coordonnées du cr jusqu'au point donné

    cairo_move_to( cr, rect2.x + rect2.width, rect2.y );
    cairo_line_to( cr, rect1.x + rect1.width, rect1.y );

    cairo_move_to( cr, rect2.x, rect2.y + rect2.height );
    cairo_line_to( cr, rect1.x, rect1.y + rect2.height );

    cairo_set_source_rgb (cr, 0, 255, 0);   // On definie la couleur du trait

    cairo_stroke(cr); // On ne dessine que les contours*/

    Cube cube1;
    initialiser_Cube( &cube1, 200, 200, 250, 250, 250, 250, 250, 250, cr );
    printf("cube initialiser \n");
    dessiner_Cube( &cube1 );
    printf("cube dessiner \n");

    return TRUE;
}

static gboolean realize_callback( GtkWidget *widget, GdkEventExpose *event, gpointer data )
{
    cairo_t *cr;
    cr = gdk_cairo_create (widget->window);
printf("realize event \n");

    cairo_set_line_width (cr, 0.1);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_rectangle (cr, 0.25, 0.25, 0.5, 0.5);
    cairo_stroke (cr);

    return TRUE;
}
