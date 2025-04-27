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
    struct Salto { int df; int dc; } salts[MAX_DIR] = 
    {
        {-2,-2}, // Diferents salts possibles
        {-2, 2},  
        { 2,-2}, 
        { 2, 2}  
    };

    for (int i = 0; i < MAX_DIR; ++i) 
    {
        Posicio dest(origen.getFila() + salts[i].df, origen.getColumna() + salts[i].dc);
        Posicio mig(origen.getFila() + salts[i].df / 2, origen.getColumna() + salts[i].dc / 2);

        if (dinsTauler(dest) && casellaBuida(dest) && fitxaContraria(mig, fitxa.getColor())) 
        {
            posicionsPossibles[nPosicions++] = dest;
            if (nPosicions >= MAX_MOVIMENTS) return;
        }
    }
}

void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const {
    nPosicions = 0;
    if (!dinsTauler(origen)) return;

    const Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];
    if (fitxa.getTipus() == TIPUS_EMPTY) return;

    // Direcciones posibles según tipo de ficha
    const int MAX_DIR = 4;
    struct Direccio { int df; int dc; };
    Direccio direccions[MAX_DIR] = {
        {-1,-1}, {-1,1}, {1,-1}, {1,1} // 4 direcciones diagonales
    };

    int inicioDir, numDir;
    if (fitxa.getTipus() == TIPUS_DAMA) {
        inicioDir = 0;
        numDir = 4; // Damas pueden moverse en las 4 direcciones
    }
    else {
        // Fichas normales solo avanzan
        if (fitxa.getColor() == COLOR_BLANC) {
            inicioDir = 0; // Blancas avanzan hacia arriba (direcciones 0 y 1)
            numDir = 2;
        }
        else {
            inicioDir = 2; // Negras avanzan hacia abajo (direcciones 2 y 3)
            numDir = 2;
        }
    }

    // Primero buscar capturas obligatorias
    buscarCaptures(origen, nPosicions, posicionsPossibles);

    // Si no hay capturas, buscar movimientos simples
    if (nPosicions == 0) {
        for (int i = inicioDir; i < inicioDir + numDir; ++i) {
            Posicio dest(origen.getFila() + direccions[i].df, origen.getColumna() + direccions[i].dc);
            if (dinsTauler(dest) && casellaBuida(dest)) {
                posicionsPossibles[nPosicions++] = dest;
                if (nPosicions >= MAX_MOVIMENTS) return;
            }
        }
    }
}

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti) {
    // 1. Validaciones básicas
    if (!dinsTauler(origen) || !dinsTauler(desti)) return false;
    if (origen == desti) return false;

    Fitxa& fitxaOrigen = m_tauler[origen.getFila()][origen.getColumna()];
    if (fitxaOrigen.getTipus() == TIPUS_EMPTY) return false;

    // 2. Obtener todos los movimientos válidos posibles
    Posicio movimentsValids[MAX_MOVIMENTS];
    int numMoviments = 0;
    getPosicionsPossibles(origen, numMoviments, movimentsValids);

    // 3. Verificar si el destino está entre los movimientos válidos
    bool movimentValid = false;
    for (int i = 0; i < numMoviments; ++i) {
        if (movimentsValids[i] == desti) {
            movimentValid = true;
            break;
        }
    }
    if (!movimentValid) return false;

    // 4. Determinar si el movimiento implica captura
    bool esCaptura = abs(desti.getFila() - origen.getFila()) > 1;
    int maxCaptures = 0;
    bool haCapturat = false;

    // 5. Verificar si hay movimientos con más capturas disponibles
    if (esCaptura) {
        // Buscar el máximo número de capturas posibles desde la posición original
        Posicio posicionsCaptura[MAX_MOVIMENTS];
        int numCaptures = 0;
        buscarCaptures(origen, numCaptures, posicionsCaptura);

        // Para cada movimiento de captura, ver cuántas capturas implica
        for (int i = 0; i < numCaptures; ++i) {
            Posicio destiCaptura = posicionsCaptura[i];
            int saltFila = (destiCaptura.getFila() - origen.getFila()) / 2;
            int saltCol = (destiCaptura.getColumna() - origen.getColumna()) / 2;

            // Contar cuántas fichas se capturarían en este movimiento
            int capturesEnAquestMoviment = abs(saltFila);
            if (capturesEnAquestMoviment > maxCaptures) {
                maxCaptures = capturesEnAquestMoviment;
            }
        }
    }

    // 6. Realizar el movimiento
    Fitxa fitxaTmp = fitxaOrigen;
    m_tauler[origen.getFila()][origen.getColumna()] = Fitxa(); // Vaciar posición origen

    // 7. Procesar capturas si las hay
    if (esCaptura) {
        int dirFila = (desti.getFila() > origen.getFila()) ? 1 : -1;
        int dirCol = (desti.getColumna() > origen.getColumna()) ? 1 : -1;

        int fila = origen.getFila();
        int col = origen.getColumna();
        int capturesRealitzades = 0;

        while (fila != desti.getFila() && col != desti.getColumna()) {
            fila += dirFila;
            col += dirCol;
            Posicio posIntermedia(fila, col);

            if (fitxaContraria(posIntermedia, fitxaTmp.getColor())) {
                m_tauler[fila][col] = Fitxa(); // Eliminar ficha capturada
                capturesRealitzades++;
                haCapturat = true;
                // Saltar la siguiente posición (estamos en un movimiento de captura)
                fila += dirFila;
                col += dirCol;
            }
        }

        // 8. Aplicar regla de "bufar" si no se capturó el máximo posible
        if (haCapturat && capturesRealitzades < maxCaptures) {
            bufarFitxa(fitxaTmp.getColor());
        }
    }
    else {
        // 9. Si no es captura pero había capturas disponibles, "bufar"
        Posicio posicionsCaptura[MAX_MOVIMENTS];
        int numCaptures = 0;
        buscarCaptures(origen, numCaptures, posicionsCaptura);

        if (numCaptures > 0) {
            bufarFitxa(fitxaTmp.getColor());
            return false; // Movimiento no permitido si hay capturas disponibles
        }
    }

    // 10. Colocar la ficha en el destino
    m_tauler[desti.getFila()][desti.getColumna()] = fitxaTmp;

    // 11. Convertir a dama si llega al extremo opuesto
    if (fitxaTmp.getTipus() == TIPUS_NORMAL) {
        bool esBlancaEnPrimeraFila = (fitxaTmp.getColor() == COLOR_BLANC && desti.getFila() == 0);
        bool esNegraEnUltimaFila = (fitxaTmp.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1);

        if (esBlancaEnPrimeraFila || esNegraEnUltimaFila) {
            m_tauler[desti.getFila()][desti.getColumna()].setTipus(TIPUS_DAMA);
        }
    }

    // 12. Actualizar movimientos válidos después del movimiento
    actualitzaMovimentsValids();

    return true;
}

void Tauler::bufarFitxa(ColorFitxa color)
{
    for (int i = 0; i < N_FILES; ++i)
    {
       for (int j = 0; j < N_COLUMNES; ++j)
        {
 
           if (m_tauler[i][j].getColor() == color && m_tauler[i][j].getTipus() != TIPUS_EMPTY)
            {
                Moviment mov;
                int nPosicions = 0;
               Posicio posicionsCaptura[MAX_MOVIMENTS];

                buscarCaptures(Posicio(i, j), nPosicions, posicionsCaptura);

                if (nPosicions == 0)
                {
                    m_tauler[i][j] = Fitxa(); 
                    return; 
               }
          }
       }
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