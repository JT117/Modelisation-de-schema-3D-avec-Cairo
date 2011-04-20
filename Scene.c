#include "Scene.h"

void Scene_initialiser_scene( Scene* scene, GtkWidget* window )
{
    scene->tObjet = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
    scene->nbObjet = 0;
    scene->tSelection = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
    scene->nbSelection = 0;
    scene->tTouche= g_array_new( FALSE, FALSE, sizeof( char* ) );
    scene->nbTouche = 0;
    scene->selection_en_cours = FALSE;
    scene->tailleCreation = 50;
}

void Scene_ajouter_cube( Scene* scene, Cube* cCube )
{
    Objet* objet = malloc( 1 * sizeof( Objet ) );
    Objet_Cube( objet, cCube );
    g_array_append_val( scene->tObjet, objet );
    scene->nbObjet++;
}

void Scene_dessiner_scene( Scene* scene, cairo_t* cr )
{
    int i = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        dessiner_Objet( g_array_index( scene->tObjet, Objet*, i ) , cr );
    }

}

void Scene_clear_scene( Scene* scene, cairo_t* cr )
{
     cairo_set_source_rgb( cr, 0.190, 0.190, 0.190 );
     cairo_paint( cr );
}

void Scene_selectionner_objet( Scene* scene, double x, double y )
{
    int i = 0;
    int nbNonSelectionner = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet* objet = (Objet*)g_array_index( scene->tObjet, Objet*, i );
        gboolean estContenu = Objet_contient_Point( objet, x, y );

        if( estContenu  )
        {
            int j = 0;
            gboolean estDejaSelectionner = FALSE;

            for( j = 0; j < scene->nbSelection; j++ )
            {
                if( objet == g_array_index( scene->tSelection, Objet*, j ) )
                {
                    estDejaSelectionner = TRUE;
                }
            }

            if( !estDejaSelectionner && Scene_selection_Multiple( scene ) )
            {
                g_array_append_val( scene->tSelection, objet );
                scene->nbSelection++;
                Objet_Selection( objet );
            }
            else if( !estDejaSelectionner )
            {
                Scene_deselectionner_tout( scene );
                g_array_free( scene->tSelection, FALSE );
                scene->tSelection = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
                g_array_append_val( scene->tSelection, objet );
                Objet_Selection( objet );
                scene->nbSelection = 1;
            }
            else
            {
                int n = 0;

                for( n = 0; n < scene->nbSelection; n++ )
                {
                    if( objet == g_array_index( scene->tSelection, Objet*, n ) )
                    {
                        g_array_remove_index_fast( scene->tSelection, n );
                        scene->nbSelection--;
                        Objet_Deselection( objet );
                    }
                }
            }
        }
        else if( !estContenu )
        {
            nbNonSelectionner++;
        }
    }

    if( nbNonSelectionner == scene->nbObjet )
    {
        Scene_deselectionner_tout( scene );
        g_array_free( scene->tSelection, FALSE );
        scene->tSelection = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
        scene->nbSelection = 0;
    }
}

void Scene_selectionner_click_drag( Scene* scene )
{
    int i = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet* objet = (Objet*)g_array_index( scene->tObjet, Objet*, i );
        int k = 0;
        int j = 0;
        int x1 = 0;
        int x2 = 0;
        int y1 = 0;
        int y2 = 0;
        gboolean estContenu = FALSE;

        if( (int)scene->departSelection.x > (int)scene->finSelection.x )
        {
            x1 = (int)scene->finSelection.x;
            x2 = (int)scene->departSelection.x;
        }
        else
        {
            x2 = (int)scene->finSelection.x;
            x1 = (int)scene->departSelection.x;
        }

        if( scene->departSelection.y > scene->finSelection.y )
        {
            y1 = (int)scene->finSelection.y;
            y2 = (int)scene->departSelection.y;
        }
        else
        {
            y2 = (int)scene->finSelection.y;
            y1 = (int)scene->departSelection.y;
        }

        for( k = x1; k < x2; k += 15 )
        {
            for( j = y1; j < y2; j += 15 )
            {
                 estContenu = estContenu || Objet_contient_Point( objet, k, j );
            }
        }

        if( estContenu )
        {
            int m = 0;
            gboolean estDejaSelectionner = FALSE;

            for( m = 0; m < scene->nbSelection; m++ )
            {
                if( objet == g_array_index( scene->tSelection, Objet*, m ) )
                {
                    estDejaSelectionner = TRUE;
                }
            }

            if( !estDejaSelectionner )
            {
                g_array_append_val( scene->tSelection, objet );
                scene->nbSelection++;
                Objet_Selection( objet );
            }
        }
        else
        {
            int n = 0;

            for( n = 0; n < scene->nbSelection; n++ )
            {
                if( objet == g_array_index( scene->tSelection, Objet*, n ) )
                {
                    g_array_remove_index_fast( scene->tSelection, n );
                    scene->nbSelection--;
                    Objet_Deselection( objet );
                }
            }
        }
    }
}

void Scene_deselectionner_tout( Scene* scene )
{
    int i = 0;

    for( i = 0; i < scene->nbSelection; i++ )
    {
        Objet* objet = (Objet*)g_array_index( scene->tSelection, Objet*, i );
        Objet_Deselection( objet );
    }
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

void Scene_selectionner_tout( Scene* scene )
{
    int i = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet* objet = g_array_index( scene->tObjet, Objet*, i );
        g_array_append_val( scene->tSelection, objet );
        scene->nbSelection++;
        Objet_Selection( objet );
    }

}

void creation_objet( Scene* scene, double x, double y )
{
    scene->creation.x = x;
    scene->creation.y = y;
    scene->creation.z = 0;
}


