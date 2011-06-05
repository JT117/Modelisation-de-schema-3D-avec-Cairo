#include <math.h>

#include "Transformation.h"


void Transformation_getMatrixTranslation(tdMatrix pMatModif, double dX, double dY, double dZ)
{
	int i = 0, j=0;

	for(i=0; i<4;i++)
	{
		for(j=0; j<4;j++)
		{
			if(i==j)
				pMatModif = 1;
		}
	}

	pMatModif[0][3] = dX;
	pMatModif[1][3] = dY;
	pMatModif[2][3] = dZ;
}

void Transformation_getMatrixRotation(tdMatrix pMatModif, double dAngle, enum axe eAxe)
{
	if(eAxe == AXEX)
	{
		pMatModif[0][0] = 1;
		pMatModif[1][0] = 0;
		pMatModif[2][0] = 0;
		pMatModif[3][0] = 0;

		pMatModif[0][1] = 0;
		pMatModif[1][1] = cos(dAngle);
		pMatModif[2][1] = sin(dAngle);
		pMatModif[3][1] = 0;

		pMatModif[0][2] = 0;
		pMatModif[1][2] = -sin(dAngle);
		pMatModif[2][2] = cos(dAngle);
		pMatModif[3][2] = 0;

		pMatModif[0][3] = 0;
		pMatModif[1][3] = 0;
		pMatModif[2][3] = 0;
		pMatModif[3][3] = 1;

	}
	else if( eAxe == AXEY )
	{
		pMatModif[0][0] = cos(dAngle);
		pMatModif[1][0] = 0;
		pMatModif[2][0] = -sin(dAngle);
		pMatModif[3][0] = 0;

		pMatModif[0][1] = 0;
		pMatModif[1][1] = 1;
		pMatModif[2][1] = 0;
		pMatModif[3][1] = 0;

		pMatModif[0][2] = sin(dAngle);
		pMatModif[1][2] = 0;
		pMatModif[2][2] = cos(dAngle);
		pMatModif[3][2] = 0;

		pMatModif[0][3] = 0;
		pMatModif[1][3] = 0;
		pMatModif[2][3] = 0;
		pMatModif[3][3] = 1;

	}
	else if( eAxe == AXEZ )
	{
		/* Construction de la matrice de transformation qui va bien */
		pMatModif[0][0] = cos(dAngle);
		pMatModif[1][0] = sin(dAngle);
		pMatModif[2][0] = 0;
		pMatModif[3][0] = 0;

		pMatModif[0][1] = -sin(dAngle);
		pMatModif[1][1] = cos(dAngle);
		pMatModif[2][1] = 0;
		pMatModif[3][1] = 0;

		pMatModif[0][2] = 0;
		pMatModif[1][2] = 0;
		pMatModif[2][2] = 1;
		pMatModif[3][2] = 0;

		pMatModif[0][3] = 0;
		pMatModif[1][3] = 0;
		pMatModif[2][3] = 0;
		pMatModif[3][3] = 1;
	}
}

void Transformation_getMatrixHomothety(tdMatrix pMatModif, double dRatio)
{
	/* Construction de la matrice de transformation qui va bien */
	pMatModif[0][0] = dRatio;
	pMatModif[1][0] = 0;
	pMatModif[2][0] = 0;
	pMatModif[3][0] = 0;

	pMatModif[0][1] = 0;
	pMatModif[1][1] = dRatio;
	pMatModif[2][1] = 0;
	pMatModif[3][1] = 0;

	pMatModif[0][2] = 0;
	pMatModif[1][2] = 0;
	pMatModif[2][2] = dRatio;
	pMatModif[3][2] = 0;

	pMatModif[0][3] = 0;
	pMatModif[1][3] = 0;
	pMatModif[2][3] = 0;
	pMatModif[3][3] = 1;
}
