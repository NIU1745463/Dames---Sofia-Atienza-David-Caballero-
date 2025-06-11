#include "tauler.hpp"
#include "moviment.h"
#include "info_joc.hpp"
#include "GraphicManager.h"

// Funcio per visualitzar el tauler 
void Tauler::visualitza() const
{
    // Generacio del tauler
    GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);

    for (int i = 0; i < NUM_FILES_TAULER; ++i)
    {
        for (int j = 0; j < NUM_COLS_TAULER; ++j)
        {
            if (m_tauler[i][j].getTipus() != TIPUS_EMPTY) // si hi ha una fitxa en la posicio s'inicialitza la fitxa
            {
                int posX = POS_X_TAULER + CASELLA_INICIAL_X + (j * AMPLADA_CASELLA);
                int posY = POS_Y_TAULER + CASELLA_INICIAL_Y + (i * ALCADA_CASELLA);
                m_tauler[i][j].visualitza(posX, posY); //inicialitzacio de la fitxa
            }
        }
    }
}

// Funcio que retorna una fitxa d'una posicio determinada en el tauler
// Potser aquí ha l'error de la captura multiple de la dama
const Fitxa& Tauler::getFitxa(const Posicio& pos) const
{
    return m_tauler[pos.getFila()][pos.getColumna()];
}

// Funcio que neteja el tauler amb un for i que marca cada casella como a empty
void Tauler::netejaTauler()
{
    for (int i = 0; i < N_FILES; i++)
    {
        for (int j = 0; j < N_COLUMNES; j++)
        {
            m_tauler[i][j] = Fitxa(COLOR_BLANC, TIPUS_EMPTY, 0);
        }
    }
}

// el constructor del tauler es basicament la neteja del tauler
Tauler::Tauler()
{
    netejaTauler();
}

// funcio que llegeix d'un fitxer un tauler, en aquest cas ens serveix per inicialitzar el tauler d'un document
void Tauler::llegeixTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES])
{
    ifstream fitxer(nomFitxer);
    if (fitxer.is_open())
    {
        // primer inicialitzem un tauler completament buit amb les dimensions que busquem
        for (int i = 0; i < N_FILES; ++i)
            for (int j = 0; j < N_COLUMNES; ++j)
                tauler[i][j] = ' ';

        char valor;
        string pos;

        while (fitxer >> valor >> pos) //mentre hi hagi parametres per passar continura
        {
            int col = pos[0] - 'a';              // 'a' → 0, 'b' → 1, etc.
            int fila = 8 - (pos[1] - '0');        // '1' → 7, '8' → 0

            if (fila >= 0 && fila < N_FILES && col >= 0 && col < N_COLUMNES)
                tauler[fila][col] = valor; // incialitzem el tauler en funcio de la posicio i els valors que dona el .txt
        }
        fitxer.close();
    }
}

// funcio que escriu en fitxer el tauler
void Tauler::escriuTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES])
{
    ofstream fitxer;
    fitxer.open(nomFitxer);

    if (fitxer.is_open())
    {
        for (int i = 0; i < N_FILES; i++)
        {
            for (int j = 0; j < N_COLUMNES; j++)
            {
                if (tauler[i][j] != ' ') // si el valor del tauler no es buit
                {
                    char colChar = 'a' + j;
                    char filaChar = '0' + (8 - i);
                    fitxer << tauler[i][j] << " " << colChar << filaChar << "\n"; // escriu el valor de la fitxa, la columna y la fila
                }
            }
        }
        fitxer.close();
    }

}

// funcio que actualitza els moviments valids provinent de moviment (la hem possat aquí per estalviar del tauler i fer la funcio a moviment)
// al principi tota aquesta funcio sencera estaba a tauler, pero no acababa de tenir sentit possar-la aquí
void Tauler::actualitzaMovimentsValids()
{
    Moviment mov;
    mov.actualitzaMovimentsValids(*this); 
}

