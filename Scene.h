#ifndef SCENE_H
#define SCENE_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include <string.h>

#include "Segment.h"

struct Selection;

#include "Objet.h"
#include "Cube.h"
#include "Rectangle.h"
#include "Point.h"
#include "Modification.h"
#include "Clavier.h"
#include "Groupe.h"
#include "Enum.h"
#include "Camera.h"

/** Structure representant la scene */
typedef struct Scene Scene;
struct Scene
{
    /** Pointeur sur le module des modifications */
    Modification* modification;
    /** Pointeur sur le module de selection */
    struct Selection* selection;
    /** Tableau contenant tous les objets de la scene */
    GArray* tObjet;
    /** Int contenant le nombre d'objet dans le tableau tObjet */
    int nbObjet;

    /** Tableau contenant un pointeur sur tous les groupe de la scene */
    GArray* tGroupe;
    /** Int contenant le nombre de groupe dans le tableau tGroupe */
    int nbGroupe;
    /** Pointeur sur la module clavier */
    Clavier* clavier;
    /** Un point contenant les informations necessaires à l'ajout d'un nouvel element */
    Point* creation;
    float tailleCreation;
    /** Pointeur sur la zone de dessin */
    GtkWidget* zoneDeDessin;
    GtkWidget* mainWindow;
    GtkWidget* entry1; // X
    GtkWidget* entry2; // Y
    GtkWidget* entry3; // Z

    /** Caméra de notre scéne 3D **/
    InfoCamera* camera;
    /** Pointeur sur la base de l'arbre **/
    GtkTreeStore* store;
    /** Gere la selection dans le treeView */
    GtkTreeSelection* treeSelection;
    /** La vue de l'arbre */
    GtkWidget* tree;

    GtkWidget* CBajoutGroupe;
    GtkWidget* fenetre;
    GtkWidget* entryNom;

    int souris;
    Point rotation;

    GdkCursor* curseur;

    /*Taille de la surface de projection */
    double dHeight;
    double dWidth;
};


/**
 * Va contenir les informations relatives à un objet ou un groupe dans la perspective de classer les elements
 */
typedef struct ClassifyObj
{
	double dDistance; /*  distance entre la caméra et le centre de gravité de chaque face, servira pour classer les indexs de face*/
	Groupe* pGroup;
	Objet* pObj;
}ClassifyObj;


/** Fonction qui initialise une scene */
void Scene_initialiser_scene( Scene* scene, GtkWidget* window, GtkWidget* mainWindow  );

/** Fonction qui ajoute un cube à la scene */
void Scene_ajouter_cube( Scene* scene, Cube* cCube, int idGroupe );

void Scene_ajouter_rectangle( Scene* scene, Rectangle* rect, int idGroupe );

void Scene_ajouter_segment( Scene* scene, Segment* pSeg, int idGroupe );

void Scene_ajouter_sphere(Scene* scene, Sphere* sphere, int idGroupe );



/** Fonction qui eneleve un objet de la scene */
void Scene_enlever_objet( Scene* scene, Objet* objet );

/** Fonction qui dessine tout les objets de la scene */
void Scene_dessiner_scene( Scene* scene, cairo_t* cr );

/** Fonction qui est temporaire */
void Scene_creation_objet( Scene* scene, double x, double y );

/** Fontion qui nettoie une scene */
void Scene_clear_scene( Scene* scene, cairo_t* cr );

/** Fonction qui "detruit" de façon specifique une scene */
void Scene_detruire( Scene* scene );

/** Fonction qui fait un "reset" de la scene */
void Scene_reset( Scene* scene, GtkWidget* window );

/** Fonction qui recostruit la scene apres un reset */
void Scene_reconstruire( Scene* scene, GtkWidget* window );

void Scene_ajouter_triangle( Scene* scene, Triangle* pTri, int idGroupe );

void Scene_deleteClassifiedElements(GArray* tToDelete);

void Scene_ajouter_quadrilateral(Scene* scene, Quadrilateral* pQuadri, int idGroupe );


#endif // SCENE_H
