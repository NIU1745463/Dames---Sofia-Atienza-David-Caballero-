#include "posicio.hpp"


Posicio::Posicio()
{
	m_fila = 0;
	m_columna = 0;
}

Posicio::Posicio(int fila, int columna)
{
	m_fila = fila;
	m_columna = columna;
}


string Posicio::toString() const
{
	string pos = "a1";
	pos[0] = 'a' + m_columna;
	pos[1] = '1' + (N_FILES - 1) - m_fila;
	return pos;
}

Posicio::Posicio(const string& posicio) 
{
	fromString(posicio);
}

void Posicio::fromString(const string& pos)
{
	m_fila = (N_FILES - 1) - (pos[1] - '1');
	m_columna = pos[0] - 'a';
}


void llegeixFitxa(const string& nomFitxer, char& tipusFitxa, Posicio& posicio)
{
	ifstream fitxer(nomFitxer);
	fitxer >> tipusFitxa;
	fitxer >> posicio;
	fitxer.close();
}

void escriuFitxa(const string& nomFitxer, char tipusFitxa, const Posicio& posicio)
{
	ofstream fitxer(nomFitxer);
	fitxer << tipusFitxa << ' ';
	fitxer << posicio;
	fitxer.close();
}

bool Posicio::operator==(const Posicio& posicio) const 
{
    return (m_fila == posicio.m_fila) && (m_columna == posicio.m_columna);
}

ifstream& operator>>(ifstream& fitxer, Posicio& posicio) {
	string pos;
	fitxer >> pos;
	posicio.fromString(pos);
	return fitxer;
}

ostream& operator<<(ostream& os, const Posicio& pos) {
	os << pos.toString();
	return os;
}

