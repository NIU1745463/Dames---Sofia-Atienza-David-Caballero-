#include "moviment.h"

Moviment::Moviment()
{
	m_fitxesMatades = 0;
	m_nCaselles = 0;
}

Moviment::Moviment(Fitxa fitxa, int filaInici, int columnaInici)
{
    m_fitxa = fitxa;
    m_filaInici = filaInici;
    m_columnaInici = columnaInici;
    m_fitxesMatades = 0;
    m_nCaselles = 0;
}

void Moviment::getPosicioInicial(int& fila, int& columna) const 
{
    fila = m_filaInici;
    columna = m_columnaInici;
}

void Moviment::getPosicioFinal(int& fila, int& columna) const 
{
    fila = m_filaFinal;
    columna = m_columnaFinal;
}

void Moviment::addSalt(int fila, int columna) // Cada salt que fem s'agegira aqui i incrementara el nom de caselles recorregudes
{
    if (m_nCaselles < MAX_SALTS) 
    {
        m_saltsFila[m_nCaselles] = fila;
        m_saltsColumna[m_nCaselles] = columna;
        m_nCaselles++; 
    }
}

void Moviment::getSalt(int index, int& fila, int& columna) const {
    if (index >= 0 && index < m_nCaselles) {
        fila = m_saltsFila[index];
        columna = m_saltsColumna[index];
    }
    else {
        fila = -1;
        columna = -1;
    }
}

void Moviment::addFitxaMatada() 
{
    m_fitxesMatades++;
}

void Moviment::setPosicioFinal(int fila, int columna) 
{
    m_filaFinal = fila;
    m_columnaFinal = columna;
}

bool Moviment::movimentValid() const 
{
    // FER SENCER 
}