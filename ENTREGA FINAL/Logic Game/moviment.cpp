#include "moviment.h"
#include "tauler.hpp"


// Constructor por defecto
Moviment::Moviment()
{
    m_fitxesMatades = 0;
}

// Destructor std::vector sencarrega de la gestio de memoria
Moviment::~Moviment() {}

// Funcio per a assignar espaci dinamic a m_salts
void Moviment::setSalts(int size)
{
    m_salts.resize(size); // Redimensionem el vector
}

// Actualiza els moviments valids en el tauler
void Moviment::actualitzaMovimentsValids(Tauler& tauler)
{
    tauler.m_numMoviments = 0;
    for (int fila = 0; fila < N_FILES; ++fila)
    {
        for (int col = 0; col < N_COLUMNES; ++col)
        {
            Posicio pos(fila, col); //determina una posicio 
            Fitxa& fitxa = tauler.m_tauler[fila][col]; //extreu la fitxa del tauler en una casella determinada per fila i columna

            if (fitxa.getTipus() != TIPUS_EMPTY) // si la casella no es buida
            {
                std::vector<Posicio> possibles(MAX_MOVIMENTS);
                int nPos = 0;
                tauler.getPosicionsPossibles(pos, nPos, possibles.data()); // obte les posicions posibles amb la funcio de tauler

                for (int i = 0; i < nPos && tauler.m_numMoviments < MAX_MOVIMENTS; ++i)
                {
                    tauler.m_movimentsValids[tauler.m_numMoviments++] =
                        pos.toString() + "->" + possibles[i].toString(); // dona la posicio actual i les possibles amb el bucle for
                }
            }
        }
    }
}

// Movimient de fitxa en el tauler
bool Moviment::mouFitxa(Tauler& tauler, const Posicio& origen, const Posicio& desti)
{
    // si esta fora del tauler retorna un false
    if (!tauler.dinsTauler(origen) || !tauler.dinsTauler(desti) || origen == desti)
        return false;

    Fitxa fitxaMovida = tauler.m_tauler[origen.getFila()][origen.getColumna()];

    // si no hi ha fitxa retorna false
    if (fitxaMovida.getTipus() == TIPUS_EMPTY)
        return false;

    std::vector<Posicio> movimentsValids(MAX_MOVIMENTS); // si no feiem aquesta part dinamic teniem un overflow de la memoria
    int numMoviments = 0;

    // per fer el moviment volem les posicions possibles en funcio de l'origen que se'ns dona
    tauler.getPosicionsPossibles(origen, numMoviments, movimentsValids.data()); 

    bool movimentValid = false;
    for (int i = 0; i < numMoviments; ++i)
    {
        if (movimentsValids[i] == desti) // si el desti es equivalent a un dels moviments valids significa que es pot fer
        {
            movimentValid = true;
            break;
        }
    }

    if (!movimentValid) // si no es així aleshores no podrem moure
        return false;

    // Calcul de la diferencia de posicio entre l'inici i el final, tant de la fila com la columna
    int dFila = desti.getFila() - origen.getFila();
    int dColumna = desti.getColumna() - origen.getColumna();

    bool esCaptura = false;
    int numCaptures = 0;

    // auqesta diferencia ha de ser mes gran de 1 per haver fet qualsevol moviment
    if (abs(dFila) > 1 || abs(dColumna) > 1)
    {
        esCaptura = true;
        int filaCapturada = origen.getFila() + (dFila / 2); // es divideix entre dos, ja que volem el mig
        int colCapturada = origen.getColumna() + (dColumna / 2);

        // COM ESTAVEM TENINT PROBLEMES VAM FER AIXÒ A LA DEPURACIO DEL PROGRAMA
        // ES IRRELEVANT EN EL FUNCIONAMENT 
        // Verificacio abans deliminar

        std::cout << "Intentando eliminar ficha en " << filaCapturada << ", " << colCapturada
            << " (Tipo antes: " << tauler.m_tauler[filaCapturada][colCapturada].getTipus() << ")" << std::endl;

        // aixo es relevamnt, es la fitxa eliminada
        tauler.m_tauler[filaCapturada][colCapturada] = Fitxa(); // Eliminación de ficha, passa a ser buida

        // Verificacio despres deliminar (només serveix per la consola i poder fer les comprovacions)

        std::cout << "Ficha eliminada en " << filaCapturada << ", " << colCapturada
            << " (Tipo después: " << tauler.m_tauler[filaCapturada][colCapturada].getTipus() << ")" << std::endl;

        numCaptures = 1;
    }

    tauler.m_tauler[desti.getFila()][desti.getColumna()] = fitxaMovida; // moure la fitxa al punt de captura
    tauler.m_tauler[origen.getFila()][origen.getColumna()] = Fitxa(); // buidar el punt d'inici

    // Aquesta funcio es molt simple, converteix la fitcha normal en dama depenent del color que sigui
    if (fitxaMovida.getTipus() == TIPUS_NORMAL)
    {
        if ((fitxaMovida.getColor() == COLOR_BLANC && desti.getFila() == 0) ||
            (fitxaMovida.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1))
        {
            tauler.m_tauler[desti.getFila()][desti.getColumna()] =
                Fitxa(fitxaMovida.getColor(), TIPUS_DAMA, fitxaMovida.getJugador());
        }
    }

    // A PARTIR D'AQUI HAURIA DE SERVIR PER BUFAR, PERO NO ENS FUNCIONA CORRECTAMENT
    bool calBufar = false;
    int maxCapturesPossibles = 0;

    if (esCaptura)
    {
        std::vector<Posicio> possibles(MAX_MOVIMENTS);
        int nPos = 0;
        tauler.buscarCaptures(origen, nPos, possibles.data());

        for (int i = 0; i < nPos; ++i)
        {
            int captures = tauler.calculaNumCaptures(origen, possibles[i]);
            if (captures > maxCapturesPossibles)
                maxCapturesPossibles = captures;
        }

        if (numCaptures < maxCapturesPossibles)
            calBufar = true;
    }
    else
    {
        std::vector<Posicio> possibles(MAX_MOVIMENTS);
        int nPos = 0;
        tauler.buscarCaptures(origen, nPos, possibles.data());

        if (nPos > 0)
            calBufar = true;
    }

    if (calBufar)
        tauler.bufarFitxa(fitxaMovida.getColor());

    tauler.actualitzaMovimentsValids();

    return true;
}

