#include <string.h>

#include "Text.h"


void Text_drawText(Text* pText, cairo_t* cr, InfoCamera* pCam)
{
	cairo_text_extents_t te;
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_select_font_face (cr, "Georgia",CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 1.2);
	cairo_text_extents (cr, "a", &te);
	cairo_move_to (cr, 0.5 - te.width / 2 - te.x_bearing,
	    0.5 - te.height / 2 - te.y_bearing);
	cairo_show_text (cr, "a");
}

Text* Text_createText(tdCoord2D tPos, char* pText)
{
	Text* pNewText = NULL;

	if( (pNewText= (Text*)malloc(sizeof(Text))) )
	{
		pNewText->coordText[0]=tPos[0];
		pNewText->coordText[1]=tPos[1];

		if( (pNewText->pText= (char*)malloc(sizeof(pText))) )
		{
			 strcpy(pNewText->pText,pText);
		}
		else
		{
			free(pNewText);
			//fatalerror
		}
	}
	else
	{
		//TODO : fatalerror
	}
	return pNewText;
}

void Text_destroy(Text* pText)
{
	if(pText->pText != NULL)
	{
		free(pText->pText);
	}
	free(pText);
}

gboolean Text_Contient_Point( Text* pText, double x, double y );

/*
int scalaire_result( Point a, Point b, int x, int y );
*/

/**
 * Agrandit/retrécit un objet de type Text suivant le ratio fourni en param.
 * @param pRectangle Le Text en question
 * @param dRatio Le ratio d'agrandissement/rétrecissement
 */
void Text_modSize(Text* pText, double dRatio);
