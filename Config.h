#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <math.h>

#include "Point.h"
#include "Cube.h"
#include "Objet.h"
#include "Selection.h"
#include "Scene.h"
#include "Modification.h"
#include "Clavier.h"
#include "Rectangle.h"


// **************** Selection ***************************************
void Selection_initialiser( Selection* selection );

void Selection_selectionner_objet( Scene* scene, double x, double y );

void Selection_deselectionner_tout( Selection* selection );

void Selection_selectionner_tout( Scene* scene );

void Selection_selectionner_click_drag( Scene* scene );

void Selection_dessiner_rectangle( Selection* selection, cairo_t* cr );

void Selection_detruire( Selection* selection );

gboolean Selection_est_deja_selectionner( Selection* selection, Objet* objet );

void Selection_deselectionner( Selection* selection, Objet* objet );

void Selection_selectionner( Selection* selection, Objet* objet );

//*******************Modification************************************
void Modification_modification_effectuer( Scene* scene );

void Modification_initialiser( Modification* modif );

void Modification_annuler( Scene* scene );

void Modification_refaire( Scene* scene );

void Modification_detruire_temporaire( Modification* modif );

//*******************Clavier*******************************************

gboolean Clavier_est_appuyer( Scene* scene, char* touche );

void Clavier_initialiser( Clavier* clavier );

void Clavier_detruire( Clavier* clavier );

void Clavier_touche_appuyer( Scene* scene, char* nomTouche );

void Clavier_touche_relacher( Scene* scene, char* nomTouche );



