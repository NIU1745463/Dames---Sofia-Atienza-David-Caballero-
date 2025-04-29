#include "tauler.hpp"

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

void Tauler::inicialitza(const string& nomFitxer) {
    netejaTauler();
    char taulerTemp[N_FILES][N_COLUMNES];
    llegeixTauler(nomFitxer, taulerTemp);

    for (int i = 0; i < N_FILES; i++) {
        for (int j = 0; j < N_COLUMNES; j++) {
            switch (taulerTemp[i][j]) {
            case 'O': m_tauler[i][j] = Fitxa(COLOR_BLANC, TIPUS_NORMAL, 1); break;
            case 'X': m_tauler[i][j] = Fitxa(COLOR_NEGRE, TIPUS_NORMAL, 2); break;
            case 'D': m_tauler[i][j] = Fitxa(COLOR_BLANC, TIPUS_DAMA, 1); break;
            case 'R': m_tauler[i][j] = Fitxa(COLOR_NEGRE, TIPUS_DAMA, 2); break;
            default: m_tauler[i][j] = Fitxa(); break;
            }
        }
    }
    actualitzaMovimentsValids();
}

void Tauler::actualitzaMovimentsValids() {
    m_numMoviments = 0; // Resetear contador

    for (int fila = 0; fila < N_FILES; ++fila) {
        for (int col = 0; col < N_COLUMNES; ++col) {
            Posicio pos(fila, col);
            Fitxa& fitxa = m_tauler[fila][col];

            if (fitxa.getTipus() != TIPUS_EMPTY) {
                Posicio possibles[MAX_MOVIMENTS];
                int nPos = 0;
                getPosicionsPossibles(pos, nPos, possibles);

                for (int i = 0; i < nPos && m_numMoviments < MAX_MOVIMENTS; ++i) {
                    m_movimentsValids[m_numMoviments++] =
                        pos.toString() + "->" + possibles[i].toString();
                }
            }
        }
    }
}

bool Tauler::dinsTauler(const Posicio& pos) const
{
    return pos.getFila() >= 0 && pos.getFila() < N_FILES &&
        pos.getColumna() >= 0 && pos.getColumna() < N_COLUMNES; // Aplico directament la condicio logica perque retorni o true o false
}

bool Tauler::casellaBuida(const Posicio& pos) const
{
    return dinsTauler(pos) && m_tauler[pos.getFila()][pos.getColumna()].getTipus() == TIPUS_EMPTY;
}

bool Tauler::fitxaContraria(const Posicio& pos, ColorFitxa color) const
{
    return dinsTauler(pos) &&
        m_tauler[pos.getFila()][pos.getColumna()].getTipus() != TIPUS_EMPTY &&
        m_tauler[pos.getFila()][pos.getColumna()].getColor() != color;
} // Si no esta ni vuit y es del color contrari al meu, evidentment hi ha una fitxa contraria


