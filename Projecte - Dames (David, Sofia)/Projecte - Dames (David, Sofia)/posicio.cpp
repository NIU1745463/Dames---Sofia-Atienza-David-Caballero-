#include "posicio.h"

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

void Posicio::fromString(const string& pos)
{
    m_fila = (N_FILES - 1) - (pos[1] - '1');
    m_columna = pos[0] - 'a';
}

Posicio::Posicio(const string& posicio) 
{
    fromString(posicio);
}

bool Posicio::operator==(const Posicio& posicio) const 
{
    return (m_fila == posicio.m_fila) && (m_columna == posicio.m_columna);
}