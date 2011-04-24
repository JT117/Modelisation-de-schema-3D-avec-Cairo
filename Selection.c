#include "Selection.h"
#include "Config.h"

void Selection_initialiser( Selection* selection )
{
    selection->tSelection = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
    selection->nbSelection = 0;
    selection->departSelection.x = 0;
    selection->departSelection.y = 0;
    selection->departSelection.z = 0;
    selection->finSelection.x = 0;
    selection->finSelection.y = 0;
    selection->finSelection.z = 0;
    selection->selection_en_cours = FALSE;
}

void Selection_detruire( Selection* selection )
{
    int i = 0;

    for( i = 0; i < selection->nbSelection; i++ )
    {
        Objet_detruire( g_array_index( selection->tSelection, Objet*, i ) );
    }
    g_array_free( selection->tSelection, FALSE );
}

void Selection_selectionner_objet( Scene* scene, double x, double y )   // A optimiser + Si un objet a une alloc dyn vider les tableaux avant de les free
{
    int i = 0;
    int nbNonSelectionner = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet* objet = (Objet*)g_array_index( scene->tObjet, Objet*, i );
        gboolean estContenu = Objet_contient_point( objet, x, y );

        if( estContenu  )
        {
            int j = 0;
            gboolean estDejaSelectionner = FALSE;

            for( j = 0; j < scene->selection->nbSelection; j++ )
            {
                if( objet == g_array_index( scene->selection->tSelection, Objet*, j ) )
                {
                    estDejaSelectionner = TRUE;
                }
            }

            if( !estDejaSelectionner && Scene_selection_Multiple( scene ) )
            {
                g_array_append_val( scene->selection->tSelection, objet );
                scene->selection->nbSelection++;
                Objet_selection( objet );
            }
            else if( !estDejaSelectionner )
            {
                Selection_deselectionner_tout( scene->selection );
                g_array_free( scene->selection->tSelection, FALSE );
                scene->selection->tSelection = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
                g_array_append_val( scene->selection->tSelection, objet );
                Objet_selection( objet );
                scene->selection->nbSelection = 1;
            }
            else
            {
              Selection_deselectionner( scene->selection, objet );
            }
        }
        else if( !estContenu )
        {
            nbNonSelectionner++;
        }
    }

    if( nbNonSelectionner == scene->nbObjet )
    {
        Selection_deselectionner_tout( scene->selection );
        g_array_free( scene->selection->tSelection, FALSE );
        scene->selection->tSelection = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
        scene->selection->nbSelection = 0;
    }
}

void Selection_selectionner_click_drag( Scene* scene )
{
    int i,k,j,x1,x2,y1,y2 = 0;

    if( (int)scene->selection->departSelection.x > (int)scene->selection->finSelection.x )
    {
        x1 = (int)scene->selection->finSelection.x;
        x2 = (int)scene->selection->departSelection.x;
    }
    else
    {
        x2 = (int)scene->selection->finSelection.x;
        x1 = (int)scene->selection->departSelection.x;
    }

    if( scene->selection->departSelection.y > scene->selection->finSelection.y )
    {
        y1 = (int)scene->selection->finSelection.y;
        y2 = (int)scene->selection->departSelection.y;
    }
    else
    {
        y2 = (int)scene->selection->finSelection.y;
        y1 = (int)scene->selection->departSelection.y;
    }

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet* objet = (Objet*)g_array_index( scene->tObjet, Objet*, i );
        gboolean estContenu = FALSE;

        for( k = x1; k < x2; k += 15 )
        {
            for( j = y1; j < y2; j += 15 )
            {
                 estContenu = estContenu || Objet_contient_point( objet, k, j );
                 if( estContenu )
                 {
                     k = x2;
                     j = y2;
                 }
            }
        }

        if( estContenu )
        {
            Selection_selectionner( scene->selection, objet );
        }
        else
        {
            Selection_deselectionner( scene->selection, objet );
        }
    }
}

void Selection_deselectionner( Selection* selection, Objet* objet )
{
    int i = 0;

    for( i = 0; i < selection->nbSelection; i++ )
    {
        if( objet == g_array_index( selection->tSelection, Objet*, i ) )
        {
            g_array_remove_index_fast( selection->tSelection, i );
            selection->nbSelection--;
            Objet_deselection( objet );
        }
    }
}

void Selection_selectionner( Selection* selection, Objet* objet )
{
    int i = 0;
    gboolean estDejaSelectionner = FALSE;

    for( i = 0; i < selection->nbSelection; i++ )
    {
        if( objet == g_array_index( selection->tSelection, Objet*, i ) )
        {
            estDejaSelectionner = TRUE;
        }
    }

    if( !estDejaSelectionner )
    {
        g_array_append_val( selection->tSelection, objet );
        selection->nbSelection++;
        Objet_selection( objet );
    }
}

gboolean Selection_est_deja_selectionner( Selection* selection, Objet* objet )
{
    int i = 0;

    for( i = 0; i < selection->nbSelection; i++ )
    {
        if( objet == g_array_index( selection->tSelection, Objet*, i ) )
        {
            return TRUE;
        }
    }
    return FALSE;
}

void Selection_deselectionner_tout( Selection* selection )
{
    int i = 0;

    for( i = 0; i < selection->nbSelection; i++ )
    {
        Objet* objet = (Objet*)g_array_index( selection->tSelection, Objet*, i );
        Objet_deselection( objet );
    }
}

void Selection_selectionner_tout( Scene* scene )
{
    int i = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet* objet = g_array_index( scene->tObjet, Objet*, i );
        g_array_append_val( scene->selection->tSelection, objet );
        scene->selection->nbSelection++;
        Objet_selection( objet );
    }
}

void Selection_dessiner_rectangle( Selection* selection, cairo_t* cr )
{
    if( selection->selection_en_cours )
    {
        cairo_set_source_rgba( cr, 51.0, 255.0, 255.0, 0.25 );
        cairo_rectangle( cr, selection->departSelection.x, selection->departSelection.y, selection->finSelection.x - selection->departSelection.x, selection->finSelection.y - selection->departSelection.y );
        cairo_fill( cr );
    }
}







