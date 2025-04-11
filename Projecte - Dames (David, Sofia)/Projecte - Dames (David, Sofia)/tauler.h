#ifndef TAULER_H
#define TAULER_H

#include "posicio.h"
#include "fitxa.h"
#include "moviment.h"
#include <string>
#include <fstream>

using namespace std;

const int MAX_MOVIMENTS = 1000;
const int N_FILES = 8;
const int N_COLUMNES = 8;

class Tauler {
public:
    Tauler();
    void inicialitza(const string& nomFitxer);
    void actualitzaMovimentsValids();
    void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const;
    bool mouFitxa(const Posicio& origen, const Posicio& desti);
    string toString() const;
    bool movimentValid(const Posicio& origen, const Posicio& desti) const;
    bool maximaCaptura(const Posicio& origen) const;
    void aplicarMoviment(const Moviment& moviment);
    void convertirADama(const Posicio& pos);// NO CREEC QUE FACI FALTA, JA QUE EL PODEM IMPLEMENTAR DIRECTAMENT
    void bufarFitxa();  //
private:
    Fitxa m_tauler[N_FILES][N_COLUMNES];
    void netejaTauler();
    void llegeixTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);
    void escriuTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);
    void generarMovimentsFitxaNormal(const Posicio& pos, Moviment& mov) const;
    void generarMovimentsDama(const Posicio& pos, Moviment& mov) const;
    bool dinsTauler(const Posicio& pos) const;
    bool casellaBuida(const Posicio& pos) const;
    bool fitxaContraria(const Posicio& pos, ColorFitxa color) const;
    void buscarCaptures(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const;

};
#endif