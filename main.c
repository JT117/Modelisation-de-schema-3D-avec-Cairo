#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>

#include "FenetreAjoutCube.h" /* TODO : réflechir à un moyen de ne faire qu'un seule module FenetreAjout */
#include "FenetreAjoutRectangle.h"
#include "Cube.h"
#include "Objet.h"
#include "Scene.h"
#include "Config.h"

static gboolean realize_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data);
static gboolean expose_event_callback (GtkWidget *widget, GdkEventExpose *event,gpointer data);
static gboolean gestion_clavier(GtkWidget *window, GdkEventKey* event, gpointer data);
static gboolean gestion_souris_callback(GtkWidget *window, GdkEventButton* event, gpointer data);
static gboolean nouveau_cube( GtkWidget *menuItem, gpointer data );
static gboolean nouveau_rectangle( GtkWidget *menuItem, gpointer data );
static gboolean main_ouvrir( GtkWidget *menuItem, gpointer data );
static gboolean main_sauvegarder( GtkWidget *menuItem, gpointer data );
static gboolean main_annuler( GtkWidget *menuItem, gpointer data );
static gboolean main_refaire( GtkWidget *menuItem, gpointer data );
static gboolean main_quitter( GtkWidget *menuItem, gpointer data );
static gboolean main_nouveau( GtkWidget *menuItem, gpointer data );


 int main (int argc, char *argv[])
 {
    GtkWidget* mainWindow = NULL;                                               // Instantation de la fenetre principal
    GtkWidget* zoneDeDessin = NULL;
    Scene* scene = NULL;

    gtk_init( &argc, &argv );

    mainWindow = gtk_window_new( GTK_WINDOW_TOPLEVEL );                         // appel au constructeur de la fenetre
    zoneDeDessin = gtk_drawing_area_new ();

    scene = (Scene*)malloc( 1 * sizeof( Scene) );
    Scene_initialiser_scene( scene, zoneDeDessin );

    GtkWidget* main_box = gtk_vbox_new( FALSE, 0 );
    GtkWidget* menuBarre = gtk_menu_bar_new();
    gtk_widget_set_size_request( menuBarre, -1, -1 );

    GtkWidget* menu = gtk_menu_new();
    GtkWidget* fichier = gtk_menu_item_new_with_label( "Fichier" );
    GtkWidget* nouveau = gtk_menu_item_new_with_label( "Nouveau" );
    GtkWidget* ouvrir = gtk_menu_item_new_with_label( "Ouvrir" );
    GtkWidget* sauvegarder = gtk_menu_item_new_with_label( "Sauvegarder" );
    GtkWidget* quitter = gtk_menu_item_new_with_label( "Quitter" );
    gtk_menu_item_set_submenu(GTK_MENU_ITEM (fichier), menu);

    GtkWidget* menu2 = gtk_menu_new();
    GtkWidget* edition = gtk_menu_item_new_with_label( "Edition" );
    scene->modification->annuler = gtk_menu_item_new_with_label( "Annuler" );
    scene->modification->refaire = gtk_menu_item_new_with_label( "Refaire" );
    gtk_menu_item_set_submenu(GTK_MENU_ITEM (edition), menu2);

    gtk_menu_attach( GTK_MENU(menu2), scene->modification->annuler, 0, 1, 0, 1 );
    gtk_menu_attach( GTK_MENU(menu2), scene->modification->refaire, 0, 1, 1, 2 );

    gtk_menu_attach( GTK_MENU(menu), nouveau, 0, 1, 0, 1 );
    gtk_menu_attach( GTK_MENU(menu), ouvrir, 0, 1, 1, 2 );
    gtk_menu_attach( GTK_MENU(menu), sauvegarder, 0, 1, 2, 3 );
    gtk_menu_attach( GTK_MENU(menu), quitter, 0, 1, 3, 4 );
    gtk_menu_bar_append(GTK_MENU_BAR (menuBarre), fichier);
    gtk_menu_bar_append(GTK_MENU_BAR (menuBarre), edition);

    gtk_container_add( GTK_CONTAINER( main_box ), menuBarre );
    gtk_container_add( GTK_CONTAINER( main_box ), zoneDeDessin );

    GdkScreen* screen = NULL;
    screen = gtk_window_get_screen(GTK_WINDOW(mainWindow));
    double width = gdk_screen_get_width(screen);
    double height = gdk_screen_get_height(screen);

    /* Création de la caméra qui va bien TODO : étudier un moyen de mettre ça dans une fonction d'initialisation, init scene ?*/
    scene->camera = Camera_createCam(width-10,height-75);

    gtk_widget_set_size_request( zoneDeDessin, width-10, height-75 );        //taille minimum de la zone de dessin
    gtk_container_add( GTK_CONTAINER( mainWindow ), main_box );             // Ajout de la zone de dessin dans le mainWindow

    gtk_widget_show_all( mainWindow );

    gtk_window_set_title( GTK_WINDOW( mainWindow), "Sch3Dma" );          // Nom totalement provisiore ^^ (mais qui envoie quand même du bois !)

    gtk_widget_add_events( zoneDeDessin, GDK_BUTTON_PRESS_MASK );
    gtk_widget_add_events( zoneDeDessin, GDK_BUTTON_RELEASE_MASK );   //active la detection de la souris
    gtk_widget_add_events( zoneDeDessin, GDK_BUTTON1_MOTION_MASK );
    gtk_widget_add_events( zoneDeDessin, GDK_POINTER_MOTION_HINT_MASK );

    g_signal_connect( G_OBJECT( mainWindow ), "delete-event", G_CALLBACK( main_quitter ), NULL );
    g_signal_connect( G_OBJECT( mainWindow ), "key-press-event", G_CALLBACK(gestion_clavier), scene);
    g_signal_connect( G_OBJECT( zoneDeDessin ), "realize", G_CALLBACK( realize_callback ), NULL );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "expose-event", G_CALLBACK( expose_event_callback ), scene );
    g_signal_connect( G_OBJECT( mainWindow ), "key-release-event", G_CALLBACK( gestion_clavier ), scene );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "button-press-event", G_CALLBACK( gestion_souris_callback ), scene );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "button-release-event", G_CALLBACK( gestion_souris_callback ), scene );
    g_signal_connect( G_OBJECT( zoneDeDessin ), "motion-notify-event", G_CALLBACK( gestion_souris_callback ), scene );

    g_signal_connect( G_OBJECT( nouveau ), "activate", G_CALLBACK( main_nouveau ), scene);
    g_signal_connect( G_OBJECT( ouvrir ), "activate", G_CALLBACK( main_ouvrir ), scene);
    g_signal_connect( G_OBJECT( sauvegarder ), "activate", G_CALLBACK( main_sauvegarder ), scene);
    g_signal_connect( G_OBJECT( quitter ), "activate", G_CALLBACK( main_quitter ), NULL );
    g_signal_connect( G_OBJECT( scene->modification->annuler ), "activate", G_CALLBACK( main_annuler ), scene);
    g_signal_connect( G_OBJECT( scene->modification->refaire ), "activate", G_CALLBACK( main_refaire ), scene);

    gtk_widget_set_sensitive(scene->modification->annuler, FALSE);
    gtk_widget_set_sensitive(scene->modification->refaire, FALSE);

    gtk_window_maximize( GTK_WINDOW( mainWindow ) );

    gtk_main();

    Modification_detruire_temporaire( scene->modification );
    Scene_detruire( scene );
    Clavier_detruire( scene->clavier );
    free( scene->selection );
    free( scene->clavier );
    free( scene->creation );
    free( scene->modification );
    free( scene );

    return EXIT_SUCCESS;

 }

