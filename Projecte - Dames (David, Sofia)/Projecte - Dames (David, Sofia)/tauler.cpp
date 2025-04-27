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
//void Tauler::llegeixTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]) // CAMBIAR
//{
 //   ifstream fitxer;
  //  fitxer.open(nomFitxer);
  //  string linia;

   // if (fitxer.is_open())
   // {
    //    for (int i = 0; i < N_FILES; i++)
      //  {
       //     for (int j = 0; j < N_COLUMNES; j++)
      //      {
          //      tauler[i][j] = ' ';
          //  }
     //   }

      //  char valor;
      //  string pos;

    //    while (fitxer >> valor >> pos)
     //   {
    //        char colChar = pos[0];
     //       char filaChar = pos[1];
     //       int col = colChar - 'a';
     //       int fila = 8 - (filaChar - '0');

     //       if (fila >= 0 && fila < N_FILES && col >= 0 && col < N_COLUMNES)
      //      {
      ////          tauler[fila][col] = valor;
      //      }
     //   }
     //   fitxer.close();
    //}

//}

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
//void Tauler::inicialitza(const string& nomFitxer)
//{
   // netejaTauler();
    //char taulerTemp[N_FILES][N_COLUMNES];
   // llegeixTauler(nomFitxer, taulerTemp);
//
   // for (int i = 0; i < N_FILES; i++) 
   // {
      //  for (int j = 0; j < N_COLUMNES; j++) 
      //  {
            //switch (taulerTemp[i][j]) {
        //    case 'B': m_tauler[i][j] = Fitxa(COLOR_BLANC, TIPUS_NORMAL, 1); break;
         //   case 'N': m_tauler[i][j] = Fitxa(COLOR_NEGRE, TIPUS_NORMAL, 2); break;
         //   case 'D': m_tauler[i][j] = Fitxa(COLOR_BLANC, TIPUS_DAMA, 1); break;
         //   case 'Q': m_tauler[i][j] = Fitxa(COLOR_NEGRE, TIPUS_DAMA, 2); break;
          //  default: m_tauler[i][j] = Fitxa(); break;
         //   }
      //  }
 //   }
 //   actualitzaMovimentsValids();
//}


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

//void Tauler::actualitzaMovimentsValids() { //para poder registrar los movimientos validos y poder hacer bufar
    //for (int fila = 0; fila < N_FILES; ++fila) {
     //   for (int columna = 0; columna < N_COLUMNES; ++columna) {
       //     Fitxa& fitxa = m_tauler[fila][columna];
        //    if (fitxa.getTipus() != TIPUS_EMPTY) {
             //   Posicio origen(fila, columna);
              //  Posicio posicionsPossibles[MAX_MOVIMENTS];
              //  int nPosicions = 0;
               // getPosicionsPossibles(origen, nPosicions, posicionsPossibles);
              //  for (int i = 0; i < nPosicions; ++i) {
                 //   if (m_numMoviments < 100) { //cuando lo hagamos dinamico no tendremos q comprobar q sea menor a 100
                  //      m_movimentsValids[m_numMoviments++] = origen.toString() + posicionsPossibles[i].toString();
                //    }
                //    else {
                //        cout << "posicio no correcte";
                //    }
              //  }
         //   }
      //  }
 //   }
