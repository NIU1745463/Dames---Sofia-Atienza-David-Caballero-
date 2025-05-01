#include "moviment.h"
#include "tauler.hpp"

Moviment::Moviment()
{
	m_fitxesMatades = 0;
	m_nCaselles = 0;
}

void Moviment::actualitzaMovimentsValids(Tauler& tauler) {
    tauler.m_numMoviments = 0;
    for (int fila = 0; fila < N_FILES; ++fila) {
        for (int col = 0; col < N_COLUMNES; ++col) {
            Posicio pos(fila, col);
            Fitxa& fitxa = tauler.m_tauler[fila][col];
            if (fitxa.getTipus() != TIPUS_EMPTY) {
                Posicio possibles[MAX_MOVIMENTS];
                int nPos = 0;
                tauler.getPosicionsPossibles(pos, nPos, possibles);
                for (int i = 0; i < nPos && tauler.m_numMoviments < MAX_MOVIMENTS; ++i) {
                    tauler.m_movimentsValids[tauler.m_numMoviments++] =
                        pos.toString() + "->" + possibles[i].toString();
                }
            }
        }
    }
}

