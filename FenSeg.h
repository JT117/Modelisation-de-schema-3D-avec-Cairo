#ifndef FENSEG_H
#define FENSEG_H

#include "Scene.h"
#include "Modification.h"
#include "Segment.h"

#include <gtk/gtk.h>

struct createSegment;

typedef struct FenSeg FenSeg;
struct FenSeg
{
	struct createSegment* pCreateSeg;
	GtkWidget* fenetre;
	Scene* scene;

	GtkWidget* layout; /* Layout général*/

    GtkWidget* hbox_checkSegment;
	GtkWidget* wArrowed;
	GtkWidget* wDashed;

	GtkWidget* hbox_color;
	GtkWidget* entryR; // R
	GtkWidget* entryG; // G
	GtkWidget* entryB; // B
	GtkWidget* entryA; // Alpha

	GtkWidget* hbox_button;
	GtkWidget* boutonOk;
	GtkWidget* boutonAnnuler;
};

void FenSeg_init( FenSeg* ft, Scene* scene);

/**
 * Va créer un nouveau segment avec les inf
 */
void FenSeg_newSegment( GtkButton* button, gpointer data);


#endif //FEN_TEXT_H
