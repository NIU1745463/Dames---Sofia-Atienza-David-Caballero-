#ifndef FITXA_H
#define FITXA_H

#include <vector>
#include <string>

typedef enum
{
    TIPUS_NORMAL,
    TIPUS_DAMA,
    TIPUS_EMPTY
} TipusFitxa;

typedef enum
{
    COLOR_NEGRE,
    COLOR_BLANC,
} ColorFitxa;

class Fitxa
{
public:
    // CONSTRUCTORS
    Fitxa();
    Fitxa(ColorFitxa color, TipusFitxa tipus, int njugador);
    ~Fitxa(); // Destructor per a lliberar memoria

    // FUNCIONS QUE INDIQUEN EL COLOR I TIPUS DE LA FITXA

    ColorFitxa getColor() const;
    TipusFitxa getTipus() const;
    int getJugador() const;

    // funcio que fa el resize del moviments
    void setMoviments(int size);
    const std::string& getMoviment(int index) const;

    void visualitza(int x, int y) const;


private:
    ColorFitxa m_colorFitxa;
    TipusFitxa m_tipusFitxa;
    int m_nJugador;
    std::vector<std::string> m_moviments;
};

#endif