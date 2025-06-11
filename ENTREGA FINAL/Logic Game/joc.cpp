#include "joc.hpp"
#include <iostream>
#include <fstream> // CAMBIO
#include "GraphicManager.h"

// Constructor
Joc::Joc()
{
    m_torn = COLOR_BLANC;
}

// Destructor no necessari en aquesta classe
Joc::~Joc() {}

// Inicializa el juego en funcio del mode y en base del tauler precargat com a inicial, tambe te el fitxer amb moviments)
// AQUESTA FUNCIO SERA CRIDADA DESDE EL MAIN PER PODER EXECUTARLA CORRECTAMENT
// PERMET ELECCIO ENTRE REPETICIO O PARTIDA NORMAL Y CADA MODE GUARDA EN UN ARXIU DIFERENT

void Joc::inicialitza(ModeJoc mode, const string& nomFitxerTauler, const string& nomFitxerMoviments)
{
    m_mode = mode;
    m_nomFitxerMoviments = nomFitxerMoviments; 
    m_tauler.inicialitza(nomFitxerTauler);

    if (mode == MODE_JOC_REPLAY)
    {
        m_movimentsRealitzats.carregarMoviments(nomFitxerMoviments);
    }
}

bool Joc::actualitza(int mousePosX, int mousePosY, bool mouseStatus)
{
    // Mostrar fons i tauler
    GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
    m_tauler.visualitza();

    // Mostra la posicio del ratoli
    int posTextX = POS_X_TAULER;
    int posTextY = POS_Y_TAULER + (ALCADA_CASELLA * NUM_FILES_TAULER) + 120;
    std::string msg = "PosX: " + std::to_string(mousePosX) + ", PosY: " + std::to_string(mousePosY);
    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, posTextX, posTextY, 0.8, msg);

    // Mostrar torn
    std::string tornText;
    if (m_torn == COLOR_BLANC) 
    {
        tornText = "Torn: Blanc";
    }
    else 
    {
        tornText = "Torn: Negre";
    }
    GraphicManager::getInstance()->drawFont(FONT_GREEN_30, POS_X_TAULER, 720, 0.8, tornText);

    // Al actualizar ha de comprobar si es el fi de la partida
    if (m_partidaFinalitzada || haGuanyat())
    {
        m_partidaFinalitzada = true;
        std::string guanyador;
        if (m_torn == COLOR_NEGRE) 
        {
            guanyador = "Blanc";
        }
        else 
        {
            guanyador = "Negre";
        }
        GraphicManager::getInstance()->drawFont(FONT_RED_30, 100, 780, 1.0, "Ha guanyat el jugador " + guanyador);
        return true;
    }

    // Control de clics (clic nou)
    static bool clicAnterior = false;
    bool clicNou = mouseStatus && !clicAnterior;
    clicAnterior = mouseStatus;

    if (clicNou &&
        mousePosX >= POS_X_TAULER + CASELLA_INICIAL_X &&
        mousePosY >= POS_Y_TAULER + CASELLA_INICIAL_Y &&
        mousePosX < POS_X_TAULER + CASELLA_INICIAL_X + NUM_COLS_TAULER * AMPLADA_CASELLA &&
        mousePosY < POS_Y_TAULER + CASELLA_INICIAL_Y + NUM_FILES_TAULER * ALCADA_CASELLA)
    {
        int columna = (mousePosX - (POS_X_TAULER + CASELLA_INICIAL_X)) / AMPLADA_CASELLA;
        int fila = (mousePosY - (POS_Y_TAULER + CASELLA_INICIAL_Y)) / ALCADA_CASELLA;
        Posicio clic(fila, columna);

        if (m_mode == MODE_JOC_NORMAL) // Per seleccionar fitxes haura de ser el mode normal
        {
            if (!m_fitxaSeleccionada) // Si no hi ha cap seleccionada podrem seleccionar
            {
                if (!m_tauler.casellaBuida(clic) && m_tauler.getFitxa(clic).getColor() == m_torn) // Aquesta haura de ser del mateix color que el torn
                {
                    m_fitxaSeleccionada = true; 
                    m_posicioSeleccionada = clic; // la posicio a on anira sera el clic que fara despres de seleccionar
                }
            }
            else // si esta seleccionada podrem moure la fitxa a on es fa el clic
            {
                if (validarMoviment(m_posicioSeleccionada, clic))
                {
                    mouFitxa(m_posicioSeleccionada, clic); 
                    m_fitxaSeleccionada = false; 
                    // s'haura de canviar el torn
                    if (m_torn == COLOR_BLANC) 
                    {
                        m_torn = COLOR_NEGRE;
                    }
                    else 
                    {
                        m_torn = COLOR_BLANC;
                    }
                }
                else 
                    if (!m_tauler.casellaBuida(clic) && m_tauler.getFitxa(clic).getColor() == m_torn)
                {
                    // Permet canviar de seleccio a una fitxa del mateix color
                    m_posicioSeleccionada = clic;
                }
                else
                {
                    // Clic invalid, deseleccionar
                    m_fitxaSeleccionada = false;
                }
            }
        }
        else 
            if (m_mode == MODE_JOC_REPLAY) // En el cas de replay es buscaran els moviments y es mouran en funcio de l'origen i desti
        {
            if (!m_movimentsRealitzats.buida())
            {
                std::string moviment = m_movimentsRealitzats.treureMoviment();
                std::string origenStr = moviment.substr(0, moviment.find(" -> "));
                std::string destiStr = moviment.substr(moviment.find(" -> ") + 4);
                // Amb el que em extret del replay generem l'origen y desti del document
                Posicio origen(origenStr);
                Posicio desti(destiStr);
                mouFitxa(origen, desti);

                // Canvia els torns
                if (m_torn == COLOR_BLANC) 
                {
                    m_torn = COLOR_NEGRE;
                }
                else 
                {
                    m_torn = COLOR_BLANC;
                }
            }
            else
            {
                GraphicManager::getInstance()->drawFont(FONT_RED_30, 100, 780, 0.8, "No queden més moviments.");
            }
        }
    }

    // Mostrar grafic de moviments valids en cada frame si hi ha fitxa seleccionada
    if (m_fitxaSeleccionada)
    {
        int nPosicions = 0;
        Posicio posicionsPossibles[MAX_MOVIMENTS]; // array estatic, ens donaba error si ho intentavem fer dinamic
        m_tauler.getPosicionsPossibles(m_posicioSeleccionada, nPosicions, posicionsPossibles);
        for (int i = 0; i < nPosicions; ++i)
        {
            // Valora totes les posicions de la fitxa per donar les posicions valides
            int posX = POS_X_TAULER + CASELLA_INICIAL_X + (posicionsPossibles[i].getColumna() * AMPLADA_CASELLA);
            int posY = POS_Y_TAULER + CASELLA_INICIAL_Y + (posicionsPossibles[i].getFila() * ALCADA_CASELLA);
            GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, posX, posY);
        }
    }

    return false;
}


