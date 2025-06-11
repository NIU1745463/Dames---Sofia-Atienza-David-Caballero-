#ifndef JOC_H
#define JOC_H

#include "tauler.hpp"
#include "moviment.h"
#include "cuamoviments.h"
#include <iostream>
#include "info_joc.hpp"
#include <stdio.h>


class Joc
{
public:
    Joc();
    ~Joc();

    // funcions comentades en el .cpp
    void inicialitza(ModeJoc mode, const string& nomFitxerTauler, const string& nomFitxerMoviments); 
    void jugar();
    bool actualitza(int mousePosX, int mousePosY, bool mouseStatus);
    bool validarMoviment(const Posicio& origen, const Posicio& desti);
    void mouFitxa(const Posicio& origen, const Posicio& desti);
    bool haGuanyat() const;
    void mostrarEstat() const;
    string m_nomFitxerMoviments; 
    void finalitza();

private:
    Tauler m_tauler;
    ColorFitxa m_torn; // color de la fitxa que efectua el torn
    CuaMoviments m_movimentsRealitzats;
    ModeJoc m_mode;
    // Comencen inicialitzats a false, ja que no te sentit que estiguin inicialitzas a true
    bool m_partidaFinalitzada = false; 
    bool m_fitxaSeleccionada = false;
    Posicio m_posicioSeleccionada;

};

#endif
