#include "Scene.h"

void initialiser_Scene( Scene* scene, GtkWidget* window )
{
    scene->tCube = g_array_new( FALSE, FALSE, sizeof( Cube* ) );
    scene->nbCube = 0;
    scene->tCubeSelection = g_array_new( FALSE, FALSE, sizeof( Cube* ) );
    scene->nbCubeSelection = 0;
    scene->tTouche= g_array_new( FALSE, FALSE, sizeof( char* ) );
    scene->nbTouche = 0;
}

void ajouter_objet( Scene* scene, Cube* cCube )
{
    g_array_append_val( scene->tCube, cCube );
    scene->nbCube++;
}

void dessiner_Scene( Scene* scene, cairo_t* cr )
{
    int i = 0;

    for( i = 0; i < scene->nbCube; i++ )
    {
        dessiner_Cube( g_array_index( scene->tCube, Cube*, i ) , cr );
    }

}

void selectionner_objet( Scene* scene, double x, double y )
{
    int i = 0;
    int nbNonSelectionner = 0;

    for( i = 0; i < scene->nbCube; i++ )
    {
        Cube* cube = (Cube*)g_array_index( scene->tCube, Cube*, i );
        gboolean estContenu = contientPoint( cube, x, y );

        printf(" -------------------------------- \n");

        if( estContenu &&  selection_Multiple( scene ) )
        {
            int j = 0;
            gboolean estDejaSelectionner = FALSE;

            for( j = 0; j < scene->nbCubeSelection; j++ )
            {
                if( cube == g_array_index( scene->tCubeSelection, Cube*, j ) )
                {
                    estDejaSelectionner = TRUE;
                }
            }

            if( !estDejaSelectionner )
            {
                printf("SELECTION MULTIPLE ACTIVER \n");
                g_array_append_val( scene->tCubeSelection, cube );
                scene->nbCubeSelection++;
                cube->estSelectionne = TRUE;
                printf("SELECTION MULTIPLE EFFECTUER \n");
            }
        }
        else if( estContenu )
        {
            printf("SELECTION UNIQUE ACTIVER \n");
            deselectionner_tout( scene );
            g_array_free( scene->tCubeSelection, FALSE );
            scene->tCubeSelection = g_array_new( FALSE, FALSE, sizeof( Cube* ) );
            g_array_append_val( scene->tCubeSelection, cube );
            cube->estSelectionne = TRUE;
            scene->nbCubeSelection = 1;
            printf("SELECTION UNIQUE EFFECTUER \n");
        }
        else if( !estContenu )
        {
            nbNonSelectionner++;
        }
    }

    if( nbNonSelectionner == scene->nbCube )
    {
        printf("Deselection \n");
        deselectionner_tout( scene );
        g_array_free( scene->tCubeSelection, FALSE );
        scene->tCubeSelection = g_array_new( FALSE, FALSE, sizeof( Cube* ) );
        scene->nbCubeSelection = 0;
        printf("deselection EFFECTUER \n");
    }
      printf("nombre de cube -> %d  |  nombre de selection %d \n", scene->nbCube, scene->nbCubeSelection );
}

void deselectionner_tout( Scene* scene )
{
    int i = 0;

    for( i = 0; i < scene->nbCubeSelection; i++ )
    {
        Cube* cube = (Cube*)g_array_index( scene->tCubeSelection, Cube*, i );
        cube->estSelectionne = FALSE;
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