/** Fonction dessinant la scene, partie principale du programme, gérant aussi le dessin du rectangle de selection
 * @param widget, la fenetre contenant la zone de dessin à dessiner
 * @param event, l'evenement associer à l'appel du CallBack, ici inutiliser contenant "expose-event"
 * @param data, pointer générique contenant la scene qui elle contient toutes les informations necessaire au dessin
 * @return TRUE pour que l'expose event soit propagé dans l'application
 */
static gboolean expose_event_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    Scene* scene = (Scene*)data;                                     //Cast du pointeur generique en Scene*

    cairo_t* cr = gdk_cairo_create ( widget->window );               //Creation du contexte pour pouvoir dessiner

    Scene_clear_scene( scene , cr );                                 //Nettoyage de la scene

    Scene_dessiner_scene( scene, cr );                               //On dessine tout les objets

    Selection_dessiner_rectangle( scene->selection, cr );            //Si selection de zone en cours, on dessine le rectangle de selection

    cairo_destroy( cr );                                             //Destruction du contexte

    return TRUE;
}

static gboolean realize_callback( GtkWidget *widget, GdkEventExpose *event, gpointer data )
{
    printf("realize event \n");
    return TRUE;
}

/** Fonction gérant les entrées clavier, associant aux touches/combinaison de touches les fonctions associées
 * @param window, le widget ayant reçu l'evenement "key-press-event" ou "key-release-event"
 * @param event, le type d'evenement ici deux evenements nous interessent : "key-press-event" ou "key-release-event"
 * @param data, pointeur générique sur la scene, qui contient toutes les informations sur l'etat du clavier
 * @return TRUE pour propager les evenements dans l'application
 */