void Tauler::inicialitza(const string& nomFitxer)
{
    Moviment mov;
    netejaTauler();
    char taulerTemp[N_FILES][N_COLUMNES];
    llegeixTauler(nomFitxer, taulerTemp); // Es declara un tauler buit temporal per omplir-lo

    for (int i = 0; i < N_FILES; i++)
    {
        for (int j = 0; j < N_COLUMNES; j++)
        {
            switch (taulerTemp[i][j]) // El tauler temporal es pasa a m_tauler
            {
            case 'O': m_tauler[i][j] = Fitxa(COLOR_BLANC, TIPUS_NORMAL, 1);
                break;
            case 'X': m_tauler[i][j] = Fitxa(COLOR_NEGRE, TIPUS_NORMAL, 2);
                break;
            case 'D': m_tauler[i][j] = Fitxa(COLOR_BLANC, TIPUS_DAMA, 1);
                break;
            case 'R': m_tauler[i][j] = Fitxa(COLOR_NEGRE, TIPUS_DAMA, 2);
                break;
            default: m_tauler[i][j] = Fitxa();
                break;
            }
        }
    }
    mov.actualitzaMovimentsValids(*this);
    // Necesitem que es puguin veure els moviments vàlids de m_tauler en funcio d'aquest no tauler
    // En el nostre cas, la incialitzacio es fa amb la funcio de llegirtauler, pero perque es el mateix que el tauler inciial
    // Si tinguessim un altre tipus de tauler la incialització de llegirtauler seria molt més util
}



bool Tauler::dinsTauler(const Posicio& pos) const
{
    return pos.getFila() >= 0 && pos.getFila() < N_FILES &&
        pos.getColumna() >= 0 && pos.getColumna() < N_COLUMNES;
    // Aplico directament la condicio logica perque retorni o true o false
    // En el cas de complir les condicions dinsTauler = true
}

bool Tauler::casellaBuida(const Posicio& pos) const
{
    return dinsTauler(pos) && m_tauler[pos.getFila()][pos.getColumna()].getTipus() == TIPUS_EMPTY;
    // Mirem el tipus de casella en una posicio determinada, si es vuida retorna true
}

bool Tauler::fitxaContraria(const Posicio& pos, ColorFitxa color) const
{
    return dinsTauler(pos) &&
        m_tauler[pos.getFila()][pos.getColumna()].getTipus() != TIPUS_EMPTY &&
        m_tauler[pos.getFila()][pos.getColumna()].getColor() != color;
} // Si no esta ni buit y es del color contrari al meu, evidentment hi ha una fitxa contraria