// Bucle principal del joc
void Joc::jugar()
{
    while (!haGuanyat()) // Si ningu ha guanya s'efectuen torns
    {
        mostrarEstat();
        string origenStr, destiStr;

        // Marca el torn actual
        std::cout << "Torn de ";
        if (m_torn == COLOR_BLANC) 
        {
            std::cout << "Blanc";
        }
        else 
        {
            std::cout << "Negre";
        }
        std::cout << ". Introdueix el moviment (ex: a1 b2): ";
        std::cin >> origenStr >> destiStr;

        // Passa les cadenes a posicions
        Posicio origen(origenStr);
        Posicio desti(destiStr);

        // Comprova si el moviment és vàlid
        if (validarMoviment(origen, desti))
        {
            mouFitxa(origen, desti); // canvia el color del torn
            if (m_torn == COLOR_BLANC) 
            {
                m_torn = COLOR_NEGRE;
            }
            else 
            {
                m_torn = COLOR_BLANC;
            }
        }
        else
        {
            std::cout << "Moviment no vàlid. Torna-ho a provar!\n";
        }
    }

    // Quan acaba el joc, dona el guanyador
    std::string jugadorGuanyador;
    if (m_torn == COLOR_NEGRE) 
    {
        jugadorGuanyador = "Blanc";
    }
    else 
    {
        jugadorGuanyador = "Negre";
    }
    std::cout << "El jugador " << jugadorGuanyador << " ha guanyat!\n";
}

// Valida el moviment abans dexecutarlo
bool Joc::validarMoviment(const Posicio& origen, const Posicio& desti) // funcio que rep un origen i un desti
{
    int nPosicions = 0;
    Posicio posicionsPossibles[MAX_MOVIMENTS];
    m_tauler.getPosicionsPossibles(origen, nPosicions, posicionsPossibles);

    for (int i = 0; i < nPosicions; ++i)
    {
        if (posicionsPossibles[i] == desti) // Si el desti esta en les possibles posicions, significa que el moviment es valid
            return true;
    }

    return false;
}

// Mou una fitxa i registra el moviment
void Joc::mouFitxa(const Posicio& origen, const Posicio& desti)
{
    if (m_tauler.mouFitxa(origen, desti))
    {
        string moviment = origen.toString() + " -> " + desti.toString();
        m_movimentsRealitzats.afegirMoviment(moviment);
        
        // es comprova si la partida ha acabat per falta de moviments de blanques
        if (m_tauler.comprovarFiPartida(COLOR_BLANC)) 
        {
            // no acaba de ser util aquesta part ja que més adalt ja es fa la funcio per cridar el guanyador
            std::cout << "¡Las negres guanyen! Fi del joc.\n";
            m_partidaFinalitzada = true;
            return;
        }

        // es comprova si la partida ha acabat per falta de moviments de negres
        if (m_tauler.comprovarFiPartida(COLOR_NEGRE)) 
        {
            std::cout << "¡Las blanques gunyen! Fi del joc.\n";
            m_partidaFinalitzada = true;
            return;
        }
    }
}

// Verifica si un jugador ha guanyat
bool Joc::haGuanyat() const
{
    int fitxesBlanques = 0, fitxesNegres = 0;

    for (int i = 0; i < N_FILES; ++i)
    {
        for (int j = 0; j < N_COLUMNES; ++j)
        {
            Fitxa f = m_tauler.getFitxa(Posicio(i, j)); // Aquest bucle fa un recompte de les fitxes

            if (f.getTipus() != TIPUS_EMPTY)
            {
                if (f.getColor() == COLOR_BLANC)
                    fitxesBlanques++;
                else
                    fitxesNegres++;
            }
        }
    }

    return (fitxesBlanques == 0 || fitxesNegres == 0); // en el cas de que no jagi trobat cap fitxa d'algun color en cap posicio 
    // significa que algu haura guanyat, el guanyador es dona amb la funcio que hem vist adalt que crida aquest metode
    
}

// Mostra l'estat del joc
void Joc::mostrarEstat() const
{
    std::cout << "Estat actual del tauler:\n";
    std::cout << m_tauler.toString() << "\n";
    m_movimentsRealitzats.mostrarMoviments();
}

void Joc::finalitza() // Finalitza la partida i la guarda en un document (diferent al del replay)
{
    if (m_mode == MODE_JOC_NORMAL)
    {
        m_movimentsRealitzats.guardarMoviments(m_nomFitxerMoviments);
    }
}