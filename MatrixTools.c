#include <stdio.h>
#include <stdlib.h>

#include "MatrixTools.h"

void Matrix_initMatrix(tdMatrix mat)
{
	int iLoop=0,iLoop2=0;

	/*Initialisation de la matrice*/
		for(iLoop=0;iLoop<4;++iLoop)
		{
			for(iLoop2=0;iLoop2<4;++iLoop2)
				mat[iLoop][iLoop2]= 0;
		}
}

void Matrix_initIdentityMatrix(tdMatrix mat)
{
	int iLoop=0,iLoop2=0;

	/*Initialisation de la matrice*/
		for(iLoop=0;iLoop<4;++iLoop)
		{
			for(iLoop2=0;iLoop2<4;++iLoop2)
			{
				if(iLoop==iLoop2)
					mat[iLoop][iLoop2]= 1;
				else
					mat[iLoop][iLoop2]= 0;
			}

		}
}

tdMatrix* Matrix_multiMatricesAlloc(tdMatrix mat1, tdMatrix mat2)
{
	tdMatrix* pResMat = NULL;
	int i=0,j=0,k=0;

	/*Allocation nouvelle matrice*/
	if( (pResMat = (tdMatrix*)malloc(sizeof(tdMatrix)*1) ) != NULL )
	{
		/* Let's multiply !*/
		for(i=0;i<4;++i)
		{
			for(j=0;j<4;++j)
			{
				for(k=0;k<4;++k)
					(*pResMat)[i][j]= (*pResMat)[i][j] + mat1[i][k]*mat2[k][j];
			}
		}
	}
	else
	{
		printf("Erreur allocation matrice lors multiplication");
	}

	return pResMat;
}

void Matrix_multiMatrices(tdMatrix mat1, tdMatrix mat2)
{
	tdMatrix tdResMat;
	int i,j,k;

	Matrix_initMatrix(tdResMat); /*initialisation de la matrice résultat */

	for(i=0;i<4;++i)
	{
		for(j=0;j<4;++j)
		{
			for(k=0;k<4;++k)
				tdResMat[i][j]= tdResMat[i][j] + mat1[i][k]*mat2[k][j];
		}
	}

	/* Enregistrement du résultat dans la matrice 1 */
	for(i=0;i<4;++i)
	{
		for(j=0;j<4;++j)
			mat1[i][j] = tdResMat[i][j];

	}
}

void Matrix_multiMatrixVect(tdMatrix pMatrix, double* pVect, tCoord tdRes )
{
	int i=0,j=0;

	for(i=0;i<4;++i)
	{
		tdRes[i] = 0;
		for(j=0;j<4;j++)
			tdRes[i] = tdRes[i] + (pMatrix[i][j]*pVect[j]);
	}
}

void Matrix_printMatrix(tdMatrix mat)
{
	int iLine=0,iCol=0;

	for(iLine=0;iLine<4;++iLine)
	{
		for(iCol=0;iCol<4;++iCol)
		{
			printf("%f ",mat[iLine][iCol]);
		}
		printf("\n");
	}
}
