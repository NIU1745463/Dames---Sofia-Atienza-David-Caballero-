#include "tauler.hpp"
#include "moviment.h"

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

Tauler::Tauler()
{
    netejaTauler();
}

void Tauler::llegeixTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES])
{
    ifstream fitxer(nomFitxer);
    if (fitxer.is_open())
    {
        for (int i = 0; i < N_FILES; ++i)
            for (int j = 0; j < N_COLUMNES; ++j)
                tauler[i][j] = ' ';

        char valor;
        string pos;

        while (fitxer >> valor >> pos)
        {
            int col = pos[0] - 'a';              // 'a' → 0, 'b' → 1, etc.
            int fila = 8 - (pos[1] - '0');        // '1' → 7, '8' → 0

            if (fila >= 0 && fila < N_FILES && col >= 0 && col < N_COLUMNES)
                tauler[fila][col] = valor;
        }
        fitxer.close();
    }
}

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
                if (tauler[i][j] != ' ')
                {
                    char colChar = 'a' + j;
                    char filaChar = '0' + (8 - i);
                    fitxer << tauler[i][j] << " " << colChar << filaChar << "\n";
                }
            }
        }
        fitxer.close();
    }

}

void Tauler::actualitzaMovimentsValids()
{
    Moviment mov;
    mov.actualitzaMovimentsValids(*this); // Ens veiem obligats a utilitzar memoria dinamica
}

void Tauler::inicialitza(const string& nomFitxer)
{
    Moviment mov;
    netejaTauler();
    char taulerTemp[N_FILES][N_COLUMNES];
    llegeixTauler(nomFitxer, taulerTemp); // Es declara un tauler buit temporal per omplir-lp

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
    // Necesitem que es puguin veure els moviments vàlids de m_tauler, em utilitzat memoria dinamica per simplificar-ho 
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
    const Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()]; //Declarem una fitxa la cual extreurem la seva posicio en el tauler

    if (fitxa.getTipus() == TIPUS_EMPTY)
        return; // Evidentment, si no hi ha fitxa, no hem de continuar

    const int MAX_DIR = 4;
    struct Direccio { int df; int dc; };
    Direccio direccions[MAX_DIR] = { {-1,-1}, {-1,1}, {1,-1}, {1,1} }; // Son els 4 moviments que es poden realitzar

    if (fitxa.getTipus() == TIPUS_NORMAL) // Realitzem els moviments per les fitxes normals, despres farem el mateix per les dames
    {
        for (int i = 0; i < MAX_DIR; ++i)
        {
            Posicio mig(origen.getFila() + direccions[i].df, origen.getColumna() + direccions[i].dc);
            Posicio dest(origen.getFila() + 2 * direccions[i].df, origen.getColumna() + 2 * direccions[i].dc);
            // Obtenim les posicions del mig y la del final (al fer una captura es mouen dues caselles, per aixo es diferencien)

            if (dinsTauler(mig) && dinsTauler(dest) && casellaBuida(dest) && fitxaContraria(mig, fitxa.getColor()))
            {
                posicionsPossibles[nPosicions++] = dest;
            }
        }
    }
    else //Aquest else es pel TIPUS_DAMA
    {
        for (int i = 0; i < MAX_DIR; ++i)
        {
            Posicio actual = origen;
            bool haTrobatFitxa = false;
            Posicio posFitxaContraria; // Ara busquem el que seria la primera fitxa contraria 

            while (true)
            {
                actual = Posicio(actual.getFila() + direccions[i].df, actual.getColumna() + direccions[i].dc);

                if (dinsTauler(actual) == false) // Evidentment si no esta dins no te sentit continuar
                    break;

                if (haTrobatFitxa == false)
                {
                    if (fitxaContraria(actual, fitxa.getColor()))
                    {
                        haTrobatFitxa = true;
                        posFitxaContraria = actual;
                    }
                    else if (casellaBuida(actual) == false)
                    {
                        break; // Una fitxa propia esta bloquejant
                    }
                }
                else
                {
                    if (casellaBuida(actual))
                    {
                        posicionsPossibles[nPosicions++] = actual; // Afegim les posicions després de la captura
                    }
                    else
                    {
                        break; // Una segona gitxa bloqueando
                    }
                }
            }

            // En cas de trobar una fitxa contraria, s'han de valorar mes moviments
            if (haTrobatFitxa)
            {
                actual = posFitxaContraria;
                while (true)
                {
                    actual = Posicio(actual.getFila() + direccions[i].df, actual.getColumna() + direccions[i].dc);
                    if (dinsTauler(actual) == false)
                        break;

                    if (casellaBuida(actual))
                    {
                        posicionsPossibles[nPosicions++] = actual;
                    }
                    else
                    {
                        break; // Aixo signficia que hi ha una altra fitxa
                    }
                }
            }
        }
    }
}

