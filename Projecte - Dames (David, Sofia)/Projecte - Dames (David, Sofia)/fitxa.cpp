#include "fitxa.h"

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

bool Fitxa::convertirDama()
{
	if (m_tipusFitxa != TIPUS_DAMA && finalTauler() == true)
	{
		m_tipusFitxa = TIPUS_DAMA;
		return true;
	}
	return false;
	// SI NO ES DAMA PASARA A SER DAMA, I SI HO NO ES FA CAP CANVI
}
