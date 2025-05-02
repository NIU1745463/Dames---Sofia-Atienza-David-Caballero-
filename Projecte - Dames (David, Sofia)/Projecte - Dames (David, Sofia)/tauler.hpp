#ifndef TAULER_H
#define TAULER_H

#include "posicio.hpp"
#include "fitxa.h"
#include "moviment.h"
#include <string>
#include <fstream>

using namespace std;

const int MAX_MOVIMENTS = 1000;

class Moviment;

class Tauler {
public: 
    Tauler();
    friend class Moviment; 
    void inicialitza(const string& nomFitxer);
    void actualitzaMovimentsValids();
    void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const;
    bool mouFitxa(const Posicio& origen, const Posicio& desti);
    void bufarFitxa(ColorFitxa color);
    string toString() const;

private:
    Fitxa m_tauler[N_FILES][N_COLUMNES];
    void netejaTauler();
    void llegeixTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);
    void escriuTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);
    bool dinsTauler(const Posicio& pos) const;
    bool casellaBuida(const Posicio& pos) const;
    bool fitxaContraria(const Posicio& pos, ColorFitxa color) const;
    void buscarCaptures(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const;
    string m_movimentsValids[100]; //més endavant serà dinamic
    int m_numMoviments;
    int calculaNumCaptures(const Posicio& origen, const Posicio& desti) const;


};
#endif