void Tauler::buscarCaptures(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const 
{
    nPosicions = 0;
    const Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()]; // Es declara una fitxa que prove de l'origen que es dona a la funcio

    if (fitxa.getTipus() == TIPUS_EMPTY) // Si la casella es vuida es retorna la funcio
        return;

    const int MAX_DIR = 4; // el maxim de direccions que poden pendre les fitxes son 4
    struct Direccio { int df; int dc; };
    Direccio direccions[MAX_DIR] = { {-1,-1}, {-1,1}, {1,-1}, {1,1} }; // aquí tenim el tipus de moviments

    if (fitxa.getTipus() == TIPUS_NORMAL)
    {
        // Determina cap a on va la fitxa depenent del color (cap a abaix, o cap a dalt)
        int direccioForward;
        if (fitxa.getColor() == COLOR_BLANC)
        {
            direccioForward = -1; 
        }
        else
        {
            direccioForward = 1; 
        }

        // Només es revisen les direccions forward per les fitxes normals
        for (int i = 0; i < MAX_DIR; ++i)
        {
            if ((direccioForward == -1 && direccions[i].df > 0) || // Si la fitxa es mou cap amunt i la direcció apunta cap avall
                (direccioForward == 1 && direccions[i].df < 0))  // Si la fitxa es mou cap avall i la direcció apunta cap amunt
            {
                continue; // S'omet aquesta direcció
            }

            // Calcula la posicio del mig (la posició on hauria d'haver una fitxa contraria)
            Posicio mig(origen.getFila() + direccions[i].df, origen.getColumna() + direccions[i].dc);
            // Calcula la posici0 final on es pot moure la fitxa
            Posicio dest(origen.getFila() + 2 * direccions[i].df, origen.getColumna() + 2 * direccions[i].dc);

            // Comprova si la posicio del mig i la i la final estan dins del tauler
            // I si la posicio final esta buida i la posicio del mig conte una fitxa de l'altre color
            if (dinsTauler(mig) && dinsTauler(dest) && casellaBuida(dest) && fitxaContraria(mig, fitxa.getColor())) 
            {
                posicionsPossibles[nPosicions++] = dest; // Afegir la posició final com a possible moviment
            }
        }
    }
    else 
    {
        // Pel tipis dama (else) no hi ha cap mena de restriccio en el moviment
        for (int i = 0; i < MAX_DIR; ++i) 
        {
            Posicio actual = origen;
            bool haTrobatFitxa = false; // es marca que no ha trobat fitxa contraria per incialitzar
            Posicio posFitxaContraria;

            while (true) 
            {
                actual = Posicio(actual.getFila() + direccions[i].df, actual.getColumna() + direccions[i].dc); // conte totes les direccions on pot anar
                if (dinsTauler(actual) == false)
                    break; // si es surt del tauler no es pot

                if (haTrobatFitxa == false) // si no troba la fitxa buscarem
                {
                    if (fitxaContraria(actual, fitxa.getColor())) // si en una de les iteracions del for troba que hi ha una fitxa contraria es marcara
                    {
                        haTrobatFitxa = true;
                        posFitxaContraria = actual; // aquesta fitxa contraria sera marcada amb la posicio d'aquell bucle for
                    }
                    else 
                        if (casellaBuida(actual) == false) 
                    {
                        break;
                    }
                }
                else 
                {
                    if (casellaBuida(actual) == true) // si la casella es buida, es podra moure pero no hi haura on capturar
                    {
                        posicionsPossibles[nPosicions++] = actual; // aquest moviment es guarda en posicions possibles i augmenta el valor de nposicions
                    }
                    else 
                    {
                        break;
                    }
                }
            }

            if (haTrobatFitxa) // en el cas d'haver trobat la fitxa contraria 
            {
                actual = posFitxaContraria;
                while (true) 
                {
                    actual = Posicio(actual.getFila() + direccions[i].df, actual.getColumna() + direccions[i].dc);
                    // es marca desde on es pot anar desde la fitxa contraria per marcar on acabara la validesa del moviment i on acaba la dama
                    if (dinsTauler(actual) == false)
                        break;

                    if (casellaBuida(actual)) 
                    {
                        posicionsPossibles[nPosicions++] = actual;
                    }
                    else 
                    {
                        break;
                    }
                }
            }
            // com podem veure el bucle segueix efectuant-se, ja que podrien haver mes fitxes per capturar
        }
    }
}

