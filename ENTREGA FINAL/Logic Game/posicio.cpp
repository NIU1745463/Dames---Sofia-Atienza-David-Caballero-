#include "posicio.hpp"

// Constructors 
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

// Retorna la posicio que es un string transformat amb la funcio fromstring
Posicio::Posicio(const string& posicio)
{
	fromString(posicio);
}

// Pasa la posicio a un string
string Posicio::toString() const
{
	string pos = "a1";
	pos[0] = 'a' + m_columna;
	pos[1] = '1' + (N_FILES - 1) - m_fila;
	return pos;
}

// Pasa l'string a una posicio
void Posicio::fromString(const string& pos)
{
	m_fila = (N_FILES - 1) - (pos[1] - '1');
	m_columna = pos[0] - 'a';
}


// Llegeix d'un fitxer la fitxa
void llegeixFitxa(const string& nomFitxer, char& tipusFitxa, Posicio& posicio)
{
	ifstream fitxer(nomFitxer);
	fitxer >> tipusFitxa;
	fitxer >> posicio;
	fitxer.close();
}

// Llegeix en un fitxer la fitxa
void escriuFitxa(const string& nomFitxer, char tipusFitxa, const Posicio& posicio)
{
	ofstream fitxer(nomFitxer);
	fitxer << tipusFitxa << ' ';
	fitxer << posicio;
	fitxer.close();
}

// Comproba que dues posicions no siguin iguals, si ho son retorna true;
bool Posicio::operator==(const Posicio& posicio) const
{
	return (m_fila == posicio.m_fila) && (m_columna == posicio.m_columna);
}

// Aquest operator permet que d'un fitxer s'hagafi una posicio i la retorni com una posicio
ifstream& operator>>(ifstream& fitxer, Posicio& posicio) 
{
	string pos;
	fitxer >> pos;
	posicio.fromString(pos);
	return fitxer;
}


// Aquest operator permet que una posicio passi a ser string
ostream& operator<<(ostream& os, const Posicio& pos) 
{
	os << pos.toString();
	return os;
}
