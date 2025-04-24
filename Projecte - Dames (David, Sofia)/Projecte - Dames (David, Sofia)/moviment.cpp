#include "moviment.h"

Moviment::Moviment()
{
	m_fitxesMatades = 0;
	m_nCaselles = 0;
}

void Moviment::getPosicioInicial(int& fila, int& columna) const // No sé si tendrian que ser strings aquí o están bien como fila columna
{
    fila = m_inici.getFila();
    columna = m_inici.getColumna();
}

void Moviment::getPosicioFinal(int& fila, int& columna) const 
{
    fila = m_final.getFila();
    columna = m_final.getColumna();;
}

void Moviment::addSalt(int fila, int columna) {
    if (m_nCaselles < MAX_SALTS) {
        m_salts[m_nCaselles] = Posicio(fila, columna);
        m_nCaselles++;
    }
}

void Moviment::getSalt(int index, int& fila, int& columna) const {
    if (index >= 0 && index < m_nCaselles) {
        fila = m_salts[index].getFila();
        columna = m_salts[index].getColumna();
    }
    else {
        fila = -1;
        columna = -1;
    }
}

void Moviment::addFitxaMatada() // juntar
{
    m_fitxesMatades++;
}

void Moviment::setPosicioFinal(int fila, int columna) 
{
    m_final.setFila(fila);
    m_final.setColumna(columna);
}

bool Moviment::movimentValid() const
{
    Posicio possPossibles[MAX_MOVIMENTS]; 
    int nPosicions = 0;

    Tauler m_tauler;

    for (int i = 0; i < nPosicions; ++i)
    {
        if (possPossibles[i] == m_final)
        {
            return true;
        }
    }
    return false;
}


