#include "Groupe.h"

void Groupe_initialiser( Groupe* groupe, Groupe* pere, int nb )
{
    groupe->pere = pere;
    groupe->id = nb;
    groupe->tObjet = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
    groupe->nbObjet = 0;
    groupe->nbFils = 0;
    groupe->tFils = g_array_new( FALSE, FALSE, sizeof( Groupe* ) );
    groupe->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter) );
}

void Groupe_detruire( Groupe* groupe )
{
    g_array_free( groupe->tObjet, FALSE );
    g_array_free( groupe->tFils, FALSE );
}

void Groupe_ajouter_Objet( Groupe* groupe, Objet* objet )
{
    g_array_append_val( groupe->tObjet, objet );
    groupe->nbObjet++;
}

void Groupe_ajouter_Fils( Groupe* groupe, Groupe* fils )
{
    g_array_append_val( groupe->tFils, fils );
    groupe->nbFils++;
}

void Groupe_enlever_objet( Groupe* groupe, Objet* objet )
{
    int i = 0;

    for( i = 0; i < groupe->nbObjet; i++ )
    {
        if( objet == g_array_index( groupe->tObjet, Objet*, i ) )
        {
            g_array_remove_index_fast( groupe->tObjet, i );
            groupe->nbObjet--;
        }
    }
}

void Groupe_enlever_fils( Groupe* groupe, Groupe* fils )
{
    int i = 0;

    for( i = 0; i < groupe->nbFils; i++ )
    {
        if( fils == g_array_index( groupe->tFils, Groupe*, i ) )
        {
            //Groupe_detruire( fils );
            g_array_remove_index_fast( groupe->tFils, i );
            groupe->nbFils--;
        }
    }
}



