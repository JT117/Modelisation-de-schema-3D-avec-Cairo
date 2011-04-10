#include <math.h>
#include <stdio.h>

#include "./include/Camera.h"

void Camera_setCoordCam(sInfoCamera* pCam,double dX, double dY, double dZ)
{
	pCam->CoordCam[0]=dX;
	pCam->CoordCam[1]=dY;
	pCam->CoordCam[2]=dZ;
	pCam->CoordCam[3]=1;

	/* On en profite pour modifer la matrice de passage World->repére caméra */
	Camera_setMatPassage(pCam, dX, dY, dZ);

}

void Camera_setMatPassage(sInfoCamera* pCam,double dX, double dY, double dZ)
{
	int iLoop1, iLoop2;

	for(iLoop1=0; iLoop1<4 ;iLoop1++)
	{
		for(iLoop2=0; iLoop2<4 ;iLoop2++)
		{
			if(iLoop1==iLoop2)
				pCam->tdMatPassage[iLoop1][iLoop2]=1;
			else
				pCam->tdMatPassage[iLoop1][iLoop2]=0;
		}
	}
	pCam->tdMatPassage[0][3]=-dX;
	pCam->tdMatPassage[1][3]=-dY;
	pCam->tdMatPassage[2][3]=-dZ;
}


void Camera_setFrustum(sInfoCamera* pCam, double dAngleV, double dRatio)
{
	if(pCam->dDMin != 0)
	{
		pCam->dUmax = pCam->dDMin*tan(dAngleV);
		pCam->dRmax = dRatio*pCam->dUmax;
		pCam->dUmin = -pCam->dUmax;
		pCam->dRmin = -pCam->dRmax;
	}
	else
		printf("Aucune distance du near plan renseignée !");
}

void Camera_setDistancePlan(sInfoCamera* pCam, double dDMin, double dDmax)
{
	pCam->dDMin=dDMin;
	pCam->dDMax=dDmax;
}

void Camera_projectionCalculation(sInfoCamera *pCam)
{
	/* TODO : des boucles */

    pCam->tdMatProj[0][0] = -pCam->dDMin;
    pCam->tdMatProj[1][0] = 0;
    pCam->tdMatProj[2][0] = 0;
    pCam->tdMatProj[3][0] = -1;

    pCam->tdMatProj[0][1] =  0;
    pCam->tdMatProj[1][1] =  -pCam->dDMin;
    pCam->tdMatProj[2][1] =  0;
    pCam->tdMatProj[3][1] =  0;

    pCam->tdMatProj[0][2] =  0;
    pCam->tdMatProj[1][2] =  0;
    pCam->tdMatProj[2][2] =  -pCam->dDMin;
    pCam->tdMatProj[3][2] =  0;

    pCam->tdMatProj[0][3] =  0;
    pCam->tdMatProj[1][3] =  0;
    pCam->tdMatProj[2][3] =  0;
    pCam->tdMatProj[3][3] =  0;
 }