void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const
{
    nPosicions = 0;
    if (dinsTauler(origen) == false)
        return;

    const Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()]; //Declarem una fitxa la cual extreurem la seva posicio en el tauler

    if (fitxa.getTipus() == TIPUS_EMPTY) // Aquesta part te la mateixa explicacio que buscarcaptures
        return;

    const int MAX_DIR = 4;
    struct Direccio { int df; int dc; };
    Direccio direccions[MAX_DIR] = { {-1,-1}, {-1,1}, {1,-1}, {1,1} };

    struct MovimentCaptura
    {
        Posicio desti;
        Posicio fitxaCapturada;
    };
    MovimentCaptura captures[MAX_MOVIMENTS];  // Fem un array per guardar captures, basat en l'struct

    int numCaptures = 0;

    if (fitxa.getTipus() == TIPUS_NORMAL) // Primer busquem les captures possibles
    {
        // Per normals
        for (int i = 0; i < MAX_DIR; ++i)
        {
            Posicio mig(origen.getFila() + direccions[i].df, origen.getColumna() + direccions[i].dc);
            Posicio dest(origen.getFila() + 2 * direccions[i].df, origen.getColumna() + 2 * direccions[i].dc);

            if (dinsTauler(mig) && dinsTauler(dest) && casellaBuida(dest) &&
                fitxaContraria(mig, fitxa.getColor())) // Si enmig hi ha una fitxa contraria es podra fer la captura
            {
                captures[numCaptures++] = { dest, mig }; // Guardem la captura a l'array fet anteriorment
            }
        }
    }
    else
    {
        // Per dames // Tot aixo funciona igual que en buscarcaptures
        for (int i = 0; i < MAX_DIR; ++i)
        {
            Posicio actual = origen;
            bool haTrobatFitxa = false;
            Posicio posFitxaContraria;

            while (true)
            {
                actual = Posicio(actual.getFila() + direccions[i].df, actual.getColumna() + direccions[i].dc);
                if (dinsTauler(actual) == false)
                    break;

                if (!haTrobatFitxa)
                {
                    if (fitxaContraria(actual, fitxa.getColor()))
                    {
                        haTrobatFitxa = true;
                        posFitxaContraria = actual;
                    }
                    else if (casellaBuida(actual) == false)
                    {
                        break;
                    }
                }
                else
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


            if (haTrobatFitxa)
            {
                actual = posFitxaContraria;
                while (true)
                {
                    actual = Posicio(actual.getFila() + direccions[i].df, actual.getColumna() + direccions[i].dc);
                    if (dinsTauler(actual) == false)
                        break;

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
        }
    }

    // Aqui es on canvia la funcio, ja que haurem de procesar les captures amb buscarCaptures
    if (numCaptures > 0)
    {
        for (int i = 0; i < numCaptures && nPosicions < MAX_MOVIMENTS; ++i)
        {
            posicionsPossibles[nPosicions++] = captures[i].desti;

            // Simulem el moviment per realitzar captures multiples, ens hem vist obligat a utilitzar memoria dinamica
            Tauler taulerTemp = *this;
            taulerTemp.m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
            taulerTemp.m_tauler[captures[i].fitxaCapturada.getFila()][captures[i].fitxaCapturada.getColumna()] = Fitxa();
            taulerTemp.m_tauler[captures[i].desti.getFila()][captures[i].desti.getColumna()] = fitxa;

            Posicio novesCaptures[MAX_MOVIMENTS];
            int numNovesCaptures = 0;
            taulerTemp.buscarCaptures(captures[i].desti, numNovesCaptures, novesCaptures);

            // S'anira recorrent la matriu per a comprovar si es poden realitzar mes captures
            for (int j = 0; j < numNovesCaptures && nPosicions < MAX_MOVIMENTS; ++j)
            {
                bool repetida = false;
                for (int k = 0; k < nPosicions; ++k)
                {
                    if (posicionsPossibles[k] == novesCaptures[j])
                    {
                        repetida = true;
                        break;
                    }
                }
                if (repetida == false)
                {
                    posicionsPossibles[nPosicions++] = novesCaptures[j];
                }
            }
        }
    }

    
    if (fitxa.getTipus() == TIPUS_NORMAL) // Despres de comprovar, es busquen els moviments simples (sense captura)
    {
        // Per a fitxes normals
        int direccio;
        if (fitxa.getColor() == COLOR_BLANC)
        {
            direccio = -1;  // Es negatiu ja que les fitxes blanques es mouen de manera descendent
        }
        else
        {
            direccio = 1;   // Es positiu ja que les fitxes negres es mouen de manera ascendent
        }

        // Es comprova diagonal esquerre
        Posicio dest1(origen.getFila() + direccio, origen.getColumna() - 1);
        if (dinsTauler(dest1) && casellaBuida(dest1))
        {
            posicionsPossibles[nPosicions++] = dest1;
        }

        // Es comprova diagonal dreta
        Posicio dest2(origen.getFila() + direccio, origen.getColumna() + 1);
        if (dinsTauler(dest2) && casellaBuida(dest2))
        {
            posicionsPossibles[nPosicions++] = dest2;
        }
    }
    else
    {
        // Si no son fitxes normals, es comprova per a les dames (totes les direccions)
        for (int i = 0; i < MAX_DIR; ++i)
        {
            Posicio actual = origen;
            while (true)
            {
                actual = Posicio(actual.getFila() + direccions[i].df,
                    actual.getColumna() + direccions[i].dc);
                if (dinsTauler(actual) == false)
                    break;

                if (casellaBuida(actual))
                {
                    posicionsPossibles[nPosicions++] = actual; //en el cas de que la casella es troba buida s'afeigeix la pos a llista de moviments possibles
                }
                else
                {
                    if (fitxaContraria(actual, fitxa.getColor()))
                    {
                        // En el cas de haver una fitxa contraria, es calcula la posicio seguent per calcularla
                        Posicio dest = Posicio(actual.getFila() + direccions[i].df, actual.getColumna() + direccions[i].dc);
                        if (dinsTauler(dest) && casellaBuida(dest))
                        {
                            posicionsPossibles[nPosicions++] = dest;
                            actual = dest; //si la casella es troba buida es fa la captura
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
}
bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
    Moviment mov;
    return mov.mouFitxa(*this, origen, desti); //ens veiem obligats a utilitzar memoria dinamica
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
        if (m_tauler[fila][col].getTipus() != TIPUS_EMPTY) //si es troba una fitxa, es captura (i s'incrementen les captures)
        {
            captures++;
        }
        fila += df;
        col += dc;
    }
    return captures;
}

void Tauler::bufarFitxa(ColorFitxa color)
{
    Posicio candidates[MAX_MOVIMENTS]; // array on es guardaran les pos de les fitxes les quals seran bufades
    int nCandidates = 0;

    for (int fila = 0; fila < N_FILES; ++fila)
    {
        for (int col = 0; col < N_COLUMNES; ++col)
        {
            const Fitxa& fitxa = m_tauler[fila][col];
            //donat un color en específic
            if (fitxa.getColor() == color && fitxa.getTipus() != TIPUS_EMPTY)
            {
                Posicio origen(fila, col);
                Posicio possibles[MAX_MOVIMENTS];
                int nPos = 0;

                //es busca si te captures pendents
                buscarCaptures(origen, nPos, possibles);

                if (nPos > 0) {
                    candidates[nCandidates++] = origen; //en el cas de que hi hagin, s'afegeix a l'array candidates
                }
            }
        }
    }

    // un cop trobada, es borra la fitxa 
    if (nCandidates > 0)
    {
        int fila = candidates[0].getFila();
        int col = candidates[0].getColumna();
        m_tauler[fila][col] = Fitxa(); 
    }

    actualitzaMovimentsValids();
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