static gboolean gestion_clavier(GtkWidget *window, GdkEventKey* event, gpointer data)
{
    Scene* scene = (Scene*) data;                                                               //Cast du pointeur generique en Scene*

    if( event->type == GDK_KEY_PRESS )
    {
        Clavier_touche_appuyer( scene, gdk_keyval_name(event->keyval) );                        //On stocke toutes les touches qui ont ete appuyés et non relachés

        if( strcmp( gdk_keyval_name(event->keyval), "Right") == 0 )
        {
            // Fonction de rotation scene/objet
            gtk_widget_queue_draw( window );
        }
        else if( strcmp( gdk_keyval_name(event->keyval), "a") == 0 )
        {
            if( Clavier_est_appuyer( scene, "Control_L" ) )
            {
                Selection_selectionner_tout( scene );                                           //Raccourci Ctrl_L + a = selectionner tout
            }
            gtk_widget_queue_draw( window );
        }
        else if( strcmp( gdk_keyval_name(event->keyval), "Escape") == 0 )
        {
            Selection_deselectionner_tout( scene->selection );                                  //Echap = Tout deselectionner
            gtk_widget_queue_draw( window );
        }
        else if( strcmp( gdk_keyval_name( event->keyval), "z" ) == 0 )
        {
            if( Clavier_est_appuyer( scene, "Control_L" ) && gtk_widget_get_sensitive( scene->modification->annuler ) )
            {
                Modification_annuler( scene );                                                  //Ctrl_L + z = annuler si annuler disponible
            }
            gtk_widget_queue_draw( window );
        }
    }
    else if( event->type == GDK_KEY_RELEASE )
    {
        Clavier_touche_relacher( scene, gdk_keyval_name(event->keyval) );                       //On eneleve les touches relachées
    }
    return TRUE;
}