// funcio semblant a la de buscarCaptures
void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const
{
    nPosicions = 0;
    if (dinsTauler(origen) == false) // Si la posicio d'origen es fora del tauler no es pot fer
        return;

    const Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()]; // Es declara una fitxa que prove de l'origen que es dona a la funcio

    if (fitxa.getTipus() == TIPUS_EMPTY) // Si la casella es vuida es retorna la funcio
        return;

    const int MAX_DIR = 4; // el maxim de direccions que poden pendre les fitxes son 4
    struct Direccio { int df; int dc; };
    Direccio direccions[MAX_DIR] = { {-1,-1}, {-1,1}, {1,-1}, {1,1} }; // aquests son els tipus de moviments

    struct MovimentCaptura // fem un struct am la posicio de desti i de fitxa capturada
    {
        Posicio desti;
        Posicio fitxaCapturada;
    };
    MovimentCaptura captures[MAX_MOVIMENTS];

    int numCaptures = 0;

    if (fitxa.getTipus() == TIPUS_NORMAL)
    {
        int direccioForward;
        if (fitxa.getColor() == COLOR_BLANC)
        {
            direccioForward = -1; // Les fitxes blanques es mouen cap amunt
        }
        else
        {
            direccioForward = 1; // Les fitxes negres es mouen cap avall
        }

        for (int i = 0; i < MAX_DIR; ++i)
        {
            // Determina cap a on va la fitxa depenent del color (cap a abaix, o cap a dalt)
            if ((direccioForward == -1 && direccions[i].df > 0) ||
                (direccioForward == 1 && direccions[i].df < 0)) {
                continue;
            }

            // Donara la posicio del mig (on estara la fitxa a captura)
            Posicio mig(origen.getFila() + direccions[i].df, origen.getColumna() + direccions[i].dc);
            // I la posicio desti
            Posicio dest(origen.getFila() + 2 * direccions[i].df, origen.getColumna() + 2 * direccions[i].dc);

            // Es comprovara si la fitxa contraria es a dins, si el desti es buit, i si el desti i el mig son dins del tauler
            if (dinsTauler(mig) && dinsTauler(dest) && casellaBuida(dest) &&
                fitxaContraria(mig, fitxa.getColor()))
            {
                captures[numCaptures++] = { dest, mig }; // es guardara en captures el desti i el mig DIFERENCIA AMB BUSCAR CAPTURES 
                // BUSCAR CAPTURES NOMÉS GUARDAVA EL DESTI
            }
        }
    }
    else // En el cas de les dames
    {
        for (int i = 0; i < MAX_DIR; ++i)
        {
            Posicio actual = origen;
            bool haTrobatFitxa = false;
            Posicio posFitxaContraria;

            while (true)
            {
                actual = Posicio(actual.getFila() + direccions[i].df, actual.getColumna() + direccions[i].dc);
                // la posicio actual contemplara amb el for cap a on pot anar la dama
                if (dinsTauler(actual) == false) // si esta fora del tauler no podra ser
                    break;

                if (haTrobatFitxa == false) // si no s'ha trobat una fitxa
                {
                    if (fitxaContraria(actual, fitxa.getColor())) // es busca que en l'actual d'aquest for hi hagi una fitxa
                    {
                        haTrobatFitxa = true;
                        posFitxaContraria = actual;
                    }
                    else
                        if (casellaBuida(actual) == false)
                        {
                            break;
                        }
                }
                else // si s'ha trobat quedara marcada la captura, amb la posicio actual
                {
                    if (casellaBuida(actual))
                    {
                        captures[numCaptures++] = { actual, posFitxaContraria };
                    }
                    else
                    {
                        break;
                    }
                }
            }

            if (haTrobatFitxa) // en el cas d'haver trobat la fitxa la posicio actual sera la de la fitxa contraria
            {
                actual = posFitxaContraria;
                while (true)
                {
                    // Explorem en la mateixa direccio per veure si hi ha mes posicionson la dama pot anar
                    actual = Posicio(actual.getFila() + direccions[i].df, actual.getColumna() + direccions[i].dc);

                    if (dinsTauler(actual) == false) // Si la nova posicio esta fora del tauler s'atura el bucle
                        break;

                    if (casellaBuida(actual)) // Si la nova posicio esta buida afegim la posicio com a possible captura
                    {
                        captures[numCaptures++] = { actual, posFitxaContraria };
                    }
                    else // Si la nova posicio no esta buida s'atura el bucl
                    {
                        break;
                    }
                }
            }
        }
    }

    if (numCaptures > 0) // Si el número de captures és major que 0, s'hauran de fer
    {
        for (int i = 0; i < numCaptures && nPosicions < MAX_MOVIMENTS; ++i)
        {
            posicionsPossibles[nPosicions++] = captures[i].desti; // Les posicions possibles són el destí de les possibles captures

            Tauler taulerTemp = *this; // Es crea un tauler temporal i s'inicialitza
            taulerTemp.m_tauler[origen.getFila()][origen.getColumna()] = Fitxa(); // Es borra la fitxa original del tauler temporal
            taulerTemp.m_tauler[captures[i].fitxaCapturada.getFila()][captures[i].fitxaCapturada.getColumna()] = Fitxa(); // Es borra la fitxa capturada del tauler temporal
            taulerTemp.m_tauler[captures[i].desti.getFila()][captures[i].desti.getColumna()] = fitxa; // Es col·loca la fitxa original al desti de la captura al tauler temporal

            Posicio novesCaptures[MAX_MOVIMENTS];
            int numNovesCaptures = 0;
            taulerTemp.buscarCaptures(captures[i].desti, numNovesCaptures, novesCaptures); // Busca noves captures desde el desti de la captura
            // Es aixo que buscarCaptures i getPossicionsPossibles son simialrs, pero es que son complementaries per parts com aquestes

            for (int j = 0; j < numNovesCaptures && nPosicions < MAX_MOVIMENTS; ++j)
            {
                bool repetida = false;
                for (int k = 0; k < nPosicions; ++k)
                {
                    if (posicionsPossibles[k] == novesCaptures[j]) // Comprova si la nova captura ja esta en les posicions possibles
                    {
                        repetida = true;
                        break;
                    }
                }
                if (repetida == false)
                {
                    posicionsPossibles[nPosicions++] = novesCaptures[j]; // Afegir la nova captura si no esta repetida
                }
            }
        }
    }

    if (fitxa.getTipus() == TIPUS_NORMAL)
    {
        int direccio;
        if (fitxa.getColor() == COLOR_BLANC)
        {
            direccio = -1; // Les fitxes blanques es mouen cap amunt
        }
        else
        {
            direccio = 1; // Les fitxes negres es mouen cap avall
        }

        // Comprova les dues possibles posicions on es pot moure una fitxa normal
        Posicio dest1(origen.getFila() + direccio, origen.getColumna() - 1);
        if (dinsTauler(dest1) && casellaBuida(dest1)) // Si la posició esta dins del tauler i esta buida 
           //es pot Afegir la posició com a possible moviment
        {
            posicionsPossibles[nPosicions++] = dest1; 
        }

        Posicio dest2(origen.getFila() + direccio, origen.getColumna() + 1);
        if (dinsTauler(dest2) && casellaBuida(dest2)) // el mateix que adalt
        {
            posicionsPossibles[nPosicions++] = dest2; 
        }
    }
    else
    {
        // Per el tipus dama (else) no hi ha cap mena de restriccio en el moviment
        for (int i = 0; i < MAX_DIR; ++i)
        {
            Posicio actual = origen;
            while (true)
            {
                actual = Posicio(actual.getFila() + direccions[i].df, actual.getColumna() + direccions[i].dc); // Actualitza la posicio en la iteracio actual
                if (dinsTauler(actual) == false) 
                    break;

                if (casellaBuida(actual))
                {
                    posicionsPossibles[nPosicions++] = actual; // Si la posicia esta buida afegirem la posicio com a possible moviment
                }
                else
                {
                    if (fitxaContraria(actual, fitxa.getColor())) // Si la posició conte una fitxa contraria
                    {
                        // Es calcula la posicio del desti després de capturar
                        Posicio dest = Posicio(actual.getFila() + direccions[i].df, actual.getColumna() + direccions[i].dc); 

                        if (dinsTauler(dest) && casellaBuida(dest)) // Si la posicio desti esta dins del tauler i esta buida
                        {
                            // Afegirem la posicio desti com a possible moviment
                            posicionsPossibles[nPosicions++] = dest; 
                            actual = dest; // Actualitzem per poder buscar mes captures
                        }
                        else
                        {
                            break; 
                        }
                    }
                    else
                    {
                        break; // S'atura el bucle si la posicio conte una fitxa del mateix color
                    }
                }
            }
        }
    }
}

