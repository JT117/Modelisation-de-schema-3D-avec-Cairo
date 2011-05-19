#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <math.h>

#include "Point.h"
#include "Cube.h"
#include "Objet.h"
#include "Scene.h"
#include "Modification.h"
#include "Clavier.h"
#include "Rectangle.h"

// **************** Selection ***************************************


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



