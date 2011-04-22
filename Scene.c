#include "Scene.h"

void Scene_initialiser_scene( Scene* scene, GtkWidget* window )
{
    scene->tObjet = g_array_new( FALSE, TRUE, sizeof( Objet* ) );
    scene->nbObjet = 0;

    scene->zoneDeDessin = window;

    scene->selection = (Selection*)malloc( 1 * sizeof(Selection) );
    Selection_initialiser( scene->selection );

    scene->tTouche= g_array_new( FALSE, TRUE, sizeof( char* ) );
    scene->nbTouche = 0;

    scene->tailleCreation = 50.0;
    scene->creation = (Point*)malloc( 1* sizeof( Point ) );
    scene->creation->x = 0.0;
    scene->creation->y = 0.0;
    scene->creation->z = 0.0;
}

void Scene_detruire( Scene* scene )
{
    int i = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet_detruire( g_array_index( scene->tObjet, Objet*, i ) );
    }
    g_array_free( scene->tObjet, TRUE );
    g_array_free( scene->tTouche, TRUE );
    Selection_detruire( scene->selection );
}

void Scene_ajouter_cube( Scene* scene, Cube* cCube )
{
    Objet* objet = (Objet*)malloc( 1 * sizeof( Objet ) );
    Objet_est_un_Cube( objet, cCube );
    g_array_append_val( scene->tObjet, objet );
    scene->nbObjet++;
}

void Scene_dessiner_scene( Scene* scene, cairo_t* cr )
{
    int i = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet_dessiner_objet( g_array_index( scene->tObjet, Objet*, i ) , cr );
    }
}

void Scene_clear_scene( Scene* scene, cairo_t* cr )
{
     cairo_set_source_rgb( cr, 0.190, 0.190, 0.190 );
     cairo_paint( cr );
}

gboolean Scene_selection_Multiple( Scene* scene )
{
    int i = 0;

    for( i = 0; i < scene->nbTouche; i++ )
    {
        printf(" %s \n", g_array_index( scene->tTouche, char*, i ) );
        if( strcmp( g_array_index( scene->tTouche, char*, i ), "Shift_L") == 0 )
        {
            return TRUE;
        }
    }

    return FALSE;
}

void Scene_touche_appuyer( Scene* scene, char* nomTouche )
{
    int i = 0;
    gboolean dejaAppuyer = FALSE;

    for( i = 0; i < scene->nbTouche; i++ )
    {
        if( strcmp( g_array_index( scene->tTouche, char*, i ), nomTouche) == 0 )
        {
            dejaAppuyer = TRUE;
        }
    }

    if( dejaAppuyer == FALSE )
    {
        printf(" %s \n", nomTouche );
        g_array_append_val( scene->tTouche, nomTouche );
        scene->nbTouche++;
    }
}

void Scene_touche_relacher( Scene* scene, char* nomTouche )
{
    int i = 0;

    for( i = 0; i < scene->nbTouche; i++ )
    {
        if( strcmp( g_array_index( scene->tTouche, char*, i ), nomTouche) == 0 )
        {
            g_array_remove_index_fast( scene->tTouche, i );
            scene->nbTouche--;
        }
    }
}


void Scene_creation_objet( Scene* scene, double x, double y )
{
    scene->creation->x = x;
    scene->creation->y = y;
    scene->creation->z = 0;
}


