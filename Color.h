#ifndef COLOR_H
#define COLOR_H


typedef double tColor[4];

/**
 * Setter d'une structure sColor.
 *
 * @param pColor Pointeur vers la structure sColor à modifier
 * @param r Composante Rouge
 * @param g Composante Verte
 * @param b Composante Bleue
 */
void Color_setColor(tColor* ptColor,double r, double g, double b, double a);


#endif
