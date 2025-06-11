#ifndef MOVIMENT_H
#define MOVIMENT_H

#include "fitxa.h"
#include "posicio.hpp"
#include <vector> // Incluimos la librería para usar std::vector

class Tauler;

class Moviment
{
public:
    Moviment();
    ~Moviment(); // Destructor per lliberar la memoria si es necessari 

    void actualitzaMovimentsValids(Tauler& tauler);
    bool mouFitxa(Tauler& tauler, const Posicio& origen, const Posicio& desti);

    // getters i setters
    void setSalts(int size);
    Posicio getSalt(int index) const
    {
        if (index >= 0 && index < static_cast<int>(m_salts.size()))
        {
            return m_salts[index];
        }
        else
        {
            return Posicio();  // Retorna una Posicio per defecte si l'índex no és vàlid
        }
    }

private:
    Fitxa m_fitxa; // el tipus de fitxa que 
    // posicio tant d'inici con de final en respecte a una fitxa
    Posicio m_inici;
    Posicio m_final;
    std::vector<Posicio> m_salts; 
    int m_fitxesMatades;
};

#endif
