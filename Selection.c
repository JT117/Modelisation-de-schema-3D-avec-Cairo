#include "Selection.h"
#include "Config.h"
#include "Objet.h"

/** Fonction qui initialise une structure selection
 * @param selection, un pointeur sur la selection à initialiser
 * @warning La selection est a libérer avec Selection_detruire
 **/
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
    selection->selection_multiple = FALSE;
}

/** Fonction qui libère la mémoire alloué pour une selection
 * @param selection, un pointeur sur une selection initialisée
 **/
void Selection_detruire( Selection* selection )
{
    int i = 0;

    for( i = 0; i < selection->nbSelection; i++ )
    {
        Objet_detruire( g_array_index( selection->tSelection, Objet*, i ) );
    }
    g_array_free( selection->tSelection, FALSE );
}

/** Fonction gérant la selection par clic simple
 * @param scene, un pointeur sur une scene initialisée
 * @param x, la coordonnées x à tester
 * @param y, la coordonnées y à tester
**/
void Selection_selectionner_objet( Scene* scene, double x, double y )   // A optimiser + Si un objet a une alloc dyn vider les tableaux avant de les free
{
    int i = 0;
    int nbNonSelectionner = 0;

    for( i = 0; i < scene->nbObjet; i++ )
    {
        Objet* objet = (Objet*)g_array_index( scene->tObjet, Objet*, i );
        gboolean estContenu = Objet_contient_point( objet, x, y, scene->camera );

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

            if( !estDejaSelectionner && scene->selection->selection_multiple )
            {
                g_array_append_val( scene->selection->tSelection, objet );
                scene->selection->nbSelection++;
                Objet_selection( objet );
                gtk_tree_selection_select_iter( scene->treeSelection,objet->iter );
            }
            else if( !estDejaSelectionner )
            {
                Selection_deselectionner_tout( scene );
                g_array_free( scene->selection->tSelection, FALSE );
                scene->selection->tSelection = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
                Selection_selectionner(scene, objet );
            }
            else if( !scene->selection->selection_multiple )
            {
              Selection_deselectionner( scene, objet );
            }
        }
        else if( !estContenu )
        {
            nbNonSelectionner++;
        }
    }

    if( nbNonSelectionner == scene->nbObjet )
    {
        Selection_deselectionner_tout( scene );
    }
}

/** Fonction gérant la selection par rectangle dynamique
 * @param scene, un pointeur sur une scene initialisée
 **/
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
                 estContenu = estContenu || Objet_contient_point( objet, k, j, scene->camera );
                 if( estContenu )
                 {
                     k = x2;
                     j = y2;
                 }
            }
        }

        if( estContenu )
        {
            Selection_selectionner( scene, objet );
        }
        else
        {
            Selection_deselectionner( scene, objet );
        }
    }
}

/** Fonction qui deselectionne l'objet, objet
 * @param selection , un pointeur sur une selection initialisée
 * @param objet, un pointeur sur l'objet à deselectionner
 **/
void Selection_deselectionner( Scene* scene, Objet* objet )
{
    int i = 0;

    for( i = 0; i < scene->selection->nbSelection; i++ )
    {
        if( objet == g_array_index( scene->selection->tSelection, Objet*, i ) )
        {
            g_array_remove_index( scene->selection->tSelection, i );
            scene->selection->nbSelection--;
            Objet_deselection( objet );
            gtk_tree_selection_unselect_iter( scene->treeSelection,objet->iter );
            gtk_widget_queue_draw( scene->tree );
        }
    }
}

/** Fonction qui selectionne l'objet objet
 * @param selection, un pointeur sur une selection initialisée
 * @param objet, un pointeur sur l'objet à selectionner
 **/
void Selection_selectionner( Scene* scene, Objet* objet )
{
    int i = 0;
    gboolean estDejaSelectionner = FALSE;

    for( i = 0; i < scene->selection->nbSelection; i++ )
    {
        if( objet == g_array_index( scene->selection->tSelection, Objet*, i ) )
        {
            estDejaSelectionner = TRUE;
        }
    }

    if( !estDejaSelectionner )
    {
        g_array_append_val( scene->selection->tSelection, objet );
        scene->selection->nbSelection++;
        Objet_selection( objet );
        gtk_tree_selection_select_iter( scene->treeSelection, objet->iter );
        gtk_widget_queue_draw( scene->tree );
    }
}