/** Fonction gérant les clics/deplacements souris, associant aux clics les differentes fonctions
 * @param window, le widget ayant reçu l'evenement
 * @param event, le type d'evenement ici plusieurs evenements nous interessent : "button-press-event", "button-release-event", "motion-notify-event"
 * @param data, pointeur générique sur la scene, qui contient toutes les informations sur l'etat de la souris
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean gestion_souris_callback(GtkWidget *widget, GdkEventButton* event, gpointer data)
{
    Scene* scene = (Scene*)data;

    if( event->type == GDK_BUTTON_PRESS && event->button == 1 )
    {
        scene->selection->departSelection.x = event->x;                                       //Click gauche, on stocke le point clické au cas ou l'evenement soit suivi d'un motion-notify-event
        scene->selection->departSelection.y = event->y;
        Selection_selectionner_objet( scene, event->x, event->y );                            //On regarde si le click ne dois pas selectionner un objet

        gtk_widget_queue_draw( widget );
    }
    else if( event->type == GDK_BUTTON_PRESS && event->button == 3 )                          //Click droit on affiche le menu contextuel
    {
        GtkWidget *menu = gtk_menu_new();
        GtkWidget *pItem = gtk_menu_item_new_with_label("Nouvel objet");
        GtkWidget *pItem2 = gtk_menu_item_new_with_label("Propriété");

        GtkWidget *sousMenu1 = gtk_menu_new();
        GtkWidget *pItem3 = gtk_menu_item_new_with_label("Cube");
        GtkWidget *pItem4 = gtk_menu_item_new_with_label("Rectangle");
        gtk_menu_attach( GTK_MENU(sousMenu1), pItem3, 0, 1, 0, 1 );
        gtk_menu_attach( GTK_MENU(sousMenu1), pItem4, 0, 1, 1, 2 );

        gtk_menu_item_set_submenu( GTK_MENU_ITEM(pItem), sousMenu1 );

        gtk_menu_attach( GTK_MENU(menu), pItem, 0, 1, 0, 1 );
        gtk_menu_attach( GTK_MENU(menu), pItem2, 0, 1, 1, 2 );

        gtk_widget_show_all(menu);

        Scene_creation_objet( scene, event->x, event->y );

        gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event->button, event->time);
        g_signal_connect( G_OBJECT( pItem3 ), "activate", G_CALLBACK(nouveau_cube), scene);
        g_signal_connect( G_OBJECT( pItem4 ), "activate", G_CALLBACK(nouveau_rectangle), scene);
    }
    else if( event->type == GDK_MOTION_NOTIFY )                                                 //Prob lag normalement resolu grace à GDK_POINTER_MOTION_HINT_MASK
    {
        scene->selection->finSelection.x = event->x;                                            //Motion-notify-event on stocke le point d'arrive pour dessiner le rectangle
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

/** Fonction créant la fenetre d'ajout d'un nouvel objet dans la scene
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, pointeur générique sur la scene, qui contient la represantation de la scene a laquelle il faudra ajouter le nouvel objet
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean nouveau_cube( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;
    FenetreAjoutCube* fao = (FenetreAjoutCube*)malloc( 1 *sizeof( FenetreAjoutCube) );
    initialiser_FenetreAjoutCube( fao, scene );

    return TRUE;
}

static gboolean nouveau_rectangle( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;
    FenetreAjoutRectangle* fao = (FenetreAjoutRectangle*)malloc( 1 *sizeof( FenetreAjoutRectangle ) );
    initialiser_FenetreAjoutRectangle( fao, scene );

    return TRUE;
}

/** Fonction gérant l'ouverture d'un fichier de sauvegarde
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, pointeur générique sur la scene, qui sera reecrite selon le ficier de sauvegarde lu
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean main_ouvrir( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;
    Scene_reset( scene, scene->zoneDeDessin );

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
                    Modification_modification_effectuer( scene );

                    fscanf( fichier, "%s", typeObjet );
                }
            }
             else
            {
                GtkWidget* avertissement =
                gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Le fichier n'est pas un fichier de sauvegarde Sch3Dma" );

                if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
                {
                    gtk_widget_destroy( avertissement );

                }
            }
            fclose( fichier );
        }
        else
        {
            GtkWidget* avertissement =
            gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Le fichier n'a pas pu etre ouvert" );

            if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
            {
                gtk_widget_destroy( avertissement );

            }
        }
    }
    gtk_widget_destroy (opener);
    gtk_widget_queue_draw( scene->zoneDeDessin );
    return TRUE;
}

/** Fonction gérant la sauvegarde d'une scene
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, pointeur générique sur la scene, qui sera lu et ecrite dans un fichier
 * @return TRUE pour propager les evenements dans l'application
 **/
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
            filename = (char*)realloc( filename, ( strlen(filename) + 4 ) * sizeof( char ) );
            filename = strcat( filename, ".txt" );

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
        GtkWidget* avertissement =
        gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "La scene est vide sauvegarde inutile !" );

        if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
        {
            gtk_widget_destroy( avertissement );

        }
    }
    return TRUE;
}


/** Fonction gérant le click sur l'element annuler
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, pointeur générique sur la scene, qui contient un pointeur sur le module Modification
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean main_annuler( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;

    Modification_annuler( scene );

    return TRUE;
}

/** Fonction gérant le click sur l'element refaire
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, pointeur générique sur la scene, qui contient un pointeur sur le module Modification
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean main_refaire( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;

    Modification_refaire( scene );

    return TRUE;
}

/** Fonction gérant la destruction de la fenetre principale
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, NULL
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean main_quitter( GtkWidget *menuItem, gpointer data )
{
    GtkWidget* avertissement =
    gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, "Etes-vous sur de vouloir quitter ? Tout travail non sauvegardé sera perdu !" );

    if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
    {
        gtk_widget_destroy( avertissement );
        gtk_main_quit();
    }
    else
    {
        gtk_widget_destroy( avertissement );
    }
    return TRUE;
}

/** Fonction gérant la demande d'une nouvelle scene
 * @warning l'ancienne scene sera detruite, à l'exception de la zone de dessin qui sera conservé
 * @param menuItem, l'element du menu ayant ete cliqué
 * @param data, pointeur générique sur la scene, qui sera detruite et reconstruite vierge
 * @return TRUE pour propager les evenements dans l'application
 **/
static gboolean main_nouveau( GtkWidget *menuItem, gpointer data )
{
    Scene* scene = (Scene*)data;
    GtkWidget* avertissement =
    gtk_message_dialog_new( NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, "Tout travail non sauvegardé sera perdu ! Etes-vous sur de vouloir continuer ?" );

    if( gtk_dialog_run ( GTK_DIALOG ( avertissement ) ) == GTK_RESPONSE_OK )
    {
        gtk_widget_destroy( avertissement );
        Scene_reset( scene, scene->zoneDeDessin );
        gtk_widget_queue_draw( scene->zoneDeDessin );
    }
    else
    {
        gtk_widget_destroy( avertissement );
    }
    return TRUE;
}
