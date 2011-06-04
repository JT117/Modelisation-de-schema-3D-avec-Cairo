#ifndef MATRIXTOOLS_H
#define MATRIXTOOLS_H

#include "Point.h"

typedef double tdMatrix[4][4];

/**
 * Initialisation la matrice.
 */
void Matrix_initMatrix(tdMatrix mat);

/**
 * Multiplie (si c'est possible) deux matrices entre elles et retourne une nouvelle matrice allouée.
 * @param mat1 La première matrice...
 * @param mat2 et la seconde...
 *
 * @return La matrice résultat
 */
tdMatrix* Matrix_multiMatricesAlloc(tdMatrix mat1, tdMatrix mat2);


/**
 * Multiplie (si c'est possible) deux matrices entre elles et met le résultat dans la première matrice.
 *
 * @param mat1 La première matrice...
 * @param mat2 et la seconde...
 */
void Matrix_multiMatrices(tdMatrix mat1, tdMatrix mat2);

/**
 * Multiplie une matrice carrée (tout le temps matrice homogene dans notre cas) avec un vecteur.
 *
 * @param pMat la matrice
 * @param pVect Le vecteur
 * @param pRes Le resultat
 * \pre Taille matrice carrée == taille vecteur
 */
void Matrix_multiMatrixVect(tdMatrix pMat, double* pVect, double* pRes );

/**
 * Temporaire : affichage matrice dans la console
 */
void Matrix_printMatrix(tdMatrix mat);

/**
 * Initialise la matrice passée en paramètre en tant que matrice identité.
 *
 * @param mat Matrice concernée par le traitement
 */
void Matrix_initIdentityMatrix(tdMatrix mat);

/**
 * Copie le contenu de la matrice 2 dans la matrice 1.
 *
 * @param tdMat1 matrice dans laquelle les données seront copiées
 * @param tdMat2 matrice à copier
 */
void Matrix_copy(tdMatrix tdMat1, tdMatrix tdMat2);

#endif
