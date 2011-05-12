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

            for( i = 0; i < scene->nbObjet; i++ )
            {
                Objet* objet = (Objet*)g_array_index( scene->tObjet, Objet*, i );
                fprintf( fichier, "%s\n%d\n", objet->typeObjet, 0 ); // A changer quand l'implementation du groupe sera faite

                if( strcmp( objet->typeObjet, "Cube") == 0 )
                {
                    fprintf( fichier, "%f %f %f\n", objet->type.cube->tPoint[0].x, objet->type.cube->tPoint[0].y, objet->type.cube->tPoint[0].z );
                    fprintf( fichier, "%f\n", objet->type.cube->tPoint[1].x - objet->type.cube->tPoint[0].x );
                    fprintf( fichier, "%f %f %f %f\n", 0.150, 0.150, 0.150, 0.150 );
                }
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
            char deb[5] = "     ";
            fscanf( fichier, "%s", deb );

            if( strcmp( deb, "DEBUT" ) == 0 )
            {
                char typeObjet[20];
                fscanf( fichier, "%s", typeObjet );

                while( strcmp( typeObjet, "FIN") != 0 )
                {
                    int groupe = 0;
                    float x,y,z = 0;
                    float taille = 0;
                    float r,g,b,a = 0;

                    fscanf( fichier, "%s", typeObjet );
                    fscanf( fichier, "%f %f %f", &x, &y, &z );
                    fscanf( fichier, "%f", &taille );
                    fscanf( fichier, "%f %f %f %f", &r, &g, &b, &a );

                    Cube* cube = (Cube*)malloc( 1 * sizeof( Cube ) );
                    initialiser_Cube( cube, x, y, z, taille );
                    Scene_ajouter_cube( scene, cube, 0 );

                    fscanf( fichier, "%s", typeObjet );
                }
            }
            fclose( fichier );
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
                    int groupe = 0;
                    float x,y,z = 0;
                    float taille = 0;
                    float r,g,b,a = 0;

                    fscanf( fichier, "%s", typeObjet );
                    fscanf( fichier, "%f %f %f", &x, &y, &z );
                    fscanf( fichier, "%f", &taille );
                    fscanf( fichier, "%f %f %f %f", &r, &g, &b, &a );

                    Cube* cube = (Cube*)malloc( 1 * sizeof( Cube ) );
                    initialiser_Cube( cube, x, y, z, taille );
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




