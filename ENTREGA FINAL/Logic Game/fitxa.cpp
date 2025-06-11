#include "fitxa.h"
#include "GraphicManager.h"

// CONSTRUCTOR PER DEFECTE
Fitxa::Fitxa()
{
    m_colorFitxa = COLOR_BLANC;
    m_tipusFitxa = TIPUS_EMPTY;
    m_nJugador = 0;
}

// CONSTRUCTOR PER PARAMETRE
Fitxa::Fitxa(ColorFitxa color, TipusFitxa tipus, int njugador)
{
    m_colorFitxa = color;
    m_tipusFitxa = tipus;
    m_nJugador = njugador;
}

Fitxa::~Fitxa() // al utilitzar el vector aquest ja fa la gestio
{
}

// Getters i setters de la classe

ColorFitxa Fitxa::getColor() const
{
    return m_colorFitxa;
}

TipusFitxa Fitxa::getTipus() const
{
    return m_tipusFitxa;
}

int Fitxa::getJugador() const
{
    return m_nJugador;
}

void Fitxa::setMoviments(int size)
{
    m_moviments.resize(size);
}

const std::string& Fitxa::getMoviment(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_moviments.size()))
    {
        return m_moviments[index];
    }
    static const std::string emptyString;
    return emptyString;
}

// FUNCIO QUE FARA LA VISUTALITACIO DE LES FITXERS
void Fitxa::visualitza(int x, int y) const
{
    IMAGE_NAME grafico;

    if (m_tipusFitxa == TIPUS_DAMA) // si es dama depenent del color la fara blanca o negra
    {
        if (m_colorFitxa == COLOR_BLANC) 
        {
            grafico = GRAFIC_DAMA_BLANCA;
        }
        else 
        {
            grafico = GRAFIC_DAMA_NEGRA;
        }
    }
    else  // si no es dama depenent del color la fara blanca o negra
    {
        if (m_colorFitxa == COLOR_BLANC)
        {
            grafico = GRAFIC_FITXA_BLANCA;
        }
        else 
        {
            grafico = GRAFIC_FITXA_NEGRA;
        }
    }

    GraphicManager::getInstance()->drawSprite(grafico, x, y); // dibuixa la fitxa en el lloc concret
}
