#ifndef MOVIMENT_H
#define MOVIMENT_H

#include "fitxa.h"
#include "posicio.hpp"

const int MAX_SALTS = 8;

class Tauler;

class Moviment
{
public:
    Moviment();
    void actualitzaMovimentsValids(Tauler& tauler);
    bool mouFitxa(Tauler& tauler, const Posicio& origen, const Posicio& desti);

private:
    Fitxa m_fitxa;
    // INICI
    Posicio m_inici;
    // FINAL
    Posicio m_final;
    Posicio m_salts[MAX_SALTS];
    // ALTRES
    int m_nCaselles;
    int m_fitxesMatades;
};

#endif
