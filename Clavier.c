#include "Clavier.h"
#include "Scene.h"

void Clavier_initialiser( Clavier* clavier )
{
    clavier->tTouche= g_array_new( FALSE, TRUE, sizeof( char* ) );
    clavier->nbTouche = 0;
}

void Clavier_detruire( Clavier* clavier )
{
    int i = 0;

    for( i = 0; i < clavier->nbTouche; i++ )
    {
        g_free( g_array_index( clavier->tTouche, char*, i ) );
    }
    g_array_free( clavier->tTouche, FALSE );
}

gboolean Clavier_est_appuyer( Scene* scene, char* touche )
{
    int i = 0;

    for( i = 0; i < scene->clavier->nbTouche; i++ )
    {
        if( strcmp( g_array_index( scene->clavier->tTouche, char*, i ), touche ) == 0 )
        {
            return TRUE;
        }
    }
    return FALSE;
}

void Clavier_touche_appuyer( Scene* scene, char* nomTouche )
{
    if( !Clavier_est_appuyer( scene, nomTouche ) )
    {
        //printf(" %s \n", nomTouche );
        g_array_append_val( scene->clavier->tTouche, nomTouche );
        scene->clavier->nbTouche++;
    }
}

void Clavier_touche_relacher( Scene* scene, char* nomTouche )
{
    int i = 0;

    for( i = 0; i < scene->clavier->nbTouche; i++ )
    {
        if( strcmp( g_array_index( scene->clavier->tTouche, char*, i ), nomTouche) == 0 )
        {
            g_array_remove_index_fast( scene->clavier->tTouche, i );
            scene->clavier->nbTouche--;
        }
    }
}
