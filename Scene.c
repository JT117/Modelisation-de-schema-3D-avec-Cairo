#include "Scene.h"

void initialiser_Scene( Scene* scene, GtkWidget* window )
{
    scene->tCube = g_array_new( FALSE, FALSE, sizeof( Cube* ) );
    scene->nbCube = 0;
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
        if( g_array_index( scene->tCube, Cube*, i )->estSelectionne )
        {
            printf(" Un Cube est selectionné \n");
        }
        dessiner_Cube( g_array_index( scene->tCube, Cube*, i ) , cr );
    }

}

void selectionner_objet( Scene* scene, double x, double y )
{
    int i = 0;

    for( i = 0; i < scene->nbCube; i++ )
    {
        Cube* cube = g_array_index( scene->tCube, Cube*, i );

        contientPoint( cube, x, y );
    }
}
