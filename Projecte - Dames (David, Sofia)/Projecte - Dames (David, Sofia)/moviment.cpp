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

bool Moviment::mouFitxa(Tauler& tauler, const Posicio& origen, const Posicio& desti)
{

    if (!tauler.dinsTauler(origen) || !tauler.dinsTauler(desti)) return false;
    if (origen == desti)
        return false;

    Fitxa fitxaMovida = tauler.m_tauler[origen.getFila()][origen.getColumna()];

    if (fitxaMovida.getTipus() == TIPUS_EMPTY) return false;

    // Verifiquem si el moviment (tenint en compte l'origen i el desti) és vàlid 
    Posicio movimentsValids[MAX_MOVIMENTS];
    int numMoviments = 0;
    tauler.getPosicionsPossibles(origen, numMoviments, movimentsValids);

    bool movimentValid = false;
    for (int i = 0; i < numMoviments; ++i)
    {
        if (movimentsValids[i] == desti)
        {
            movimentValid = true;
            break;
        }
    }
    if (!movimentValid) return false;

    // Procesar capturas y mover ficha
    int deltaFila = desti.getFila() - origen.getFila();
    int deltaCol = desti.getColumna() - origen.getColumna();
    int df = (deltaFila > 0) ? 1 : -1;
    int dc = (deltaCol > 0) ? 1 : -1;

    bool esCaptura = false;
    int numCaptures = 0;

    if (abs(deltaFila) > 1 || abs(deltaCol) > 1)
    {
        esCaptura = true;
        if (fitxaMovida.getTipus() == TIPUS_NORMAL)
        {
            int fila = origen.getFila() + df;
            int col = origen.getColumna() + dc;
            tauler.m_tauler[fila][col] = Fitxa();
            numCaptures = 1;
        }
        else {
            int fila = origen.getFila() + df;
            int col = origen.getColumna() + dc;
            while (fila != desti.getFila() || col != desti.getColumna())
            {
                if (tauler.m_tauler[fila][col].getTipus() != TIPUS_EMPTY &&
                    tauler.m_tauler[fila][col].getColor() != fitxaMovida.getColor())
                {
                    tauler.m_tauler[fila][col] = Fitxa();
                    numCaptures++;
                }
                fila += df;
                col += dc;
            }
        }
    }

    // Mover la ficha
    tauler.m_tauler[desti.getFila()][desti.getColumna()] = fitxaMovida;
    tauler.m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();

    // Comprobar coronación
    if (fitxaMovida.getTipus() == TIPUS_NORMAL)
    {
        if ((fitxaMovida.getColor() == COLOR_BLANC && desti.getFila() == 0) ||
            (fitxaMovida.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1))
        {
            tauler.m_tauler[desti.getFila()][desti.getColumna()] =
                Fitxa(fitxaMovida.getColor(), TIPUS_DAMA, fitxaMovida.getJugador());
        }
    }

    // Comprobar bufada
    bool calBufar = false;
    int maxCapturesPossibles = 0;

    if (esCaptura) {
        Posicio possibles[MAX_MOVIMENTS];
        int nPos = 0;
        tauler.buscarCaptures(origen, nPos, possibles);
        for (int i = 0; i < nPos; ++i) {
            int captures = tauler.calculaNumCaptures(origen, possibles[i]);
            if (captures > maxCapturesPossibles)
                maxCapturesPossibles = captures;
        }
        if (numCaptures < maxCapturesPossibles) calBufar = true;
    }
    else
    {
        Posicio possibles[MAX_MOVIMENTS];
        int nPos = 0;
        tauler.buscarCaptures(origen, nPos, possibles);
        if (nPos > 0) calBufar = true;
    }

    if (calBufar) tauler.bufarFitxa(fitxaMovida.getColor());

    // Actualizar movimientos válidos
    tauler.actualitzaMovimentsValids();
    return true;
}
