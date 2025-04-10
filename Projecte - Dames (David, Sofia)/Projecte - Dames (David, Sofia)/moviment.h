#ifndef MOVIMENT_H
#define MOVIMENT_H

#include "fitxa.h"

const int MAX_SALTS = 8;  //Afegeixo aquesta condicio, ja que es impossible que faci mes salts que aixo
const int MAX_BLANQUES = 12;
const int MAX_NEGRES = 12;

class Moviment 
{
public:
    Moviment();
    Moviment(Fitxa fitxa, int filaInici, int columnaInici);
    int getFitxesMatades() const { return m_fitxesMatades; }
    int getNCaselles() const { return m_nCaselles; }
    Fitxa getFitxa() const { return m_fitxa; }
    void getPosicioInicial(int& fila, int& columna) const;
    void getPosicioFinal(int& fila, int& columna) const;
    void getSalt(int index, int& fila, int& columna) const;
    void addSalt(int fila, int columna);
    void addFitxaMatada();
    void setPosicioFinal(int fila, int columna);
    bool movimentValid() const;
    bool capturaFeta() const { return m_fitxesMatades > 0; }
    bool MovimentMultiple() const { return m_fitxesMatades > 1; }
private:
    Fitxa m_fitxa; 
    // INICI
    int m_filaInici;
    int m_columnaInici;
    // FINAL
    int m_filaFinal;
    int m_columnaFinal; 
    // ALTRES
    int m_saltsFila[MAX_SALTS]; 
    int m_saltsColumna[MAX_SALTS]; 
    int m_fitxesMatades; 
    int m_nCaselles; 
};

#endif