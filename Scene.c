#include "Scene.h"

void initialiser_Scene( Scene* scene, GtkWidget* window )
{
    scene->tObjet = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
    scene->nbObjet = 0;
    scene->tSelection = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
    scene->nbSelection = 0;
    scene->tTouche= g_array_new( FALSE, FALSE, sizeof( char* ) );
    scene->nbTouche = 0;
}

void ajouter_cube( Scene* scene, Cube* cCube )
{
    Objet* objet = malloc( 1 * sizeof( Objet ) );
    Objet_Cube( objet, cCube );
    g_array_append_val( scene->tObjet, objet );
    scene->nbObjet++;
}

void dessiner_Scene( Scene* scene, cairo_t* cr )
{
    int i = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        dessiner_Objet( g_array_index( scene->tObjet, Objet*, i ) , cr );
    }

}

void selectionner_objet( Scene* scene, double x, double y )
{
    int i = 0;
    int nbNonSelectionner = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet* objet = (Objet*)g_array_index( scene->tObjet, Objet*, i );
        gboolean estContenu = Objet_contient_Point( objet, x, y );

        printf(" -------------------------------- \n");

        if( estContenu &&  selection_Multiple( scene ) )
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

            if( !estDejaSelectionner )
            {
                printf("SELECTION MULTIPLE ACTIVER \n");
                g_array_append_val( scene->tSelection, objet );
                scene->nbSelection++;
                Objet_Selection( objet );
                printf("SELECTION MULTIPLE EFFECTUER \n");
            }
        }
        else if( estContenu )
        {
            printf("SELECTION UNIQUE ACTIVER \n");
            deselectionner_tout( scene );
            g_array_free( scene->tSelection, FALSE );
            scene->tSelection = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
            g_array_append_val( scene->tSelection, objet );
            Objet_Selection( objet );
            scene->nbSelection = 1;
            printf("SELECTION UNIQUE EFFECTUER \n");
        }
        else if( !estContenu )
        {
            nbNonSelectionner++;
        }
    }

    if( nbNonSelectionner == scene->nbObjet )
    {
        printf("Deselection \n");
        deselectionner_tout( scene );
        g_array_free( scene->tSelection, FALSE );
        scene->tSelection = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
        scene->nbSelection = 0;
        printf("deselection EFFECTUER \n");
    }
      printf("nombre de cube -> %d  |  nombre de selection %d \n", scene->nbObjet, scene->nbSelection );
}

void deselectionner_tout( Scene* scene )
{
    int i = 0;

    for( i = 0; i < scene->nbSelection; i++ )
    {
        Objet* objet = (Objet*)g_array_index( scene->tSelection, Objet*, i );
        Objet_Deselection( objet );
    }
}

gboolean selection_Multiple( Scene* scene )
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

void touche_appuyer( Scene* scene, char* nomTouche )
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
        g_array_append_val( scene->tTouche, nomTouche );
        scene->nbTouche++;
    }
}

void touche_relacher( Scene* scene, char* nomTouche )
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
