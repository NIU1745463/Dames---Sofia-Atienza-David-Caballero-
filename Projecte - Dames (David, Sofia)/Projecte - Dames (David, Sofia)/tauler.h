#ifndef TAULER_H
#define TAULER_H

#include <iostream>

using namespace std;

class Tauler
{
public:
void inicialitza(const string& nomFitxer);
void actualitzaMovimentsValids();
void getPosicionsPossibles(cont Posicio& origen, int& nPosicions, 
	Posicio posicionsPossibles[]); bool mouFitxa
	(const Posicio& origen, const Posicio& desti);
string toString() const
private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];

};

#endif