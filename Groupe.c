#include "Groupe.h"
#include "Scene.h"

void Groupe_initialiser( Groupe* groupe, Groupe* pere, int nb )
{
    groupe->pere = pere;
    groupe->id = nb;
    groupe->tObjet = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
    groupe->nbObjet = 0;
    groupe->nbFils = 0;
    groupe->tFils = g_array_new( FALSE, FALSE, sizeof( Groupe* ) );
    groupe->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter) );

    if( groupe->id == 0)
    {
        GtkWidget* label = gtk_label_new("Groupe 0");
        groupe->nom = gtk_label_get_text( GTK_LABEL( label ) );
    }
}

void Groupe_detruire( Groupe* groupe )
{
    g_array_free( groupe->tObjet, FALSE );
    g_array_free( groupe->tFils, FALSE );
    free( groupe->iter );
}

void Groupe_ajouter_Objet( Groupe* groupe, Objet* objet )
{
    g_array_append_val( groupe->tObjet, objet );
    objet->numeroGroupe = groupe->id;
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
            g_array_remove_index_fast( groupe->tFils, i );
            groupe->nbFils--;
        }
    }
}

Groupe* Groupe_trouver( Scene* scene, char* nom )
{
    int i = 0;

    for( i = 0; i < scene->nbGroupe; i++ )
    {
        Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, i );

        if( strcmp( groupe->nom, nom ) == 0 )
        {
            return groupe;
        }
    }
    return NULL;
}

Groupe* Groupe_trouver_ById( Scene* scene, int id )
{
    int i = 0;

    for( i = 0; i < scene->nbGroupe; i++ )
    {
        Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, i );

        if( groupe->id == id )
        {
            return groupe;
        }
    }
    return NULL;
}