//}

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
    Direccio direccions[MAX_DIR] = { {-1,-1}, {-1,1}, {1,-1}, {1,1} };

    for (int i = 0; i < MAX_DIR; ++i)
    {
        int df = direccions[i].df;
        int dc = direccions[i].dc;
        Posicio mig(origen.getFila() + df, origen.getColumna() + dc);
        Posicio dest(origen.getFila() + 2 * df, origen.getColumna() + 2 * dc);

        if (fitxa.getTipus() == TIPUS_NORMAL)
        {
            if (dinsTauler(dest) && casellaBuida(dest) && fitxaContraria(mig, fitxa.getColor()))
            {
                posicionsPossibles[nPosicions++] = dest;
            }
        }
        else if (fitxa.getTipus() == TIPUS_DAMA)
        {
            // Para Damas: avanzar mientras siga vacía
            Posicio actual = origen;
            bool haTrobatFitxa = false;
            while (true)
            {
                actual = Posicio(actual.getFila() + df, actual.getColumna() + dc);
                if (!dinsTauler(actual))
                    break;

                if (haTrobatFitxa)
                {
                    if (casellaBuida(actual))
                        posicionsPossibles[nPosicions++] = actual;
                    else
                        break; // Ya no se puede capturar más
                }
                else
                {
                    if (fitxaContraria(actual, fitxa.getColor()))
                        haTrobatFitxa = true;
                    else if (!casellaBuida(actual))
                        break; // Hay una ficha propia, no puede capturar
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

    // Primero buscar capturas obligatorias
    buscarCaptures(origen, nPosicions, posicionsPossibles);

    // Si no hay capturas, buscar movimientos normales
    if (nPosicions == 0) {
        const int MAX_DIR = 4;
        struct Direccio { int df; int dc; };
        Direccio direccions[MAX_DIR] = { {-1,-1}, {-1,1}, {1,-1}, {1,1} };

        if (fitxa.getTipus() == TIPUS_DAMA) {
            // Movimientos para dama
            for (int i = 0; i < MAX_DIR; i++) {
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
        else {
            // Movimientos para fichas normales
            int inicioDir = (fitxa.getColor() == COLOR_BLANC) ? 0 : 2;
            for (int i = inicioDir; i < inicioDir + 2; ++i) {
                Posicio dest(origen.getFila() + direccions[i].df,
                    origen.getColumna() + direccions[i].dc);
                if (dinsTauler(dest) && casellaBuida(dest)) {
                    posicionsPossibles[nPosicions++] = dest;
                }
            }
        }
    }
}

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti) {
    if (!dinsTauler(origen) || !dinsTauler(desti)) return false;
    if (origen == desti) return false;

    Fitxa& fitxaOrigen = m_tauler[origen.getFila()][origen.getColumna()];
    if (fitxaOrigen.getTipus() == TIPUS_EMPTY) return false;

    // Moviments valids
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

    // Fer moviment
    Fitxa fitxaTmp = fitxaOrigen;
    m_tauler[origen.getFila()][origen.getColumna()] = Fitxa(); // Buida origen

    // Mira si era captura
    bool esCaptura = abs(desti.getFila() - origen.getFila()) > 1;

    // Si és captura, eliminar fitxa capturada
    if (esCaptura) {
        int df = (desti.getFila() - origen.getFila()) > 0 ? 1 : -1;
        int dc = (desti.getColumna() - origen.getColumna()) > 0 ? 1 : -1;
        int fila = origen.getFila() + df;
        int columna = origen.getColumna() + dc;

        // Per dama, potser varias fitxes
        bool haCapturat = false;
        while (fila != desti.getFila() && columna != desti.getColumna()) {
            Posicio posIntermedia(fila, columna);
            if (fitxaContraria(posIntermedia, fitxaTmp.getColor())) {
                m_tauler[fila][columna] = Fitxa(); // Elimina la fitxa capturada
                haCapturat = true;
                break; // Solo puede capturar una ficha por salto
            }
            fila += df;
            columna += dc;
        }

        if (!haCapturat) {
            // Moviment no vàlid si era captura i no s'ha capturat
            m_tauler[origen.getFila()][origen.getColumna()] = fitxaTmp;
            return false;
        }
    }

    // Coloca la fitxa en destí
    m_tauler[desti.getFila()][desti.getColumna()] = fitxaTmp;

    // Mira si converteix a dama
    if (fitxaTmp.getTipus() == TIPUS_NORMAL) {
        if ((fitxaTmp.getColor() == COLOR_BLANC && desti.getFila() == 0) ||
            (fitxaTmp.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1)) {
            m_tauler[desti.getFila()][desti.getColumna()].setTipus(TIPUS_DAMA);
        }
    }

    // 🚀 Si ha fet una captura, veure si pot continuar capturant
    if (esCaptura) {
        bool continuaCapturant = true;
        Posicio actual = desti;

        while (continuaCapturant) {
            continuaCapturant = false;
            Posicio captures[MAX_MOVIMENTS];
            int nCaptures = 0;
            buscarCaptures(actual, nCaptures, captures);

            if (nCaptures > 0) {
                continuaCapturant = true;
                // Normalment el jugador hauria d'escollir, però aquí fem automàticament la primera
                Posicio seguentCaptura = captures[0];

                // Simula moviment extra automàtic (com demana el test)
                Fitxa& fitxaActual = m_tauler[actual.getFila()][actual.getColumna()];
                m_tauler[actual.getFila()][actual.getColumna()] = Fitxa();
                m_tauler[seguentCaptura.getFila()][seguentCaptura.getColumna()] = fitxaActual;

                // Elimina fitxa capturada
                int df = (seguentCaptura.getFila() - actual.getFila()) > 0 ? 1 : -1;
                int dc = (seguentCaptura.getColumna() - actual.getColumna()) > 0 ? 1 : -1;
                int fila = actual.getFila() + df;
                int columna = actual.getColumna() + dc;

                while (fila != seguentCaptura.getFila() && columna != seguentCaptura.getColumna()) {
                    Posicio posIntermedia(fila, columna);
                    if (fitxaContraria(posIntermedia, fitxaActual.getColor())) {
                        m_tauler[fila][columna] = Fitxa();
                        break;
                    }
                    fila += df;
                    columna += dc;
                }

                // Avanza a nueva posición
                actual = seguentCaptura;
            }
        }
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