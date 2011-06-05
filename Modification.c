#include "Modification.h"
#include "Config.h"

void Modification_initialiser( Modification* modif )
{
    modif->actuel = -1;
    modif->nbMax = -1;
}

void Modification_detruire_temporaire( Modification* modif )
{
    int i = 0;

    for( i = 0; i <= modif->nbMax; i++ )
    {
        char* filename = (char*)malloc( 20* sizeof( char ) );
        strcpy( filename, "systeme/temporaire/" );
        char number[3];
        sprintf( number, "%d.txt", i );
        filename = (char*)realloc( filename, (size_t)(strlen(filename) + 7) * sizeof(char) );
        filename = strcat( filename, number );
        printf(" Suppression du fichier temporaire : %s\n", filename );
        remove( filename );
        free( filename );
    }
}

void Modification_modification_effectuer( Scene* scene )
{
    if( scene->modification->actuel < 500 )
    {
        char* filename = (char*)malloc( 20* sizeof( char ) );
        strcpy( filename, "systeme/temporaire/" );
        char number[3];

        scene->modification->actuel++;
        scene->modification->nbMax++;
        sprintf( number, "%d.txt", scene->modification->actuel );

        filename = (char*)realloc( filename, (size_t)(strlen(filename) + 7) * sizeof(char) );
        filename = strcat( filename, number );

        FILE* fichier = NULL;

        fichier = fopen( filename, "w" );

        if( fichier != NULL )
        {
            int i = 0;
            fprintf( fichier, "DEBUT\n");
            fprintf( fichier, "%d\n", scene->nbGroupe-1 );

            for( i = 1; i < scene->nbGroupe; i++ )
            {
                Groupe* groupe = (Groupe*)g_array_index( scene->tGroupe, Groupe*, i );
                Groupe_sauvegarde( groupe, fichier );
            }

            fprintf( fichier, "%d\n", scene->nbObjet );

            for( i = 0; i < scene->nbObjet; i++ )
            {
                Objet* objet = (Objet*)g_array_index( scene->tObjet, Objet*, i );
                Objet_sauvegarde( objet, fichier );
            }
            fprintf( fichier, "FIN");
            fclose( fichier );

            if( scene->modification->actuel >= 1 )
            {
                gtk_widget_set_sensitive( scene->modification->annuler, TRUE );
            }

            gtk_widget_set_sensitive( scene->modification->refaire, FALSE );

        }
    }
}

void Modification_annuler( Scene* scene )
{
    if( scene->modification->actuel > 0 )
    {
        char* filename = (char*)malloc( 20* sizeof( char ) );
        char number[2];
        strcpy( filename, "systeme/temporaire/" );

        Scene_reset( scene, scene->zoneDeDessin );

        scene->modification->actuel--;
        sprintf( number, "%d.txt", scene->modification->actuel );

        filename = (char*)realloc( filename, (size_t)(strlen(filename) + 7) * sizeof(char) );
        filename = strcat( filename, number );
        printf("%s\n", filename );

        FILE* fichier = NULL;

        fichier = fopen( filename, "r" );

        if( fichier != NULL )
        {
           char deb[5] = "    ";
            fscanf( fichier, "%s", deb );

            if( strcmp( deb, "DEBUT" ) == 0 )
            {
                int i = 0;
                int nbGroupe = 0;
                int nbObjet = 0;
                fscanf( fichier, "%d", &nbGroupe );

                for( i = 0; i < nbGroupe; i++ )
                {
                    Groupe_restaure( fichier, scene );
                }

                fscanf( fichier, "%d", &nbObjet );

                for( i = 0; i < nbObjet; i++ )
                {
                    Objet_restaure( fichier, scene );
                }

                for( i = 0; i < nbGroupe; i++ ) //Application des tranfos des groupes
                {
                    int j = 0;
                    Groupe* groupe = g_array_index( scene->tGroupe, Groupe*, i );

                    for( j = 0; j < groupe->aTransfo->len; j++ )
                    {
                        Transfo* transfo = g_array_index( groupe->aTransfo, Transfo*, j );
                        if( transfo->eTransfoType == ROTATION_RECU )
                        {
                           tdMatrix tdTransfoMat;
                            if( transfo->x > 0 )
                            {
                                Transformation_getMatrixRotation( tdTransfoMat, transfo->x, AXEY );
                            }
                            if( transfo->y > 0 )
                            {
                                Transformation_getMatrixRotation( tdTransfoMat, transfo->y, AXEX );
                            }
                                    /* On applique la transfo pour tous les groupes fils */
                            for( j=0;j<groupe->tFils->len;++j)
                            {
                                    Groupe* pSon = g_array_index(groupe->tFils,Groupe*,j);   // pSon est un pointeur sur un groupe fils
                                    Groupe_transfo( pSon, tdTransfoMat);   // appel recursif de Groupe_transfo jusqu'à la fin de l'arbre
                            }
                            /* et pour les objets du groupe */
                            for( j=0;j<groupe->tObjet->len;++j)
                            {
                                    Objet* pObj = g_array_index(groupe->tObjet,Objet*,j);
                                    Objet_transfoCenter(pObj, tdTransfoMat);   // on fait tourner le centre du repre objet
                                    Objet_transfo( pObj , tdTransfoMat);    // ainsi qu l'intégralité de ses points
                            }
                        } /*TODO meme chose avec les translations*/
                    }
                }
            }

        }
        if( scene->modification->actuel < 1 )
        {
            gtk_widget_set_sensitive( scene->modification->annuler, FALSE );
        }
        gtk_widget_set_sensitive( scene->modification->refaire, TRUE );
        gtk_widget_queue_draw( scene->zoneDeDessin );
    }
}

