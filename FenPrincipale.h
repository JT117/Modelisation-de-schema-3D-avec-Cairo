#include "FenetreAjoutCube.h" /* TODO : réflechir à un moyen de ne faire qu'un seule module FenetreAjout */
#include "Cube.h"
#include "Objet.h"
#include "Scene.h"
#include "Config.h"
#include "FenetrePropriete.h"
#include "Enum.h"
#include "Selection.h"

static gboolean realize_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data);
static gboolean expose_event_callback (GtkWidget *widget, GdkEventExpose *event,gpointer data);
static gboolean gestion_clavier(GtkWidget *window, GdkEventKey* event, gpointer data);
static gboolean gestion_souris_callback(GtkWidget *window, GdkEventButton* event, gpointer data);
static gboolean nouveau_cube( GtkWidget *menuItem, gpointer data );
static gboolean main_ouvrir( GtkWidget *menuItem, gpointer data );
static gboolean main_sauvegarder( GtkWidget *menuItem, gpointer data );
static gboolean main_annuler( GtkWidget *menuItem, gpointer data );
static gboolean main_refaire( GtkWidget *menuItem, gpointer data );
static gboolean main_quitter( GtkWidget *menuItem, gpointer data );
static gboolean main_nouveau( GtkWidget *menuItem, gpointer data );
static gboolean main_supprimer( GtkWidget *menuItem, gpointer data );
static gboolean nouveau_propriete( GtkWidget *menuItem, gpointer data );
static gboolean selectionChanged(GtkTreeSelection *selection, gpointer data);
static gboolean clickDroitGroupe(GtkWidget *window, GdkEventButton* event, gpointer data);
static gboolean nouveau_groupe(GtkWidget *menuItem, gpointer data );
static gboolean ajout_Groupe( GtkButton* button, gpointer data );
static gboolean supprimer_Groupe( GtkWidget* button, gpointer data );
static gboolean suppression_Groupe( GtkButton* button, gpointer data );
static gboolean changementCurseur( GtkButton* button, gpointer data );
