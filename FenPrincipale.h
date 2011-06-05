#include "FenetreAjout.h"
#include "Cube.h"
#include "Objet.h"
#include "Scene.h"
#include "Config.h"
#include "FenetrePropriete.h"
#include "ExportWindow.h"
#include "Enum.h"
#include "Selection.h"
#include "Groupe.h"
#include "FenTrans.h"
#include "FenText.h"

 gboolean realize_callback( GtkWidget *widget, GdkEventExpose *event, gpointer data );
 gboolean expose_event_callback (GtkWidget *widget, GdkEventExpose *event,gpointer data);
 gboolean gestion_clavier(GtkWidget *window, GdkEventKey* event, gpointer data);
 gboolean gestion_souris_callback(GtkWidget *window, GdkEventButton* event, gpointer data);
 gboolean nouveau_cube( GtkWidget *menuItem, gpointer data );
 gboolean main_ouvrir( GtkWidget *menuItem, gpointer data );
 gboolean main_sauvegarder( GtkWidget *menuItem, gpointer data );
 gboolean main_annuler( GtkWidget *menuItem, gpointer data );
 gboolean main_refaire( GtkWidget *menuItem, gpointer data );
 gboolean main_quitter( GtkWidget *menuItem, gpointer data );
 gboolean main_nouveau( GtkWidget *menuItem, gpointer data );
 gboolean nouvelle_transformation( GtkWidget *menuItem, gpointer data );
 gboolean main_supprimer( GtkWidget *menuItem, gpointer data );
 gboolean nouveau_propriete( GtkWidget *menuItem, gpointer data );
 gboolean selectionChanged(GtkTreeSelection *selection, gpointer data);
 gboolean clickDroitGroupe(GtkWidget *window, GdkEventButton* event, gpointer data);
 gboolean nouveau_groupe(GtkWidget *menuItem, gpointer data );
 gboolean ajout_Groupe( GtkButton* button, gpointer data );
 gboolean supprimer_Groupe( GtkWidget* button, gpointer data );
 gboolean suppression_Groupe( GtkButton* button, gpointer data );
 gboolean changementCurseur( GtkButton* button, gpointer data );


/**
 * Appelle la fonction de création de la fenêtre d'ajout de texte.
 */
 gboolean newText(gpointer data);

/**
 * Fonction qui va appeler la fonction de création de la fenêtre d'exportation.
 */
 gboolean main_export(GtkWidget *menuItem, gpointer data );

int FenPrincipale_initialiser (int argc, char* argv[] );

