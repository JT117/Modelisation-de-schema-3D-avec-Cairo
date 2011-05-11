#include "Groupe.h"

Groupe_initialiser( Groupe* groupe, Groupe* pere, int nb )
{
    groupe->pere = pere;
    groupe->id = nb;
    groupe->tObjet = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
    groupe->tFils = g_array_new( FALSE, FALSE, sizeof( Groupe* ) );
}

Groupe_detruire( Groupe* groupe )
{
    g_array_free( groupe->tObjet, FALSE );
    g_array_free( groupe->tFils, FALSE );
}

Groupe_ajouter_Objet( Groupe* groupe, Objet* objet )
{
    g_array_append_val( groupe->tObjet, objet );
    groupe->nbObjet++;
}

Groupe_ajouter_Fils( Groupe* groupe, Groupe* fils )
{
    g_array_append_val( groupe->tFils, fils );
    groupe->nbFils++;
}

Groupe_enlever_objet( Groupe* groupe, Objet* objet )
{
    int i = 0;

    for( i = 0; i < groupe->nbObjet; i++ )
    {
        if( objet == g_array_index( groupe->tObjet, Objet*, i ) )
        {
            g_array_remove_index_fast( groupe->tObjet, i );
        }
    }
}

Groupe_enlever_fils( Groupe* groupe, Groupe* fils )
{
    int i = 0;

    for( i = 0; i < groupe->nbFils; i++ )
    {
        if( fils == g_array_index( groupe->tFils, Groupe*, i ) )
        {
            Groupe_detruire( fils );
            g_array_remove_index_fast( groupe->tFils, i );
        }
    }
}