/** Fonction qui renvoie si un objet est deja selectionné
 * @param selection, un pointeur sur une selection initialisé
 * @param objet, un pointeur sur l'objet à tester
 **/
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

/** Fonction qui deselectionne tout les objets
 * @param selection, un pointeur sur une selection initialisée
 **/
void Selection_deselectionner_tout( Scene* scene )
{
    int i = 0;

    for( i = 0; i < scene->selection->nbSelection; i++ )
    {
        Objet* objet = (Objet*)g_array_index( scene->selection->tSelection, Objet*, i );
        Objet_deselection( objet );
    }

    g_array_free( scene->selection->tSelection, FALSE );
    scene->selection->tSelection = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
    scene->selection->nbSelection = 0;

    gtk_tree_selection_unselect_all( scene->treeSelection );
}

/** Fonction qui selectionne tous les objets de la scene
 * @param scene, un pointeur sur une scene initialisée
 **/
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
    gtk_tree_selection_select_all( scene->treeSelection );
}

/** Fonction qui dessine le rectangle de selection dynamique à l'ecran
 * @param selection, un pointeur sur une selection initialisée
 * @param cr, un pointeur sur un contexte cairo pour dessiner sur une zoneDeDessin
 **/
void Selection_dessiner_rectangle( Selection* selection, cairo_t* cr )
{
    if( selection->selection_en_cours )
    {
        cairo_set_source_rgba( cr, 51.0, 255.0, 255.0, 0.25 );
        cairo_rectangle( cr, selection->departSelection.x, selection->departSelection.y, selection->finSelection.x - selection->departSelection.x, selection->finSelection.y - selection->departSelection.y );
        cairo_fill( cr );
    }
}

gboolean Selection_inFace(tCoord2D tP1,tCoord2D tP2,tCoord2D tP3, tCoord2D tP4, double dXClick, double dYClick )
{
	int iNb = 0, iLoop = 0;
	double tDistanceClick[2]; /* Distance (sur x et y) entre la position du curseur et chaque point  */
	double tDistancePoints[2]; /* Distance entre deux points d'une arrête*/
	double dDet = 0;
	tCoord2D tCoordClick;

	Point_initCoord2D(tCoordClick, dXClick, dYClick); /* Coordonnées du clique */

	/* On passe chaque arrête en revue */
	for(iLoop=0; iLoop<4; iLoop++)
	{
		switch(iLoop)
		{
			case 0:
			{
				tDistanceClick[0] =tCoordClick[0]-tP1[0];
				tDistanceClick[1] = tCoordClick[1]-tP1[1];
				tDistancePoints[0] = tP2[0]-tP1[0];
				tDistancePoints[1] = tP2[1]-tP1[1];
				break;
			}
			case 1:
			{
				tDistanceClick[0] = tCoordClick[0]-tP2[0];
				tDistanceClick[1] = tCoordClick[1]-tP2[1];
				tDistancePoints[0] = tP3[0]-tP2[0];
				tDistancePoints[1] = tP3[1]-tP2[1];
				break;
			}
			case 2:
			{
				tDistanceClick[0] = tCoordClick[0]-tP3[0];
				tDistanceClick[1] = tCoordClick[1]-tP3[1];
				tDistancePoints[0] = tP4[0]-tP3[0];
				tDistancePoints[1] = tP4[1]-tP3[1];
				break;
			}
			case 3:
			{
				tDistanceClick[0] = tCoordClick[0]-tP4[0];
				tDistanceClick[1] = tCoordClick[1]-tP4[1];
				tDistancePoints[0] = tP1[0]-tP4[0];
				tDistancePoints[1] = tP1[1]-tP4[1];
				break;
			}
		}
		dDet = Point_determinant(tDistancePoints,tDistanceClick );

		if( dDet > 0)
				iNb++;
		else if( dDet < 0)
				iNb--;
	}

    if( iNb == 4 || iNb == -4 )
        return TRUE;
    else
    	return FALSE;
}