// funcio auxiliar que serveix per poner gran part de mou fitxa a moviment.cpp i separar millor el codi
bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti) 
{
    Moviment mov;
    bool movimentRealitzat = mov.mouFitxa(*this, origen, desti); //utilitzem la funcio mouFitxa de moviment

    if (movimentRealitzat)  // Si s'ha realitzat el moviment amb els paràmetres que hem donat a mouFitxa
    {
        int numCaptures = calculaNumCaptures(origen, desti);
        if (numCaptures > 0)
        {
            // Elimina les fitxes capturades
            int df = (desti.getFila() - origen.getFila()) / abs(desti.getFila() - origen.getFila());
            int dc = (desti.getColumna() - origen.getColumna()) / abs(desti.getColumna() - origen.getColumna());

            int fila = origen.getFila() + df;
            int col = origen.getColumna() + dc;
            while (fila != desti.getFila() || col != desti.getColumna())
            {
                if (m_tauler[fila][col].getTipus() != TIPUS_EMPTY)
                {
                    m_tauler[fila][col] = Fitxa(); // Eliminar fitxa capturada
                }
                fila += df;
                col += dc;
            }
        }

        // Mostra el tauler actualitzat en la consola, era molt util per solucionar errors al principi
        std::cout << "\nTablero actualizado:\n" << toString() << std::endl;
        visualitza();
    }

    return movimentRealitzat;
}

