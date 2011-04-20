#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <math.h>

#include "Point.h"
#include "Cube.h"
#include "Objet.h"
#include "Selection.h"
#include "Scene.h"


// **************** Selection ***************************************
void Selection_initialiser( Selection* selection );

void Selection_selectionner_objet( Scene* scene, double x, double y );

void Selection_deselectionner_tout( Selection* selection );

void Selection_selectionner_tout( Scene* scene );

void Selection_selectionner_click_drag( Scene* scene );

void Selection_dessiner_rectangle( Selection* selection, cairo_t* cr );