void Tauler::buscarCaptures(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const
{
    const Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];
    const int MAX_DIR = 4;
    struct Direccio { int df; int dc; };
    Direccio direccions[MAX_DIR] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

    for (int i = 0; i < MAX_DIR; ++i)
    {
        int df = direccions[i].df;
        int dc = direccions[i].dc;

        if (fitxa.getTipus() == TIPUS_NORMAL)
        {
            Posicio mig(origen.getFila() + df, origen.getColumna() + dc);
            Posicio dest(origen.getFila() + 2 * df, origen.getColumna() + 2 * dc);

            if (dinsTauler(mig) && dinsTauler(dest) && casellaBuida(dest) && fitxaContraria(mig, fitxa.getColor()))
            {
                posicionsPossibles[nPosicions++] = dest;
            }
        }
        else if (fitxa.getTipus() == TIPUS_DAMA)
        {
            Posicio actual = origen;
            bool haTrobatFitxa = false;

            while (true)
            {
                actual = Posicio(actual.getFila() + df, actual.getColumna() + dc);
                if (!dinsTauler(actual)) break;

                if (!haTrobatFitxa)
                {
                    if (fitxaContraria(actual, fitxa.getColor()))
                    {
                        haTrobatFitxa = true;
                    }
                    else if (!casellaBuida(actual))
                    {
                        break;
                    }
                }
                else
                {
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
        }
    }
}

void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const {
    nPosicions = 0;
    if (!dinsTauler(origen)) return;

    const Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];
    if (fitxa.getTipus() == TIPUS_EMPTY) return;

    // 1. Primero buscar capturas obligatorias
    buscarCaptures(origen, nPosicions, posicionsPossibles);

    // Si hay capturas, no devolver movimientos simples (reglas del juego)
    if (nPosicions > 0) return;

    // 2. Si no hay capturas, buscar movimientos simples
    const int MAX_DIR = 4;
    struct Direccio { int df; int dc; };
    Direccio direccions[MAX_DIR] = { {-1,-1}, {-1,1}, {1,-1}, {1,1} };

    if (fitxa.getTipus() == TIPUS_NORMAL) {
        // Para fichas normales, solo movimientos hacia adelante
        int inicioDir = (fitxa.getColor() == COLOR_BLANC) ? 0 : 2; // Blancas: (-1,-1) y (-1,1)
        // Negras: (1,-1) y (1,1)
        for (int i = inicioDir; i < inicioDir + 2; ++i) {
            Posicio dest(origen.getFila() + direccions[i].df,
                origen.getColumna() + direccions[i].dc);
            if (dinsTauler(dest) && casellaBuida(dest)) {
                posicionsPossibles[nPosicions++] = dest;
            }
        }
    }
    else {
        // Para damas, movimientos en todas las direcciones
        for (int i = 0; i < MAX_DIR; ++i) {
            Posicio actual = origen;
            while (true) {
                actual = Posicio(actual.getFila() + direccions[i].df,
                    actual.getColumna() + direccions[i].dc);
                if (!dinsTauler(actual)) break;
                if (casellaBuida(actual)) {
                    posicionsPossibles[nPosicions++] = actual;
                }
                else {
                    break; // Hay una ficha en el camino
                }
            }
        }
    }
}

/*/
void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const {
    nPosicions = 0;
    if (!dinsTauler(origen)) return;

    const Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];
    if (fitxa.getTipus() == TIPUS_EMPTY) return;

    // 1. Primero buscar capturas obligatorias
    buscarCaptures(origen, nPosicions, posicionsPossibles);

    // Si hay capturas, no devolver movimientos simples (reglas del juego)
    if (nPosicions > 0) return;

    // 2. Si no hay capturas, buscar movimientos simples
    const int MAX_DIR = 4;
    struct Direccio { int df; int dc; };
    Direccio direccions[MAX_DIR] = { {-1,-1}, {-1,1}, {1,-1}, {1,1} };

    if (fitxa.getTipus() == TIPUS_NORMAL) {
        // Para fichas normales, solo movimientos hacia adelante
        int inicioDir = (fitxa.getColor() == COLOR_BLANC) ? 0 : 2; // Blancas: (-1,-1) y (-1,1)
        // Negras: (1,-1) y (1,1)
        for (int i = inicioDir; i < inicioDir + 2; ++i) {
            Posicio dest(origen.getFila() + direccions[i].df,
                origen.getColumna() + direccions[i].dc);
            if (dinsTauler(dest) && casellaBuida(dest)) {
                posicionsPossibles[nPosicions++] = dest;
            }
        }
    }
    else {
        // Para damas, movimientos en todas las direcciones
        for (int i = 0; i < MAX_DIR; ++i) {
            Posicio actual = origen;
            while (true) {
                actual = Posicio(actual.getFila() + direccions[i].df,
                    actual.getColumna() + direccions[i].dc);
                if (!dinsTauler(actual)) break;
                if (casellaBuida(actual)) {
                    posicionsPossibles[nPosicions++] = actual;
                }
                else {
                    break; // Hay una ficha en el camino
                }
            }
        }
    }
}
*/

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
    if (!dinsTauler(origen) || !dinsTauler(desti)) return false;
    if (origen == desti) return false;

    Fitxa fitxaMovida = m_tauler[origen.getFila()][origen.getColumna()];
    if (fitxaMovida.getTipus() == TIPUS_EMPTY) return false;

    Posicio movimentsValids[MAX_MOVIMENTS];
    int numMoviments = 0;
    getPosicionsPossibles(origen, numMoviments, movimentsValids);

    bool movimentValid = false;
    for (int i = 0; i < numMoviments; ++i)
        if (movimentsValids[i] == desti) movimentValid = true;

    if (!movimentValid) return false;

    // Eliminar fitxa capturada si es captura
    int df = (desti.getFila() - origen.getFila()) > 0 ? 1 : -1;
    int dc = (desti.getColumna() - origen.getColumna()) > 0 ? 1 : -1;
    int fila = origen.getFila() + df;
    int columna = origen.getColumna() + dc;

    while (fila != desti.getFila() && columna != desti.getColumna()) {
        Posicio inter(fila, columna);
        if (fitxaContraria(inter, fitxaMovida.getColor()))
            m_tauler[fila][columna] = Fitxa();
        fila += df;
        columna += dc;
    }

    // Moure fitxa
    m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
    m_tauler[desti.getFila()][desti.getColumna()] = fitxaMovida;

    // Convertir en dama si toca
    if (fitxaMovida.getTipus() == TIPUS_NORMAL) {
        if ((fitxaMovida.getColor() == COLOR_BLANC && desti.getFila() == 0) ||
            (fitxaMovida.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1)) {
            m_tauler[desti.getFila()][desti.getColumna()].setTipus(TIPUS_DAMA);
        }
    }

    // Buscar captures encadenades
    Posicio actual = desti;
    while (true) {
        Posicio captures[MAX_MOVIMENTS];
        int nCaptures = 0;
        buscarCaptures(actual, nCaptures, captures);
        if (nCaptures == 0) break;

        Posicio seguent = captures[0]; // automàticament primer
        df = (seguent.getFila() - actual.getFila()) > 0 ? 1 : -1;
        dc = (seguent.getColumna() - actual.getColumna()) > 0 ? 1 : -1;
        fila = actual.getFila() + df;
        columna = actual.getColumna() + dc;

        while (fila != seguent.getFila() && columna != seguent.getColumna()) {
            if (fitxaContraria(Posicio(fila, columna), fitxaMovida.getColor()))
                m_tauler[fila][columna] = Fitxa();
            fila += df;
            columna += dc;
        }

        m_tauler[actual.getFila()][actual.getColumna()] = Fitxa();
        m_tauler[seguent.getFila()][seguent.getColumna()] = fitxaMovida;
        actual = seguent;
    }

    actualitzaMovimentsValids();
    return true;
}

