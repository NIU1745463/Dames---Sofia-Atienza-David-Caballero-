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

void Tauler::buscarCaptures(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const {
    nPosicions = 0;
    const Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];
    if (fitxa.getTipus() == TIPUS_EMPTY) return;

    const int MAX_DIR = 4;
    struct Direccio { int df; int dc; };
    Direccio direccions[MAX_DIR] = { {-1,-1}, {-1,1}, {1,-1}, {1,1} };

    if (fitxa.getTipus() == TIPUS_NORMAL) {
        // Capturas para fichas normales
        for (int i = 0; i < MAX_DIR; ++i) {
            Posicio mig(origen.getFila() + direccions[i].df, origen.getColumna() + direccions[i].dc);
            Posicio dest(origen.getFila() + 2 * direccions[i].df, origen.getColumna() + 2 * direccions[i].dc);

            if (dinsTauler(mig) && dinsTauler(dest) &&
                casellaBuida(dest) &&
                fitxaContraria(mig, fitxa.getColor())) {
                posicionsPossibles[nPosicions++] = dest;
            }
        }
    }
    else {
        // Capturas para damas - considerar todas las posibilidades
        for (int i = 0; i < MAX_DIR; ++i) {
            Posicio actual = origen;
            bool haTrobatFitxa = false;
            Posicio posFitxaContraria;

            // Buscar primera ficha contraria en esta dirección
            while (true) {
                actual = Posicio(actual.getFila() + direccions[i].df,
                    actual.getColumna() + direccions[i].dc);
                if (!dinsTauler(actual)) break;

                if (!haTrobatFitxa) {
                    if (fitxaContraria(actual, fitxa.getColor())) {
                        haTrobatFitxa = true;
                        posFitxaContraria = actual;
                    }
                    else if (!casellaBuida(actual)) {
                        break; // Ficha propia bloqueando
                    }
                }
                else {
                    if (casellaBuida(actual)) {
                        // Añadir todas las posiciones válidas después de la captura
                        posicionsPossibles[nPosicions++] = actual;
                    }
                    else {
                        break; // Segunda ficha bloqueando
                    }
                }
            }

            // Si encontramos una ficha contraria, explorar más allá
            if (haTrobatFitxa) {
                actual = posFitxaContraria;
                while (true) {
                    actual = Posicio(actual.getFila() + direccions[i].df,
                        actual.getColumna() + direccions[i].dc);
                    if (!dinsTauler(actual)) break;

                    if (casellaBuida(actual)) {
                        posicionsPossibles[nPosicions++] = actual;
                    }
                    else {
                        break; // Otra ficha bloqueando
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

    const int MAX_DIR = 4;
    struct Direccio { int df; int dc; };
    Direccio direccions[MAX_DIR] = { {-1,-1}, {-1,1}, {1,-1}, {1,1} };

    // Array para almacenar capturas
    struct MovimentCaptura {
        Posicio desti;
        Posicio fitxaCapturada;
    };
    MovimentCaptura captures[MAX_MOVIMENTS];
    int numCaptures = 0;

    // 1. Buscar capturas posibles
    if (fitxa.getTipus() == TIPUS_NORMAL) {
        // Para fichas normales
        for (int i = 0; i < MAX_DIR; ++i) {
            Posicio mig(origen.getFila() + direccions[i].df, origen.getColumna() + direccions[i].dc);
            Posicio dest(origen.getFila() + 2 * direccions[i].df, origen.getColumna() + 2 * direccions[i].dc);

            if (dinsTauler(mig) && dinsTauler(dest) &&
                casellaBuida(dest) &&
                fitxaContraria(mig, fitxa.getColor())) {
                captures[numCaptures++] = { dest, mig };
            }
        }
    }
    else {
        // Para damas
        for (int i = 0; i < MAX_DIR; ++i) {
            Posicio actual = origen;
            bool haTrobatFitxa = false;
            Posicio posFitxaContraria;

            // Buscar primera ficha contraria en esta dirección
            while (true) {
                actual = Posicio(actual.getFila() + direccions[i].df,
                    actual.getColumna() + direccions[i].dc);
                if (!dinsTauler(actual)) break;

                if (!haTrobatFitxa) {
                    if (fitxaContraria(actual, fitxa.getColor())) {
                        haTrobatFitxa = true;
                        posFitxaContraria = actual;
                    }
                    else if (!casellaBuida(actual)) {
                        break; // Ficha propia bloqueando
                    }
                }
                else {
                    if (casellaBuida(actual)) {
                        // Añadir todas las posiciones válidas después de la captura
                        captures[numCaptures++] = { actual, posFitxaContraria };
                    }
                    else {
                        break; // Segunda ficha bloqueando
                    }
                }
            }

            // Si encontramos una ficha contraria, explorar más allá
            if (haTrobatFitxa) {
                actual = posFitxaContraria;
                while (true) {
                    actual = Posicio(actual.getFila() + direccions[i].df,
                        actual.getColumna() + direccions[i].dc);
                    if (!dinsTauler(actual)) break;

                    if (casellaBuida(actual)) {
                        captures[numCaptures++] = { actual, posFitxaContraria };
                    }
                    else {
                        break; // Otra ficha bloqueando
                    }
                }
            }
        }
    }

    // 2. Si hay capturas, procesarlas
    if (numCaptures > 0) {
        for (int i = 0; i < numCaptures && nPosicions < MAX_MOVIMENTS; ++i) {
            posicionsPossibles[nPosicions++] = captures[i].desti;

            // Simular movimiento para capturas múltiples
            Tauler taulerTemp = *this;
            taulerTemp.m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
            taulerTemp.m_tauler[captures[i].fitxaCapturada.getFila()][captures[i].fitxaCapturada.getColumna()] = Fitxa();
            taulerTemp.m_tauler[captures[i].desti.getFila()][captures[i].desti.getColumna()] = fitxa;

            Posicio novesCaptures[MAX_MOVIMENTS];
            int numNovesCaptures = 0;
            taulerTemp.buscarCaptures(captures[i].desti, numNovesCaptures, novesCaptures);

            for (int j = 0; j < numNovesCaptures && nPosicions < MAX_MOVIMENTS; ++j) {
                bool repetida = false;
                for (int k = 0; k < nPosicions; ++k) {
                    if (posicionsPossibles[k] == novesCaptures[j]) {
                        repetida = true;
                        break;
                    }
                }
                if (!repetida) {
                    posicionsPossibles[nPosicions++] = novesCaptures[j];
                }
            }
        }
    }

    // 3. Movimientos simples sin captura
    if (fitxa.getTipus() == TIPUS_NORMAL) {
        // Para fichas normales
        int direccio = (fitxa.getColor() == COLOR_BLANC) ? -1 : 1; // Blancas hacia arriba, negras hacia abajo

        // Diagonal izquierda
        Posicio dest1(origen.getFila() + direccio, origen.getColumna() - 1);
        if (dinsTauler(dest1) && casellaBuida(dest1)) {
            posicionsPossibles[nPosicions++] = dest1;
        }

        // Diagonal derecha
        Posicio dest2(origen.getFila() + direccio, origen.getColumna() + 1);
        if (dinsTauler(dest2) && casellaBuida(dest2)) {
            posicionsPossibles[nPosicions++] = dest2;
        }
    }
    else {
        // Para damas - todas las direcciones
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
                    if (fitxaContraria(actual, fitxa.getColor())) {
                        // Check for capture
                        Posicio dest = Posicio(actual.getFila() + direccions[i].df,
                            actual.getColumna() + direccions[i].dc);
                        if (dinsTauler(dest) && casellaBuida(dest)) {
                            posicionsPossibles[nPosicions++] = dest;
                            // Continue to next square after capture
                            actual = dest;
                        }
                        else {
                            break;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
        }
    }
}

/*/bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
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

    // Comprovar si és captura (moviment de més d'una casella)
    int deltaFila = desti.getFila() - origen.getFila();
    int deltaCol = desti.getColumna() - origen.getColumna();

    if (abs(deltaFila) > 1 && abs(deltaCol) > 1)
    {
        int df = (deltaFila > 0) ? 1 : -1;
        int dc = (deltaCol > 0) ? 1 : -1;
        int fila = origen.getFila() + df;
        int col = origen.getColumna() + dc;

        // Per fitxa normal, només hi ha una fitxa entre mig
        if (fitxaMovida.getTipus() == TIPUS_NORMAL)
        {
            m_tauler[fila][col] = Fitxa(); // Eliminar fitxa capturada
        }
        else // Per dama, pot capturar des de més lluny
        {
            while (fila != desti.getFila() && col != desti.getColumna())
            {
                if (m_tauler[fila][col].getTipus() != TIPUS_EMPTY)
                {
                    if (m_tauler[fila][col].getColor() != fitxaMovida.getColor())
                        m_tauler[fila][col] = Fitxa(); // Eliminar fitxa capturada
                    break; // Només pot capturar una per torn
                }
                fila += df;
                col += dc;
            }
        }
    }

    // Moure la fitxa
    m_tauler[desti.getFila()][desti.getColumna()] = fitxaMovida;
    m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();

    // Corona si arriba a l’última fila
    if (fitxaMovida.getTipus() == TIPUS_NORMAL)
    {
        if ((fitxaMovida.getColor() == COLOR_BLANC && desti.getFila() == 0) ||
            (fitxaMovida.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1))
        {
            m_tauler[desti.getFila()][desti.getColumna()] = Fitxa(fitxaMovida.getColor(), TIPUS_DAMA, fitxaMovida.getJugador());
        }
    }

    actualitzaMovimentsValids();

    // Comprovar si el moviment ha matat el màxim de fitxes possibles
    int maxCaptures = 0;
    for (int f = 0; f < N_FILES; ++f) {
        for (int c = 0; c < N_COLUMNES; ++c) {
            const Fitxa& ftx = m_tauler[f][c];
            if (ftx.getColor() == fitxaMovida.getColor() && ftx.getTipus() != TIPUS_EMPTY) {
                Posicio pos(f, c);
                Posicio destins[MAX_MOVIMENTS];
                int nDest = 0;
                buscarCaptures(pos, nDest, destins);
                if (nDest > maxCaptures)
                    maxCaptures = nDest;
            }
        }
    }

    // Si el moviment no ha matat el màxim de fitxes, bufar
    if (maxCaptures > 1) {
        bufarFitxa(fitxaMovida.getColor());
    }

    return true;
}
*/

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti) {
    if (!dinsTauler(origen) || !dinsTauler(desti)) return false;
    if (origen == desti) return false;

    Fitxa fitxaMovida = m_tauler[origen.getFila()][origen.getColumna()];
    if (fitxaMovida.getTipus() == TIPUS_EMPTY) return false;

    // Verificar si el movimiento es válido
    Posicio movimentsValids[MAX_MOVIMENTS];
    int numMoviments = 0;
    getPosicionsPossibles(origen, numMoviments, movimentsValids);

    bool movimentValid = false;
    for (int i = 0; i < numMoviments; ++i) {
        if (movimentsValids[i] == desti) {
            movimentValid = true;
            break;
        }
    }
    if (!movimentValid) return false;

    // Procesar capturas
    int deltaFila = desti.getFila() - origen.getFila();
    int deltaCol = desti.getColumna() - origen.getColumna();
    int df = (deltaFila > 0) ? 1 : -1;
    int dc = (deltaCol > 0) ? 1 : -1;

    bool esCaptura = false;
    int numCaptures = 0;

    if (abs(deltaFila) > 1 || abs(deltaCol) > 1) {
        esCaptura = true;
        if (fitxaMovida.getTipus() == TIPUS_NORMAL) {
            // Captura simple para ficha normal
            int fila = origen.getFila() + df;
            int col = origen.getColumna() + dc;
            m_tauler[fila][col] = Fitxa();
            numCaptures = 1;
        }
        else {
            // Captura múltiple para dama
            int fila = origen.getFila() + df;
            int col = origen.getColumna() + dc;
            while (fila != desti.getFila() || col != desti.getColumna()) {
                if (m_tauler[fila][col].getTipus() != TIPUS_EMPTY &&
                    m_tauler[fila][col].getColor() != fitxaMovida.getColor()) {
                    m_tauler[fila][col] = Fitxa();
                    numCaptures++;
                }
                fila += df;
                col += dc;
            }
        }
    }

    // Mover la ficha
    m_tauler[desti.getFila()][desti.getColumna()] = fitxaMovida;
    m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();

    // Comprobar si hay que coronar
    if (fitxaMovida.getTipus() == TIPUS_NORMAL) {
        if ((fitxaMovida.getColor() == COLOR_BLANC && desti.getFila() == 0) ||
            (fitxaMovida.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1)) {
            m_tauler[desti.getFila()][desti.getColumna()] =
                Fitxa(fitxaMovida.getColor(), TIPUS_DAMA, fitxaMovida.getJugador());
        }
    }

    // Comprobar si se debió capturar más fichas
    bool calBufar = false;
    int maxCapturesPossibles = 0;

    if (esCaptura) {
        // Buscar el máximo de capturas posibles desde la posición original
        Posicio possibles[MAX_MOVIMENTS];
        int nPos = 0;
        buscarCaptures(origen, nPos, possibles);

        for (int i = 0; i < nPos; ++i) {
            int captures = calculaNumCaptures(origen, possibles[i]);
            if (captures > maxCapturesPossibles) {
                maxCapturesPossibles = captures;
            }
        }

        if (numCaptures < maxCapturesPossibles) {
            calBufar = true;
        }
    }
    else {
        // Para movimientos simples, comprobar si había capturas disponibles
        Posicio possibles[MAX_MOVIMENTS];
        int nPos = 0;
        buscarCaptures(origen, nPos, possibles);
        if (nPos > 0) {
            calBufar = true;
        }
    }

    if (calBufar) {
        bufarFitxa(fitxaMovida.getColor());
    }

    actualitzaMovimentsValids();
    return true;
}

int Tauler::calculaNumCaptures(const Posicio& origen, const Posicio& desti) const {
    int captures = 0;
    int deltaFila = desti.getFila() - origen.getFila();
    int deltaCol = desti.getColumna() - origen.getColumna();
    int df = (deltaFila > 0) ? 1 : -1;
    int dc = (deltaCol > 0) ? 1 : -1;

    int fila = origen.getFila() + df;
    int col = origen.getColumna() + dc;
    while (fila != desti.getFila() || col != desti.getColumna()) {
        if (m_tauler[fila][col].getTipus() != TIPUS_EMPTY) {
            captures++;
        }
        fila += df;
        col += dc;
    }
    return captures;
}

void Tauler::bufarFitxa(ColorFitxa color) {
    // Buscar la(s) ficha(s) del color que tenían capturas pero no se han movido
    Posicio candidates[MAX_MOVIMENTS];
    int nCandidates = 0;

    for (int fila = 0; fila < N_FILES; ++fila) {
        for (int col = 0; col < N_COLUMNES; ++col) {
            const Fitxa& fitxa = m_tauler[fila][col];
            if (fitxa.getColor() == color && fitxa.getTipus() != TIPUS_EMPTY) {
                Posicio origen(fila, col);
                Posicio possibles[MAX_MOVIMENTS];
                int nPos = 0;

                buscarCaptures(origen, nPos, possibles);

                if (nPos > 0) {
                    candidates[nCandidates++] = origen;
                }
            }
        }
    }

    // Borrar la primera ficha encontrada con captura disponible
    if (nCandidates > 0) {
        int fila = candidates[0].getFila();
        int col = candidates[0].getColumna();
        m_tauler[fila][col] = Fitxa(); // Bufada
    }

    actualitzaMovimentsValids();
}

string Tauler::toString() const {
    string out;
    for (int i = 0; i < N_FILES; ++i) {
        for (int j = 0; j < N_COLUMNES; ++j) {
            const Fitxa& f = m_tauler[i][j];
            string c = "_";
            if (f.getTipus() == TIPUS_NORMAL) {
                c = (f.getColor() == COLOR_BLANC) ? "O" : "X";
            }
            else if (f.getTipus() == TIPUS_DAMA) {
                c = (f.getColor() == COLOR_BLANC) ? "D" : "R";
            }
            out += c + " ";
        }
        out += "\n";
    }
    return out;
}