int Tauler::calculaNumCaptures(const Posicio& origen, const Posicio& desti) const
{
    int captures = 0; //inicialitzem el contador de fitxes capturades
    int dFila = desti.getFila() - origen.getFila();
    int dColumna = desti.getColumna() - origen.getColumna(); // les dues calculen la diferencia entre la pos inicial i la final

    int df; //per a determinar el moviment de la fila
    if (dFila > 0) //fitxa es mou cap avall
    {
        df = 1;
    }
    else //fitxa es mou cap amunt
    {
        df = -1;
    }

    int dc; //per a determinar el moviment de la columna
    if (dColumna > 0) // fitxa es mou cap a la dreta
    {
        dc = 1;
    }
    else //fitxa es mou cap a l'esquerra
    {
        dc = -1;
    }

    int fila = origen.getFila() + df;
    int col = origen.getColumna() + dc;
    while (fila != desti.getFila() || col != desti.getColumna()) // s'avança fins a arribar a destí
    {
        if (m_tauler[fila][col].getTipus() != TIPUS_EMPTY) //si es troba una fitxa, es captura (i s'rementen les captures)
        {
            captures++;
        }
        fila += df;
        col += dc;
    }
    return captures;
}

// Aquesta funcio no ens molestem a comentarla ja que no ens ha sortit de cap manera
void Tauler::bufarFitxa(ColorFitxa color)
{
    Posicio candidates[MAX_MOVIMENTS];
    int nCandidates = 0;

    for (int fila = 0; fila < N_FILES; ++fila)
    {
        for (int col = 0; col < N_COLUMNES; ++col)
        {
            const Fitxa& fitxa = m_tauler[fila][col];
            if (fitxa.getColor() == color && fitxa.getTipus() != TIPUS_EMPTY)
            {
                Posicio origen(fila, col);
                Posicio possibles[MAX_MOVIMENTS];
                int nPos = 0;

                buscarCaptures(origen, nPos, possibles);

                if (nPos > 0)
                {
                    candidates[nCandidates++] = origen;
                }
            }
        }
    }

    for (int i = 0; i < nCandidates; ++i)
    {
        int fila = candidates[i].getFila();
        int col = candidates[i].getColumna();
        m_tauler[fila][col] = Fitxa();
    }

    actualitzaMovimentsValids();
}

// Aquesta funcio determina si un color no te mes opcions de moviment i en aquest cas dona la victoria a l'altre
bool Tauler::comprovarFiPartida(ColorFitxa color) 
{
    for (int fila = 0; fila < N_FILES; ++fila) 
    {
        for (int col = 0; col < N_COLUMNES; ++col) 
        {
            const Fitxa& fitxa = m_tauler[fila][col];
            if (fitxa.getColor() == color && fitxa.getTipus() != TIPUS_EMPTY) 
            {
                Posicio origen(fila, col);
                Posicio possibles[MAX_MOVIMENTS]; // en estátic, ja que el programa ens donaba un overflow
                int nPos = 0;

                getPosicionsPossibles(origen, nPos, possibles); // donara el nPos

                if (nPos > 0) 
                {
                    return false; // Si hi ha al menys un movimient possible el joc continua
                }
            }
        }
    }

    // Si no hi ha movimients disponibles l'altre guanya
    return true;
}

string Tauler::toString() const //per a convertir el estat actual del tauler a la representacio visual (fitxes del tauler)
{
    string out;
    for (int i = 0; i < N_FILES; ++i)
    {
        for (int j = 0; j < N_COLUMNES; ++j)
        {
            const Fitxa& f = m_tauler[i][j];
            string c = "_";
            if (f.getTipus() == TIPUS_NORMAL)
            {
                if (f.getColor() == COLOR_BLANC)
                {
                    c = "O";
                }
                else
                {
                    c = "X";
                }
            }
            else if (f.getTipus() == TIPUS_DAMA)
            {
                if (f.getColor() == COLOR_BLANC)
                {
                    c = "D";
                }
                else
                {
                    c = "R";
                }
            }
            out += c + " ";
        }
        out += "\n";
    }
    return out;
}
