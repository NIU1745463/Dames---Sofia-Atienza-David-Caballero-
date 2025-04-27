#ifndef TAULER_H
#define TAULER_H

#include "posicio.hpp"
#include "fitxa.h"
#include "moviment.h"
#include <string>
#include <fstream>

using namespace std;

const int MAX_MOVIMENTS = 1000;


class Tauler {
public:
    Tauler();
    void inicialitza(const string& nomFitxer);
    void actualitzaMovimentsValids();
    void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const;
    bool mouFitxa(const Posicio& origen, const Posicio& desti);
    //CREAR PARA BUFAR O IMPLEMENTAR EN BUFAR bool maximaCaptura(const Posicio& origen) const;
    //hacefalta?void aplicarMoviment(const Moviment& moviment);

    void bufarFitxa(ColorFitxa color);
    string toString() const;
private:
    Fitxa m_tauler[N_FILES][N_COLUMNES];
    void netejaTauler();
    void llegeixTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);
    void escriuTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);
    //crear esta en la funcion del tauler void generarMovimentsFitxaNormal(const Posicio& pos, Moviment& mov) const;
    // lo mismo aquí void generarMovimentsDama(const Posicio& pos, Moviment& mov) const;
    bool dinsTauler(const Posicio& pos) const;
    bool casellaBuida(const Posicio& pos) const;
    bool fitxaContraria(const Posicio& pos, ColorFitxa color) const;
    void buscarCaptures(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const;
    string m_movimentsValids[100]; //supongo que lo tendremos que hacer dinamico
    int m_numMoviments;

};
#endif