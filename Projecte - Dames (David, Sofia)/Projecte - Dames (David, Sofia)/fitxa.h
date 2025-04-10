#ifndef FITXA_H
#define FITXA_H

#include <iostream>

using namespace std;

typedef enum
{
	TIPUS_NORMAL,
	TIPUS_DAMA,
	TIPUS_EMPTY
} TipusFitxa;

typedef enum
{
	COLOR_NEGRE,
	COLOR_BLANC,
} ColorFitxa;

class Fitxa
{
public:
private:
	int colorFitxa;
	int tipusFitxa;
	int nJugador;
	string moviments[];
};

#endif