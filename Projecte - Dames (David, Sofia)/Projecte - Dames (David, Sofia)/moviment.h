#ifndef MOVIMENT_H
#define MOVIMENT_H

#include "fitxa.h"
#include "posicio.h"
#include "tauler.h"

const int MAX_SALTS = 8;  //Afegeixo aquesta condicio, ja que es impossible que faci mes salts que aixo
const int MAX_BLANQUES = 12;
const int MAX_NEGRES = 12;

class Moviment 
{
public:
    Moviment();
    int getFitxesMatades() const { return m_fitxesMatades; }
    int getNCaselles() const { return m_nCaselles; }
    Fitxa getFitxa() const { return m_fitxa; }
    void getPosicioInicial(int& fila, int& columna) const;
    void getPosicioFinal(int& fila, int& columna) const;
    void getSalt(int index, int& fila, int& columna) const;
    void addSalt(int fila, int columna);
    void addFitxaMatada(); // es poden juntar
    void setPosicioFinal(int fila, int columna);
    bool movimentValid() const;
    bool capturaFeta() const { return m_fitxesMatades > 0; }
    bool MovimentMultiple() const { return m_fitxesMatades > 1; }
private:
    Fitxa m_fitxa; 
    // INICI
    Posicio m_inici;
    // FINAL
    Posicio m_final;
    Posicio m_salts[MAX_SALTS];;
    // ALTRES
    int m_nCaselles;
    int m_fitxesMatades;
};

#endif