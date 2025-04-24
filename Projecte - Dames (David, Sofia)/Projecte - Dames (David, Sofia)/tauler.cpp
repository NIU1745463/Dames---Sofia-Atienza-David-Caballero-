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

void Tauler::llegeixTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]) // CAMBIAR
{
    ifstream fitxer;
    fitxer.open(nomFitxer);
    string linia;

    if (fitxer.is_open())
    {
        for (int i = 0; i < N_FILES; i++)
        {
            for (int j = 0; j < N_COLUMNES; j++)
            {
                tauler[i][j] = ' ';
            }
        }

        char valor;
        string pos;

        while (fitxer >> valor >> pos)
        {
            char colChar = pos[0];
            char filaChar = pos[1];
            int col = colChar - 'a';
            int fila = 8 - (filaChar - '0');

            if (fila >= 0 && fila < N_FILES && col >= 0 && col < N_COLUMNES)
            {
                tauler[fila][col] = valor;
            }
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

void Tauler::inicialitza(const string& nomFitxer)
{
    netejaTauler();
    char taulerTemp[N_FILES][N_COLUMNES];
    llegeixTauler(nomFitxer, taulerTemp);

    for (int i = 0; i < N_FILES; i++) 
    {
        for (int j = 0; j < N_COLUMNES; j++) 
        {
            switch (taulerTemp[i][j]) {
            case 'B': m_tauler[i][j] = Fitxa(COLOR_BLANC, TIPUS_NORMAL, 1); break;
            case 'N': m_tauler[i][j] = Fitxa(COLOR_NEGRE, TIPUS_NORMAL, 2); break;
            case 'D': m_tauler[i][j] = Fitxa(COLOR_BLANC, TIPUS_DAMA, 1); break;
            case 'Q': m_tauler[i][j] = Fitxa(COLOR_NEGRE, TIPUS_DAMA, 2); break;
            default: m_tauler[i][j] = Fitxa(); break;
            }
        }
    }
    actualitzaMovimentsValids();
}

void Tauler::actualitzaMovimentsValids()
{
    for (int i = 0; i < N_FILES; i++) {
        for (int j = 0; j < N_COLUMNES; j++) {
            if (m_tauler[i][j].getTipus() != TIPUS_EMPTY) {
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

void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const 
{
    nPosicions = 0;
    if (dinsTauler(origen) == false) 
        return;

    const Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];
    if (fitxa.getTipus() == TIPUS_EMPTY) return;

   
    const int MAX_DIRECCIONS = 4;
    struct Direccio { int df; int dc; };
    Direccio direccions[MAX_DIRECCIONS] = {
        {-1,-1}, // Igual que abans valorem les diferents posicions possibles
        {-1, 1}, 
        { 1,-1}, 
        { 1, 1}  
    };

    int inicioDir, numDir;

    if (fitxa.getTipus() == TIPUS_DAMA)  // La dama te 4 possibles direccions
    {
        inicioDir = 0;
        numDir = 4;
    } 
    else 
    {
        // Les fitxes normals 2
        if (fitxa.getColor() == COLOR_BLANC) 
        {
            inicioDir = 0; // Les blanques les 2 primeres y el contrari amb les negres
        }
        else 
        {
            inicioDir = 2; 
        }
        numDir = 2;
    }

    // Busquem moviments simples
    for (int i = inicioDir; i < inicioDir + numDir; ++i) 
    {
        Posicio dest(origen.getFila() + direccions[i].df, origen.getColumna() + direccions[i].dc);
        if (dinsTauler(dest) && casellaBuida(dest)) 
        {
            posicionsPossibles[nPosicions++] = dest;
            if (nPosicions >= MAX_MOVIMENTS) 
                return;
        }
    }

    buscarCaptures(origen, nPosicions, posicionsPossibles);
}

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
    if (dinsTauler(origen) == false) 
        return false;
    if (dinsTauler(desti) == false) 
        return false;
    if (origen == desti) 
        return false;

    // Comprobem si hi ha una fitxa en origen
    Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];
        if (fitxa.getTipus() == TIPUS_EMPTY) 
            return false;

    // Necessitarem els moviments valids i un desti valid
    Posicio movimentsValids[MAX_MOVIMENTS];
    int numMoviments = 0;
    getPosicionsPossibles(origen, numMoviments, movimentsValids);

    bool movimentValid = false;
    for (int i = 0; i < numMoviments; ++i) 
    {
        if (movimentsValids[i] == desti) {
            movimentValid = true;
            break;
        }
    }
    if (movimentValid == false) 
        return false;

    // En cas de pasar tots els requeriments farem el moviment

    Fitxa fitxaTmp = fitxa;
    m_tauler[origen.getFila()][origen.getColumna()] = Fitxa(); // Aixo serveix per vuidar ja que mourem la fitxa

    // Proceessar la captura
    int difFila = abs(desti.getFila() - origen.getFila());
    if (difFila > 1)
    {
        // Direccio de la fila
        int dirFila;
        if (desti.getFila() > origen.getFila()) 
        {
            dirFila = 1; // Mou cap abaix
        }
        else {
            dirFila = -1; // Mou cap adalt
        }

        // Direccio columna
        int dirCol;
        if (desti.getColumna() > origen.getColumna()) 
        {
            dirCol = 1; // Mou derecha
        }
        else {
            dirCol = -1; // Mou esquerra
        }

        int fila = origen.getFila();
        int col = origen.getColumna();
        while (fila != desti.getFila() && col != desti.getColumna()) 
        {
            fila += dirFila;
            col += dirCol;
            if (fitxaContraria(Posicio(fila, col), fitxaTmp.getColor())) 
            {
                m_tauler[fila][col] = Fitxa(); // Eliminar fitxa contraria capturada
            }
        }
    }

    // Situem la fitxa al desti final i comprobem si ha de ser dama
    m_tauler[desti.getFila()][desti.getColumna()] = fitxaTmp;

    // Potser seria millor fer aixo en una funcio
    if (fitxaTmp.getTipus() == TIPUS_NORMAL)
    {
        bool esBlancaEnPrimeraFila = (fitxaTmp.getColor() == COLOR_BLANC && desti.getFila() == 0); 
        bool esNegraEnUltimaFila = (fitxaTmp.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1);

        if (esBlancaEnPrimeraFila == true || esNegraEnUltimaFila == true) 
        {
            m_tauler[desti.getFila()][desti.getColumna()].setTipus(TIPUS_DAMA);
        }
    }

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

string Tauler::toString() const 
{
    ostringstream out;
    for (int i = 0; i < N_FILES; ++i) {
        for (int j = 0; j < N_COLUMNES; ++j) {
            const Fitxa& f = m_tauler[i][j];
            char c = '_';
            if (f.getTipus() == TIPUS_NORMAL)
                c = (f.getColor() == COLOR_BLANC) ? 'O' : 'X';
            else if (f.getTipus() == TIPUS_DAMA)
                c = (f.getColor() == COLOR_BLANC) ? 'D' : 'R';
            out << c << " ";
        }
        out << "\n";
    }
    return out.str();
}