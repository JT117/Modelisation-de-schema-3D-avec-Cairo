#include "Scene.h"

/** Fonction qui initialise une scene vierge
 * @param scene, un pointeur sur une scene vierge
 * @param window, la zone de dessin à garder
 **/
void Scene_initialiser_scene( Scene* scene, GtkWidget* window )
{
    scene->tObjet = g_array_new( FALSE, TRUE, sizeof( Objet* ) );
    scene->nbObjet = 0;

    scene->zoneDeDessin = window;

    scene->selection = (Selection*)malloc( 1 * sizeof(Selection) );
    Selection_initialiser( scene->selection );

    Clavier* clavier = (Clavier*)malloc( 1 * sizeof( Clavier ) );
    Clavier_initialiser( clavier );
    scene->clavier = clavier;

    scene->modification = (Modification*)malloc( 1 * sizeof( Modification ) );
    Modification_initialiser( scene->modification );

    scene->tailleCreation = 50.0;
    scene->creation = (Point*)malloc( 1* sizeof( Point ) );
    scene->creation->x = 0.0;
    scene->creation->y = 0.0;
    scene->creation->z = 0.0;
}

void Scene_reconstruire( Scene* scene, GtkWidget* window )
{
    scene->tObjet = g_array_new( FALSE, TRUE, sizeof( Objet* ) );
    scene->nbObjet = 0;

    scene->zoneDeDessin = window;

    scene->selection = (Selection*)malloc( 1 * sizeof(Selection) );
    Selection_initialiser( scene->selection );

}

/** Fonction qui libère une scene initialisée
 * @param scene, un pointeur sur une scene initialisée
 * @warning scene et les pointeurs sur les modules ne sont pas liberée
 **/
void Scene_detruire( Scene* scene )
{
    int i = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet_detruire( g_array_index( scene->tObjet, Objet*, i ) );
    }
    g_array_free( scene->tObjet, TRUE );

    Selection_detruire( scene->selection );
    //Clavier_detruire( scene->clavier );
}

/** Fonction qui ajoute un objet de type Cube à la scene
 * @param scene, un pointeur sur une scene initialisée
 * @param cCube, le cube à ajouter
 **/
void Scene_ajouter_cube( Scene* scene, Cube* cCube )
{
    Objet* objet = (Objet*)malloc( 1 * sizeof( Objet ) );
    Objet_est_un_Cube( objet, cCube );
    g_array_append_val( scene->tObjet, objet );
    scene->nbObjet++;
}

/** Fonction qui ajoute un objet de type Rectangle à la scene
 * @param pScene Pointeur sur une scene initialisée
 * @param pRect Pointeur sur le rectangle à ajouter
 **/
void Scene_ajouter_rectangle( Scene* scene, Rectangle* pRect )
{
	/* Création d'un nouvel objet*/
    Objet* objet = (Objet*)malloc( 1 * sizeof( Objet ) );
    /* L'objet est un rectangle, on initialise les infos nécessaire de notre nouvelle structure objet*/
    Objet_est_un_Rectangle( objet, pRect );
    g_array_append_val( scene->tObjet, objet );
    scene->nbObjet++;
}

/** Fonction qui dessiner tout les objets de la scene
 * @param scene, un pointeur sur une scene initialisée
 * @param cr, un contexte cairo créer sur la zoneDeDessin
 **/
void Scene_dessiner_scene( Scene* scene, cairo_t* cr )
{
    int i = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet_dessiner_objet( g_array_index( scene->tObjet, Objet*, i ) , cr, scene->camera );
    }
}

/** Fonction qui dessine une scene vierge
 * @param scene, un pointeur sur une scene initialisée
 * @param cr, un contexte cairo créer sur la zoneDeDessin
 **/
void Scene_clear_scene( Scene* scene, cairo_t* cr )
{
     cairo_set_source_rgb( cr, 0.190, 0.190, 0.190 );
     cairo_paint( cr );
}

/** Fonction qui indique si une selection multiple est en cours
 * @param scene, un pointeur sur une scene initialisée
 * @return TRUE si une selection multiple est en cours
 **/
gboolean Scene_selection_Multiple( Scene* scene )
{
    int i = 0;

    for( i = 0; i < scene->clavier->nbTouche; i++ )
    {
        printf(" %s \n", g_array_index( scene->clavier->tTouche, char*, i ) );
        if( strcmp( g_array_index( scene->clavier->tTouche, char*, i ), "Shift_L") == 0 )
        {
            return TRUE;
        }
    }

    return FALSE;
}

void Scene_creation_objet( Scene* scene, double x, double y )
{
    scene->creation->x = x;
    scene->creation->y = y;
    scene->creation->z = 0;
}

/** Fonction qui remets la scene en etat d'initialisation
 * @param scene, un pointeur sur une scene initialisée
 * @param window, une pointeur sur la zoneDeDessin
 * @warning La zone de dessin est conservé
 **/
void Scene_reset( Scene* scene, GtkWidget* window )
{
    Scene_detruire( scene );
    free( scene->selection );
    Scene_reconstruire( scene, window );
}

