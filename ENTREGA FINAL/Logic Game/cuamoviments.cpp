#include "cuamoviments.h"
#include <iostream>
#include <fstream>

// Constructor per defecte 

CuaMoviments::CuaMoviments() : primer(nullptr), ultim(nullptr) {}

// Destructor que elimina si no es buida la cua 
CuaMoviments::~CuaMoviments()
{
    while (!buida())
    {
        treureMoviment();
    }
}

void CuaMoviments::carregarMoviments(const std::string& nomFitxer) // Funcio que carrega els fitxers d'un arxiu
{
    std::ifstream fitxer(nomFitxer);
    std::string origen, desti;

    if (fitxer.is_open())
    {
        while (fitxer >> origen >> desti)
        {
            afegirMoviment(origen + " -> " + desti); // S'afegeix el moviment mentre hi hagi dades a introduir del fitxer
        }
        fitxer.close();
    }
} 

// Afegir un moviment al final de la llista
void CuaMoviments::afegirMoviment(const std::string& moviment)
{
    NodeMoviment* nouNode = new NodeMoviment(moviment); // Es crea un node de moviment i en aquest s'afegira el nou moviment
    if (buida())
    {
        primer = ultim = nouNode; // Si es buit el primer es l'ultim
    }
    else
    {
        ultim->seguent = nouNode; // Si no es vuit, el seguent de l'anterior ultim es el nou
        ultim = nouNode; // l'ultim passa a ser el nou
    }
}

std::string CuaMoviments::treureMoviment() // treu el primer moviment i el retorna

{
    if (buida()) return ""; // no hi ha res a retornar

    NodeMoviment* nodeAEliminar = primer; // marca que el primer node a eliminar es el primer
    std::string moviment = nodeAEliminar->moviment;
    primer = primer->seguent; // el primer pasa a ser el seguent a l'anterior primer

    if (primer == nullptr)
    {
        ultim = nullptr;
    }

    delete nodeAEliminar; // s'elimina el primer
    return moviment; // retorna el moviment eliminat
}

// Funcio que comprova si la cua es buida
bool CuaMoviments::buida() const
{
    return primer == nullptr;
}

// Mostrar tots els moviments fets

void CuaMoviments::mostrarMoviments() const
{
    std::cout << "Moviments realitzats:\n";
    NodeMoviment* actual = primer;
    while (actual) 
    {
        std::cout << actual->moviment << "\n";
        actual = actual->seguent; // Va recorrent tots els moviments de la cua
    }
}

// funcio que guarda els moviments a una archiu determinat
void CuaMoviments::guardarMoviments(const std::string& nomFitxer) const
{
    std::ofstream fitxer(nomFitxer);
    if (fitxer.is_open())
    {
        NodeMoviment* actual = primer;
        while (actual)
        {
            fitxer << actual->moviment << "\n"; // guarda el moviment actual
            actual = actual->seguent; // passa al seguent per despres guardar-lo
        }
        fitxer.close();
    }
}
// Els passa al .txt de moviments, ja que moviments_replay tindra una partida determinada (podria ser la partida copiada de
//moviment .txt, pero com moviment sobrescriu el replay no tindria sentit utilitzar el mateix fitxer