void Modification_refaire( Scene* scene )
{
    if( scene->modification->nbMax > scene->modification->actuel )
    {
        char* filename = (char*)malloc( 20* sizeof( char ) );
        char number[2];
        strcpy( filename, "systeme/temporaire/" );

        Modification* modif = (Modification*)malloc( 1 * sizeof( char ) );
        modif->actuel = scene->modification->actuel;
        modif->nbMax = scene->modification->nbMax;
        GtkWidget* annuler = scene->modification->annuler;
        GtkWidget* refaire = scene->modification->refaire;

        Scene_reset( scene, scene->zoneDeDessin );

        scene->modification->annuler = annuler;
        scene->modification->refaire = refaire;
        scene->modification->actuel = modif->actuel;
        scene->modification->nbMax = modif->nbMax;
        free( modif );


        scene->modification->actuel++;
        sprintf( number, "%d.txt", scene->modification->actuel );

        filename = (char*)realloc( filename, (size_t)(strlen(filename) + 7) * sizeof(char) );
        filename = strcat( filename, number );
        printf("%s\n", filename );

        FILE* fichier = NULL;

        fichier = fopen( filename, "r" );

        if( fichier != NULL )
        {
            char deb[5] = "     ";
            fscanf( fichier, "%s", deb );

            if( strcmp( deb, "DEBUT" ) == 0 )
            {
                char typeObjet[20];
                fscanf( fichier, "%s", typeObjet );

                while( strcmp( typeObjet, "FIN") != 0 )
                {

                    float x,y,z = 0;
                    float taille = 0;
                    float r,g,b,a = 0;

                    fscanf( fichier, "%s", typeObjet );
                    fscanf( fichier, "%f %f %f", &x, &y, &z );
                    fscanf( fichier, "%f", &taille );
                    fscanf( fichier, "%f %f %f %f", &r, &g, &b, &a );

                    Cube* cube = (Cube*)malloc( 1 * sizeof( Cube ) );
                    //initialiser_Cube( cube, x, y, z, taille );
                    Scene_ajouter_cube( scene, cube, 0 );

                    fscanf( fichier, "%s", typeObjet );
                }
            }
            fclose( fichier );
        }
        if( scene->modification->actuel == scene->modification->nbMax )
        {
            gtk_widget_set_sensitive( scene->modification->refaire, FALSE );
        }
        gtk_widget_set_sensitive( scene->modification->annuler, TRUE );
        gtk_widget_queue_draw( scene->zoneDeDessin );
    }
}