void Tauler::bufarFitxa(ColorFitxa color) {
    // Buscar todas las fichas del color que pueden capturar
    int maxCaptures = 0;
    Posicio posMaxCaptures;

    for (int i = 0; i < N_FILES; ++i) {
        for (int j = 0; j < N_COLUMNES; ++j) {
            if (m_tauler[i][j].getColor() == color &&
                m_tauler[i][j].getTipus() != TIPUS_EMPTY) {

                Posicio pos(i, j);
                Posicio captures[MAX_MOVIMENTS];
                int numCaptures = 0;
                buscarCaptures(pos, numCaptures, captures);

                if (numCaptures > maxCaptures) {
                    maxCaptures = numCaptures;
                    posMaxCaptures = pos;
                }
            }
        }
    }

    // Eliminar la ficha que podía hacer más capturas
    if (maxCaptures > 0) {
        m_tauler[posMaxCaptures.getFila()][posMaxCaptures.getColumna()] = Fitxa();
    }
}


string Tauler::toString() const {
    string out;
    for (int i = 0; i < N_FILES; ++i) {
        for (int j = 0; j < N_COLUMNES; ++j) {
            const Fitxa& f = m_tauler[i][j];
            string c = "_";
            if (f.getTipus() == TIPUS_NORMAL)
                c = (f.getColor() == COLOR_BLANC) ? "O" : "X";
            else if (f.getTipus() == TIPUS_DAMA)
                c = (f.getColor() == COLOR_BLANC) ? "D" : "R";
            out += c + " ";
        }
        out += "\n";
    }
    return out;
}