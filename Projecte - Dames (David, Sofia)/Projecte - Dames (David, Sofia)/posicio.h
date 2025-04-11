#ifndef POSICIO_H
#define POSICIO_H

#include <iostream>

using namespace std;

const int N_FILES = 8;
const int N_COLUMNES = 8;

class Posicio
{
public:
	Posicio();
	Posicio(int fila, int columna);
	int getFila() const { return m_fila; }
	int getColumna() const { return m_columna; }
	void setFila(int fila) { m_fila = fila; }
	void setColumna(int columna) { m_columna = columna; }
	bool PosicioValida() const { return m_fila >= 0 && m_columna >= 0; }
	string toString() const;
	void fromString(const string& pos);
	Posicio(const string& posicio);
	bool operator==(const Posicio& posicio) const;
	// FALTA FER POSICIONS POSSIBLES
	// SOBRECARREGA FETA A CLASSE
private:
	int m_fila;
	int m_columna;
};

#endif