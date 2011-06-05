#ifndef OBJET_H
#define OBJET_H

#include "Cube.h"
#include "Segment.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Quadrilateral.h"

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

struct Scene;
struct Quadrilateral;
struct Rectangle;
struct Pyramid;
struct Groupe;

typedef enum{ CUBE, RECTANGLE, QUADRILATERAL, TRIANGLE, SPHERE, SEGMENT, TEXTE, PYRAMID} typeObjet;

/** Structure representant un objet dans l'interface graphique, servant à masquer l'implementation des differents objets à la GUI */
typedef struct Objet Objet;
struct Objet
{
    char* texte;
    char* font;
    int x, y;

    /** Un string contenant le type d'objet */
    //char* typeObjet;
    typeObjet eType;

    /** Un flag indiquant si l'objet est à selectionner */
    gboolean doitEtreDeselectionner;
    /** Un int indiquant l'id du dernier groupe dont l'objet fait partie */
    int numeroGroupe;
    /** iteration de l'arbre pour placement dans l'arbre d'objet */
    GtkTreeIter* iter;

    /** Pointeur vers groupe pere **/
    struct Groupe* pFatherGroup;

    /** Union contenant le pointeur sur le bon type de l'objet */
    union{
            Cube* cube;
            struct Rectangle* rectangle;
            Segment* segment;
            Sphere* sphere;
            Triangle* triangle;
            struct Quadrilateral* quadrilateral;
            struct Pyramid* pyramid;
         }type;

	 /** Tableau des transformation à appliquer sur les objets du groupe concerné ainsi que tous les groupes fils **/
	 GArray* aTransfo;
	 /** Matrice de transformation comprenant toutes les transfo du tableau aTransfo  Tfinale = T1*T2*...*Tn **/
	 tdMatrix tTransfoMatrix;
};

/** Libere la memoire allouer dynamiquement pour un objet */
void Objet_detruire( Objet* objet );

/**
 * Va appeler la fonction updateCoordWorld spécifique à un objet  donné.
 */
void Objet_updateCoordWorld(Objet* pObj);

/** Initialise un Objet Cube */
void Objet_est_un_Cube( Objet* objet, Cube* cube );

/** Initialise un Objet Rectangle */
void Objet_est_un_Rectangle( Objet* pObj, struct Rectangle* pRect );
void Objet_est_un_Segment( Objet* pObj, Segment* pSeg );
void Objet_est_une_Sphere( Objet* pObj,struct Sphere* pSph );
void Objet_est_un_Triangle( Objet* objet, Triangle* cube );

/** Fonction capable de dessiner n'importe quel objet */
void Objet_dessiner_objet( Objet* objet, cairo_t* cr, InfoCamera* cam);


gboolean Objet_contient_point( Objet* objet, double x, double y, InfoCamera* pCam);

/** Selectionne l'objet */
void Objet_selection( Objet* objet );

/** Deselection l'objet */
void Objet_deselection( Objet* objet );

/** Recuperation des couleurs d'un objet */
void Objet_get_Color( Objet* objet, Color tColor );
void Objet_set_Color( Objet* objet, double r, double g, double b, double a );

/**
 * Méthode permettant d'appeler la méthode rotation associée au type d'objet passé en param.
 *
 * @param objet Pointeur vers l'objet concerné
 * @param x Déplacement de la souris en x
 * @param y Déplacement de la souris en y
 */
void Objet_rotation( Objet* objet, double x, double y );

/**
 * Méthode permettant d'appeler la méthode rotationWorld associée au type d'objet passé en param.
 *
 * @param objet Pointeur vers l'objet concerné
 * @param x Déplacement de la souris en x
 * @param y Déplacement de la souris en y
 */
void Objet_rotationWorld( Objet* objet, double x, double y );
void Objet_homothetie( Objet* objet, int ratio );

/**
 * Va permettre d'appliquer une transfoirmation donnée sur le centre d'un objet.
 *
 * @param objet L'objet concerné par la transformation
 * @param tdTransfo La matrice de transformation qui nous intéresse
 */
void Objet_transfoCenter(Objet* objet, tdMatrix tdTransfo);

/**
 *  Sauvegarde d'un objet dans le fichier de suvegarde.
 **/
void Objet_sauvegarde( Objet* objet, FILE* fichier );

void Objet_restaure( FILE* fichier, struct Scene* scene);

void Objet_transfo(Objet* objet, tdMatrix tdTransfo);


#endif  // OBJET_H
