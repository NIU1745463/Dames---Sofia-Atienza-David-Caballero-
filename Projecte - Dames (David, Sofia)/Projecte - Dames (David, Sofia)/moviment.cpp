#include "moviment.h"
#include "tauler.hpp"

Moviment::Moviment()
{
    m_fitxesMatades = 0;
    m_nCaselles = 0;
}

void Moviment::actualitzaMovimentsValids(Tauler& tauler) 
{
    tauler.m_numMoviments = 0;
    for (int fila = 0; fila < N_FILES; ++fila) 
    {
        for (int col = 0; col < N_COLUMNES; ++col) 
        {
            Posicio pos(fila, col); //representa la posicio actual
            Fitxa& fitxa = tauler.m_tauler[fila][col];
            if (fitxa.getTipus() != TIPUS_EMPTY) 
            { 
                // si es troba una fitxa, es calculen les posicions possibles 
                Posicio possibles[MAX_MOVIMENTS];
                int nPos = 0;
                tauler.getPosicionsPossibles(pos, nPos, possibles);
                for (int i = 0; i < nPos && tauler.m_numMoviments < MAX_MOVIMENTS; ++i) 
                {
                    // sobre totes les posicions possibles, si el num de moviments vàlids es menor que el max, es registren al array
                    tauler.m_movimentsValids[tauler.m_numMoviments++] =
                        pos.toString() + "->" + possibles[i].toString();
                }
            }
        }
    }
}

bool Moviment::mouFitxa(Tauler& tauler, const Posicio& origen, const Posicio& desti)
{
    // es verifica si el moviment es valid (origen i destí)
    if (tauler.dinsTauler(origen) == false || tauler.dinsTauler(desti) == false)
        return false;
    if (origen == desti)
        return false;

    Fitxa fitxaMovida = tauler.m_tauler[origen.getFila()][origen.getColumna()]; // obtenim la fitxa que volem moure
    if (fitxaMovida.getTipus() == TIPUS_EMPTY) return false;

    // es verifica si el moviment es valid
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

    // es determina en quina direccio es desplaça la fitxa (diagonal amunt o avall, esquerre o dreta)
    int dFila = desti.getFila() - origen.getFila();
    int dColumna = desti.getColumna() - origen.getColumna();
    int df;
    if (dFila > 0) df = 1;
    else df = -1;

    int dc;
    if (dColumna > 0) dc = 1;
    else dc = -1;

    bool esCaptura = false;
    int numCaptures = 0;

    // cal saber si el moviment es tracta d'una captura o no
    if (abs(dFila) > 1 || abs(dColumna) > 1)
    {
        esCaptura = true;
        if (fitxaMovida.getTipus() == TIPUS_NORMAL)
        {
            int fila = origen.getFila() + df;
            int col = origen.getColumna() + dc;
            tauler.m_tauler[fila][col] = Fitxa();
            numCaptures = 1;
        }
        else
        {
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
    // es mou la fitxa a la nova pos i deixa buida l'antiga
    tauler.m_tauler[desti.getFila()][desti.getColumna()] = fitxaMovida;
    tauler.m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();

    // si la fitxa es de tipus normal i arriba al final del tauler, es "promociona" a dama
    if (fitxaMovida.getTipus() == TIPUS_NORMAL)
    {
        if ((fitxaMovida.getColor() == COLOR_BLANC && desti.getFila() == 0) ||
            (fitxaMovida.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1))
        {
            tauler.m_tauler[desti.getFila()][desti.getColumna()] =
                Fitxa(fitxaMovida.getColor(), TIPUS_DAMA, fitxaMovida.getJugador());
        }
    }

    // es comprova si cal bufar la fitxa
    bool calBufar = false;
    int maxCapturesPossibles = 0;

    if (esCaptura)
    {
        Posicio possibles[MAX_MOVIMENTS];
        int nPos = 0;
        tauler.buscarCaptures(origen, nPos, possibles);
        for (int i = 0; i < nPos; ++i)
        {
            int captures = tauler.calculaNumCaptures(origen, possibles[i]);
            if (captures > maxCapturesPossibles)
                maxCapturesPossibles = captures;
        }
        if (numCaptures < maxCapturesPossibles)
            calBufar = true; // si es poden fer mes captures, cal bufar 
    }
    else
    {
        Posicio possibles[MAX_MOVIMENTS];
        int nPos = 0;
        tauler.buscarCaptures(origen, nPos, possibles);
        if (nPos > 0)
            calBufar = true;
    }

    // en el cas de que calgui bufar, cridem al mètode corresponent
    if (calBufar)
        tauler.bufarFitxa(fitxaMovida.getColor());
    tauler.actualitzaMovimentsValids();
    return true;
}

