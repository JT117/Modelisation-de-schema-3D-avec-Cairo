#include "Groupe.h"
#include "Scene.h"
#include "Objet.h"

void Groupe_initialiser( Groupe* groupe, Groupe* pere, int nb, double dX, double dY, double dZ )
{
    groupe->pere = pere;
    groupe->id = nb;
    groupe->tObjet = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
    groupe->nbObjet = 0;
    groupe->nbFils = 0;
    groupe->tFils = g_array_new( FALSE, FALSE, sizeof( Groupe* ) );
    groupe->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter) );

    Matrix_initIdentityMatrix(groupe->tTransfoMatrix); /*Initialisation de la matrice de transformaiton en tant que matrice identité */
    groupe->aTransfo = g_array_new(FALSE,FALSE,sizeof(Transfo*));

    /* Initialisation des coordonnées du repere du groupe */
    Point_initWorld(&(groupe->tCenterGroup),dX,dY,dZ);
    Point_initGroup(&(groupe->tCenterGroup),dX,dY,dZ);
    if( pere != NULL )
    {
    	Point_initGroup(&(groupe->tCenterGroup), dX-pere->tCenterGroup.tdCoordWorld[0],dY-pere->tCenterGroup.tdCoordWorld[1],dZ-pere->tCenterGroup.tdCoordWorld[2]);
    }

    if( groupe->id == 0)
    {
        GtkWidget* label = gtk_label_new("Groupe 0");
        groupe->nom = (char*)gtk_label_get_text( GTK_LABEL( label ) );
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


void Groupe_transfoCenter(Groupe* pGroup, tdMatrix tdTransfo)
{
	tCoord tCoordApTransfo;

	Point_initCoord(tCoordApTransfo, 0.0, 0.0, 0.0);
	/*APplication de la transformation au centre du groupe */
	Matrix_multiMatrixVect(tdTransfo, pGroup->tCenterGroup.tdCoordGroup, tCoordApTransfo);

	pGroup->tCenterGroup.tdCoordGroup[0] = tCoordApTransfo[0];
	pGroup->tCenterGroup.tdCoordGroup[1] = tCoordApTransfo[1];
	pGroup->tCenterGroup.tdCoordGroup[2] = tCoordApTransfo[2];
	pGroup->tCenterGroup.tdCoordGroup[3] = tCoordApTransfo[3];

	pGroup->bVisited = TRUE;
}

void Groupe_transfo(Groupe* pGroup, tdMatrix tdTransfo)
{
	int i;
	tCoord tCoordApTransfo;
	Objet* pObj = NULL;
	Groupe* pGroupSon = NULL;
	tdMatrix tdNewTransfo; /* Création d'une nouvelle matrice pour ne pas souiller celle passée en paramètre*/

	pGroup->bVisited = TRUE; /* On visite le groupe */
	/* On applique la transformation sur le centre du groupe */
	Matrix_multiMatrixVect(tdTransfo, pGroup->tCenterGroup.tdCoordGroup, tCoordApTransfo);
	pGroup->tCenterGroup.tdCoordGroup[0] = tCoordApTransfo[0];
	pGroup->tCenterGroup.tdCoordGroup[1] = tCoordApTransfo[1];
	pGroup->tCenterGroup.tdCoordGroup[2] = tCoordApTransfo[2];
	pGroup->tCenterGroup.tdCoordGroup[3] = tCoordApTransfo[3];

	Matrix_copy(tdNewTransfo,tdTransfo);
	/* Calcul de la nouvelle matrice de transformation en prenant en compte les transformations du groupe courant */
	Matrix_multiMatrices(tdNewTransfo,pGroup->tTransfoMatrix);  /* ATTENTION A CE QUE LA MATRICE DU GROUPE SOIT BIEN A JOUR*/

	for(i=0;i<pGroup->tObjet->len;++i)  /* On applique la transformation sur tous les objets du groupe */
	{
		pObj = g_array_index( pGroup->tObjet, Objet*, i ); /* Récupération de l'adresse de l'objet */
		Objet_transfoCenter(pObj, tdNewTransfo);
		Objet_transfo(pObj, tdNewTransfo); /*on applique la transformation à l'objet*/
	}

	if( pGroup->tFils->len != 0 ) /* Des appels recursifs sont à faire pour chaque groupe fils */
	{
		for(i=0;i<pGroup->tFils->len;++i)  /* On applique la transformation sur tous les fils du groupe */
		{
				pGroupSon = g_array_index( pGroup->tFils, Groupe*, i ); /* Récupération de l'adresse du groupe*/
				Groupe_transfo(pGroupSon, tdNewTransfo);
		}
	}
	/* Pas de groupe fils, plus rien à faire */

}

void Groupe_updateCoordWorld(Groupe* pGroup)
{
	int j;
	tCoord tCoordBefore, tCoordAfter;
	tdMatrix tdMatPass;
	Groupe* pFatherGroup = pGroup->pere;

	if( pFatherGroup != NULL && pFatherGroup->id != GROUPE0)
	{
		pFatherGroup = pFatherGroup->pere; /* On a déjà les coordonées du groupe dans le groupe pere, on remonte d'un groupe*/

		for(j=0;j<4;j++)  /* initialisation du tableau de coordonnées avant tout changement de base */
			tCoordAfter[j] = pGroup->tCenterGroup.tdCoordGroup[j];

		Matrix_initIdentityMatrix(tdMatPass); /* Initialisation de la matrice pour construction d'un matrice de passage */
		while( pFatherGroup->id != GROUPE0 ) /* tant qu'on est pas revenu à la racine on effectue un changement de base */
		{
			for(j=0;j<4;j++)
				tCoordBefore[j] = tCoordAfter[j];

			/* Construction nouvelle matrice de passage */
			tdMatPass[0][3] = pFatherGroup->tCenterGroup.tdCoordGroup[0];
			tdMatPass[1][3] = pFatherGroup->tCenterGroup.tdCoordGroup[1];
			tdMatPass[2][3] = pFatherGroup->tCenterGroup.tdCoordGroup[2];

			Matrix_multiMatrixVect(tdMatPass, tCoordBefore, tCoordAfter); /* Nouveau changement de base */
			pFatherGroup = pFatherGroup->pere;
		}
		/* Ici on est en possession des coordonnées de notre point dans le repere du monde (groupe0) dans tCoordAfter */
		Point_initWorld(&(pGroup->tCenterGroup), tCoordAfter[0],tCoordAfter[1],tCoordAfter[2]);
	}
	else /* Groupe de base */
	{
		if( pFatherGroup == NULL )
			Point_initWorld(&(pGroup->tCenterGroup), 0.0, 0.0 ,0.0);
		else
			Point_initWorld(&(pGroup->tCenterGroup), pGroup->tCenterGroup.tdCoordGroup[0], pGroup->tCenterGroup.tdCoordGroup[1] ,pGroup->tCenterGroup.tdCoordGroup[2]);
	}
}

void Groupe_drawMark(Groupe* pGroup,cairo_t* cr, InfoCamera* pCam)
{
	tCoord2D* pPointProj0 = NULL; /* Coordonnée du centre du repere une fois projetté */

	Groupe_updateCoordWorld(pGroup);
	/*On dessine le repere */
	pPointProj0 = ProjectionTools_getPictureCoord(&(pGroup->tCenterGroup),pCam);

	free(pPointProj0);
	cairo_arc (cr, (*pPointProj0)[0], (*pPointProj0)[1], 2, 0.0, 2*M_PI);
	cairo_set_source_rgba (cr, 1, 0.0, 0.0, 1); /*Couleur */
	cairo_fill( cr );/*remplissage du rectangle avec path preservé*/
}

void Groupe_unvisit(Groupe* pGroup)
{
	pGroup->bVisited = FALSE;
}

void Groupe_sauvegarde( Groupe* groupe, FILE* fichier )
{
    if( fichier != NULL )
    {
        fprintf( fichier, "%d %s %d %f %f %f\n", groupe->id, groupe->nom, groupe->pere->id, groupe->tCenterGroup.tdCoordGroup[0],
                                                 groupe->tCenterGroup.tdCoordGroup[0], groupe->tCenterGroup.tdCoordGroup[0] );
        fprintf( fichier , "%d\n", groupe->aTransfo->len );

        int i = 0;
        for( i = 0; i < groupe->aTransfo->len; i++ )
        {
            Transfo* transfo = g_array_index( groupe->aTransfo, Transfo*, i );
            if( transfo->eTransfoType == ROTATION_RECU )
            {
                fprintf( fichier, "Rotation_RECU %f %f %f\n", transfo->x, transfo->y, transfo->z );
            }
            else if( transfo->eTransfoType == TRANSLATION_RECU )
            {
                fprintf( fichier, "Translation_RECU %f %f %f\n", transfo->x, transfo->y, transfo->z );
            }
        }

    }
    /* TODO mettre un avertissement d'echec de sauvegarde */
}

void Groupe_restaure( FILE* fichier, Scene* scene )
{
    int idGroupePere = 0;
    int idGroupe = 0;
    char* nom = (char*)malloc( 35 * sizeof( char ) );

    Groupe* groupe = (Groupe*)malloc( 1 * sizeof( Groupe ) );
    groupe->tObjet = g_array_new( FALSE, FALSE, sizeof( Objet* ) );
    groupe->nbObjet = 0;
    groupe->nbFils = 0;
    groupe->tFils = g_array_new( FALSE, FALSE, sizeof( Groupe* ) );
    groupe->iter = (GtkTreeIter*)malloc( 1 * sizeof( GtkTreeIter) );

    fscanf( fichier, "%d %s %d %f %f %f\n", &idGroupe, nom, &idGroupePere, (float*)&groupe->tCenterGroup.tdCoordGroup[0],
                                                 (float*)&groupe->tCenterGroup.tdCoordGroup[0], (float*)&groupe->tCenterGroup.tdCoordGroup[0] );

    groupe->id = idGroupe;
    groupe->nom = nom;

    Groupe* pere = Groupe_trouver_ById( scene, idGroupePere );
    Groupe_ajouter_Fils( pere, groupe );

    gtk_tree_store_append (scene->store, groupe->iter, pere->iter);
    gtk_tree_store_set (scene->store, groupe->iter, GROUPE, groupe->nom, -1);

    g_array_append_val( scene->tGroupe, groupe );
    scene->nbGroupe++;

    int nb,i =0;
    fscanf( fichier , "%d", &nb );

    for( i = 0; i < nb; i++ )
    {
        float a,b,c;
        char* temp = (char*)malloc( 20 * sizeof( char ) );
        Transfo* transfo = (Transfo*)malloc( 1 * sizeof( Transfo ) );
        fscanf( fichier, "%s %f %f %f", temp, &a, &b, &c );

        transfo->x = a;
        transfo->y = b;
        transfo->z = c;

        if( strcmp( temp, "Rotation_RECU") == 0 )
        {
            transfo->eTransfoType = ROTATION_RECU;    //Ne sera effectuer qu'apres creation de l'arbre des groupes en entier
        }
        else if( strcmp( temp, "Translation") == 0  )
        {
            transfo->eTransfoType = TRANSLATION_RECU;
        }

        g_array_append_val( groupe->aTransfo, transfo ); //Ne sera effectuer qu'apres creation de l'arbre des groupes en entier
        free(temp);
    }

    Modification_modification_effectuer( scene );
}
