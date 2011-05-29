#include <string.h>

#include "Text.h"

void Text_drawText(Text* pText, cairo_t* cr, InfoCamera* pCam)
{
	//cairo_text_extents_t te;
	cairo_set_source_rgba (cr, pText->tColor[0], pText->tColor[1], pText->tColor[2], pText->tColor[3]);
	cairo_select_font_face (cr, "Georgia",CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

	cairo_set_font_size (cr, pText->dSize);

	cairo_move_to(cr, pText->coordText[0], pText->coordText[1]);
	cairo_show_text(cr, pText->pText);
}

void Text_setColor(Text* pText, double dR, double dG, double dB, double dA)
{
	Color_setColor(&(pText->tColor),dR,dG,dB,dA);
}

Text* Text_createText(tdCoord2D tPos, char* pText)
{
	Text* pNewText = NULL;

	if( (pNewText= (Text*)malloc(sizeof(Text))) )
	{
		Point_initCoord2D(pNewText->coordText,tPos[0],tPos[1]);

		/* COuleur + taille par défait */
		pNewText->dSize = 10;
		Color_setColor( pNewText->tColor, 1.0, 1.0, 1.0, 1.0);

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

/**
 * Agrandit/retrécit un objet de type Text suivant le ratio fourni en param.
 * @param pRectangle Le Text en question
 * @param dRatio Le ratio d'agrandissement/rétrecissement
 */
void Text_modSize(Text* pText, double dRatio);
