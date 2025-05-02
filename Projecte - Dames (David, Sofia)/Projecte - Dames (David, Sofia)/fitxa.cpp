#include "fitxa.h"

// Constructors 

Fitxa::Fitxa()
{
	m_colorFitxa = COLOR_BLANC;
	m_tipusFitxa = TIPUS_EMPTY;
	m_nJugador = 0;
}

Fitxa::Fitxa(ColorFitxa color, TipusFitxa tipus, int njugador)
{
	m_colorFitxa = color;
	m_tipusFitxa = tipus;
	m_nJugador = njugador;
}



