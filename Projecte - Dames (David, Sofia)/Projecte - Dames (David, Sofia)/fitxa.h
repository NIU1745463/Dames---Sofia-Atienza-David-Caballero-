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

	// CONSTRUCTORS
	Fitxa();
	Fitxa(ColorFitxa color, TipusFitxa tipus, int njugador);

	// FUNCIONS QUE INDIQUEN EL COLOR I TIPUS DE LA FITXA
	ColorFitxa getColor() const { return m_colorFitxa; };
	TipusFitxa getTipus() const { return m_tipusFitxa; };
	int getJugador() const { return m_nJugador; };

	string getMoviments(const int index) const { return moviments[index]; };
	// FUNCIONS QUE DONEN EL COLOR I TIPUS DE LA FITXA
	void setColor(ColorFitxa color) { m_colorFitxa = color; };
	void setTipus(TipusFitxa tipus) { m_tipusFitxa = tipus; };

private:
	ColorFitxa m_colorFitxa;
	TipusFitxa m_tipusFitxa;
	int m_nJugador;
	string moviments[100]; // En un futur haurà de ser modificat i farem servir memoria dinàmica
};

#endif